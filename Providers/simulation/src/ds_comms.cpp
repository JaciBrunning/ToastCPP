#include "toast/net/socket.hpp"
#include "thp/sim/ds_comms.hpp"

#include "toast/memory.hpp"
#include "toast/state.hpp"
#include "toast/concurrent/mutex.hpp"
#include "toast/util.hpp"
#include "toast/crash.hpp"
#include "toast/logger.hpp"
#include "io/common.hpp"

#include <thread>
#include <iostream>

using namespace Sim;
using namespace Toast::Memory;

static char encode_buffer[8];
static char decode_buffer[1024];
static char tcp_buffer[1024];
static char tcp_write_buffer[1024];

static Toast::Concurrent::Mutex mtx;
static bool server_running;

static std::thread udp_thread;
static std::thread tcp_thread;

static Toast::Logger logger("Driver Station Comms");

static void tcp_thread_func() {
	Toast::Net::Socket::ServerSocket sock(1740);
	sock.open();
	
	while (true) {
		Toast::Net::Socket::ClientSocket client = sock.accept();
		int ret = 0;
		while (ret > -1) {
			ret = client.read(tcp_buffer, 8192);
			if (ret > -1) {
				DriverStationComms::decode_ds_tcp_packet(tcp_buffer, ret);
			}
		}
		client.close();
	}

	sock.close();
}

static void udp_thread_func() {
	Toast::Net::Socket::DatagramSocket sock(1110);
	sock.bind();
	
	Toast::Net::Socket::SocketAddress addr;
	while (true) {
		mtx.lock();
		if (!server_running)
			break;
		mtx.unlock();
	
		int len = sock.read(decode_buffer, 1024, &addr);
		DriverStationComms::decode_ds_packet(decode_buffer, len);
		
		DriverStationComms::encode_ds_packet(encode_buffer);
		addr.set_port(1150);
		sock.send(encode_buffer, 8, &addr);

	}
	sock.close();
}

void DriverStationComms::start() {
	if (!server_running) {
		Toast::Net::Socket::socket_init();

		mtx.lock();
		server_running = true;
		mtx.unlock();
		
		udp_thread = std::thread(udp_thread_func);
		udp_thread.detach();

		tcp_thread = std::thread(tcp_thread_func);
		tcp_thread.detach();
	}
}

void DriverStationComms::stop() {
	mtx.lock();
	server_running = false;
	mtx.unlock();
}

typedef struct {
	uint8_t axis_count, pov_count, button_count;
	uint16_t pov[4];
	uint8_t axis[16];
	uint32_t button_mask;
} _TempJoyData;

static uint8_t sq_1 = 0, sq_2 = 0, control = 0, req = 0;
static _TempJoyData joys[6];
static long last_decode_time;

void DriverStationComms::decode_ds_packet(char *data, int length) {
	sq_1 = data[0]; sq_2 = data[1];
	if (data[2] != 0) {
		control = data[3];
		RobotState state = RobotState::DISABLED;
		bool estop = FWI_IS_BIT_SET(control, 7);	// TODO Write Estop
		bool fms = FWI_IS_BIT_SET(control, 3);

		if (FWI_IS_BIT_SET(control, 2)) {
			if (FWI_IS_BIT_SET(control, 0)) {
				state = RobotState::TEST;
			} else {
				if (FWI_IS_BIT_SET(control, 1)) {
					state = RobotState::AUTO;
				} else {
					state = RobotState::TELEOP;
				}
			}
		}

		req = data[4];
		bool reboot = FWI_IS_BIT_SET(req, 3);
		bool restart = FWI_IS_BIT_SET(req, 2);

		if (reboot || restart) {
			logger << "NOTICE: Driver Station Requested Code Restart";
			Toast::Crash::shutdown();
		}

		int alliance_position = data[5] % 3 + 1;
		Shared::DS::Alliance alliance = data[5] < 3 ? Shared::DS::Alliance::Red : Shared::DS::Alliance::Blue;

		int i = 6;
		bool search = true;
		int joy_id = 0;

		while (i < length && search) {
			int struct_size = data[i];
			search = data[i + 1] == 0x0c;
			if (!search) continue;

			_TempJoyData *joy = &joys[joy_id];

			joy->axis_count = data[i + 2];
			for (int ax = 0; ax < joy->axis_count; ax++) {
				joy->axis[ax] = data[i + 2 + ax + 1];
			}

			int b = i + 2 + joy->axis_count + 1;

			joy->button_count = data[b];
			int button_delta = (joy->button_count / 8 + ((joy->button_count % 8 == 0) ? 0 : 1));
			uint32_t total_mask = 0;
			for (int bm = 0; bm < button_delta; bm++) {
				uint8_t m = data[b + bm + 1];
				total_mask = (total_mask << (bm * 8)) | m;
			}
			joy->button_mask = total_mask;

			b += button_delta + 1;

			joy->pov_count = data[b];
			for (int pv = 0; pv < joy->pov_count; pv++) {
				uint8_t a1 = data[b + 1 + (pv * 2)];
				uint8_t a2 = data[b + 1 + (pv * 2) + 1];
				if (a2 < 0) a2 = 256 + a2;
				joy->pov[pv] = (uint16_t)(a1 << 8 | a2);
			}

			joy_id++;
			i += struct_size + 1;
		}

		// Write it all to the shared pool
		Toast::States::Internal::set_state(state);
		shared()->ds_info()->set_alliance(alliance);
		shared()->ds_info()->set_alliance_station(alliance_position);
		shared()->ds_info()->set_fms_attached(fms);

		last_decode_time = current_time_millis();
	}
}

void DriverStationComms::encode_ds_packet(char *data) {
	data[0] = sq_1; data[1] = sq_2;
	data[2] = 0x01;
	data[3] = control;
	data[4] = 0x10 | 0x20;
	
	double voltage = 12.9;	// TODO: Wire this to somewhere else.

	data[5] = (uint8_t)(voltage);
	data[6] = (uint8_t)((voltage * 100 - ((uint8_t)voltage) * 100) * 2.5);
	data[7] = 0;
}

void DriverStationComms::decode_ds_tcp_packet(char *data, int length) {
	if (data[2] == 0x02) {
		// Joystick Descriptor
		int i = 3;
		while (i < length) {
			uint8_t joyid = data[i++];
			bool xbox = data[i++] == 1;
			uint8_t type = data[i++];
			uint8_t name_length = data[i++];
			int nb_i = i;
			i += name_length;
			uint8_t axis_count = data[i++];
			uint8_t axis_types[16];
			int at_i = i;
			i += axis_count;
			uint8_t button_count = data[i++];
			uint8_t pov_count = data[i++];

			if (type != 255 && axis_count != 255) {
				MTX_LOCK(shared_mutex()->joy, joyid);
				Shared::DS::JoystickDescriptor *j = shared()->joystick(joyid)->get_descriptor();
				j->set_is_xbox(xbox);
				j->set_type((Shared::DS::JoystickType) type);
				if (name_length > 60) name_length = 60;
				j->set_name_length(name_length);
				memcpy(j->get_name(), &data[nb_i], name_length);
				j->set_axis_count(axis_count);
				for (int x = 0; x < axis_count; x++) {
					j->set_axis_type(x, (Shared::DS::JoystickAxisType) data[at_i + x]);
				}
				MTX_UNLOCK(shared_mutex()->joy, joyid);
			}
		}
	}
}
bool last = false;

void DriverStationComms::periodic_update() {
	if (current_time_millis() - last_decode_time > 1000) {
		// DS Disconnected
		shared()->ds_info()->set_ds_attached(false);
	} else {
		// DS Connected
		shared()->ds_info()->set_ds_attached(true);
	}

	for (int i = 0; i < 6; i++) {
		MTX_LOCK(shared_mutex()->joy, i);
		Shared::DS::Joystick *j = shared()->joystick(i);
		_TempJoyData *t = &joys[i];

		j->set_num_axis(t->axis_count);
		j->set_num_button(t->button_count);
		j->set_num_pov(t->pov_count);

		j->set_button_mask(t->button_mask);

		for (int x = 0; x < j->get_num_axis(); x++) {
			j->set_axis(x, t->axis[x]);
		}

		for (int x = 0; x < j->get_num_pov(); x++) {
			j->set_pov(x, t->pov[x]);
		}
		MTX_UNLOCK(shared_mutex()->joy, i);
	}
}