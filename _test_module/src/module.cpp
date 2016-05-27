#include "toast/module.hpp"
#include "toast/state.hpp"
#include "toast/logger.hpp"
#include "json11.hpp"

#include <iostream>

using namespace Toast;
using namespace std;
using namespace json11;

class MyModule : public Module {
    public:
        virtual void construct() {
        }
};

MODULE_CLASS("MyTestModule", false, MyModule);