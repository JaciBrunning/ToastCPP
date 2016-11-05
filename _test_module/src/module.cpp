#include <toast.hpp>
#include <io/io.hpp>

#include <toast/ipc.hpp>
#include <iostream>
#include <regex>

static IO::Talon t(0), t1(1);
static IO::Relay r(0);
static IO::Mapping::XboxController xbox(0);
static IO::DIO d1(0, IO::DIO::Mode::OUTPUT), d2(1, IO::DIO::Mode::INPUT);
static IO::AnalogInput ain(0);
static Toast::Logger _log("MyLogger");
//static IO::I2C i2c(IO::I2C::Port::Onboard, 123);

void interrupt_handler(IO::DIO::InterruptData data) {
	_log << "Interrupt Called on Port: " + std::to_string(data.port) + " edge: " + (data.rising ? "rising" : "falling");
}

class DriveCmd : public Toast::Command {
	void periodic() override {
		t.set(xbox.left_trigger());
		t1.set(xbox.right_trigger());

		r.set_forward(xbox.left_bumper());
		r.set_reverse(xbox.right_bumper());

	}
};

class MyModule : public Toast::IterativeModule {
    public:
        void start() override {
			Toast::Executor::start()->push(new DriveCmd());

//			d1.enable_interrupt();
//			d1.on_interrupt(&interrupt_handler);
//			d1.add_glitch_filter(4200, IO::DIO::GlitchFilterMode::NANOSECONDS);

//			d2.enable_interrupt();
//			d2.on_interrupt(&interrupt_handler);
//			d2.add_glitch_filter(2.0e9, IO::DIO::GlitchFilterMode::NANOSECONDS);

//			i2c.transaction(nullptr, 0, 0, [](IO::I2C::TransactionData data) {
//				std::cout << "I2C: " << data.device_address << std::endl;
//			});
        }
};

MODULE_CLASS("MyTestModule", false, MyModule);