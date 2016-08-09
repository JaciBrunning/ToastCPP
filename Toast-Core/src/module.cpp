#include "toast/module.hpp"

#include "toast/logger.hpp"

using namespace std;

vector<Toast::ModuleData> Toast::get_all_modules() {
    vector<Toast::ModuleData> v;
    int i;
	for (i = 0; i < 128; i++) {
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
    return v;
}