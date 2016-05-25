#include "toast/module.hpp"

#include <iostream>

using namespace Toast;
using namespace std;

class MyModule : public Module {
    public:
        virtual void construct() {
        }
};

MODULE_CLASS("MyTestModule", false, MyModule);