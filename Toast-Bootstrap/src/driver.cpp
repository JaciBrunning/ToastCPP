#include "toast/bootstrap/driver.hpp"
#include "toast/filesystem.hpp"
#include "toast/util.hpp"
#include "toast/logger.hpp"

using namespace Toast;
using namespace Toast::Bootstrap;
using namespace std;

static vector<Driver::BootstrapDriver *> __drivers;
static Logger _log("Bootstrap-Drivers");

template <class ret = void>
static ret dyn_call(Driver::BootstrapDriver *driver, std::string name) {
	DYNAMIC dyn = driver->lib;
	if (dyn != NULL) {
		SYMBOL sym = Internal::Loader::get_symbol(dyn, name);
		if (sym == NULL) return static_cast<ret>(NULL);
		return reinterpret_cast<ret(*)()>(sym)();
	}
	// This has to be a static cast, or else when ret=void, this does `return NULL` on a void function,
	// resulting in a compilation error.
	return static_cast<ret>(NULL);
}

void Driver::initialize() {
	Driver::search_drivers();
}

void Driver::free() {
	for (std::vector< Driver::BootstrapDriver * >::iterator it = __drivers.begin(); it != __drivers.end(); ++it) {
		dyn_call(*it, "driver_free");
		delete (*it);
	}
}

void Driver::search_drivers() {
	vector<string> driver_files = Filesystem::ls_local(Filesystem::path("drivers"));
	for (auto driver : driver_files) {
		if (ends_with(driver, OS_LIB)) {
			Driver::BootstrapDriver *bd = new Driver::BootstrapDriver();
			bd->file = driver;
			bd->lib = Internal::Loader::load_dynamic_library(driver);
			if (bd->lib == NULL) {
				_log.info("Driver " + driver + " could not be loaded. Is this file corrupt?");
			} else {
				__drivers.push_back(bd);
				_log.info("Discovered Driver: " + driver);
			}
		} else {
			_log.warn("Driver File " + driver + " does not have a valid file extension! Are you sure this is the correct download for your platform? Valid File Extension: " + OS_LIB);
		}
	}
}

void Driver::preinit_drivers() {
	for (auto driver : __drivers) {
		dyn_call(driver, "driver_preinit");
	}
}

void Driver::init_drivers() {
	for (auto driver : __drivers) {
		dyn_call(driver, "driver_init");
	}
}