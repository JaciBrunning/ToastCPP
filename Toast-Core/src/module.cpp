#include "toast/module.hpp"


using namespace std;
static vector<Toast::ModuleData> v;
static int mods_found = 0;

static void load_module_datas() {
	int i = 0, current_mods = 0;
	for (i = 0; i < 128; i++) {
		Toast::Memory::ModuleActState state = Toast::Memory::shared()->get_module_activity_state(i);
		if (state != Toast::Memory::ModuleActState::NOT_FOUND) {
			current_mods = i+1;
		} else {
			break;
		}
	}

	// Reload modules if any more have popped up.
	if (current_mods != mods_found) {
		v.clear();
		for (i = 0; i < current_mods; i++) {
			Toast::Memory::ModuleActState state = Toast::Memory::shared()->get_module_activity_state(i);

			if (state != Toast::Memory::ModuleActState::NOT_FOUND) {
				Toast::Memory::PrivatePool pool;
				char buf[pool.SIZE];
				Toast::Memory::copy_private_pool(i, &buf[0]);
				pool.map_to(buf);

				string name_str(pool.get_module_name());
				string file_str(pool.get_module_filepath());

				Toast::ModuleData d = { name_str, file_str, i, state };
				v.push_back(d);
			}
		}
		mods_found = current_mods;
	}
}

vector<Toast::ModuleData> Toast::get_all_modules() {
	load_module_datas();
    return v;
}

int Toast::module_id(std::string name) {
	load_module_datas();
	for (Toast::ModuleData data : v) {
		if (data.name == name) return data.module_idx;
	}
	return -1;
}