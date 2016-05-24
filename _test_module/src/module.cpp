#include "toast/module.hpp"

#include <iostream>

using namespace Toast;
using namespace std;

class MyModule : public Module {
    public:
        virtual void construct() {
            cout << "Test Module Constructed" << endl;
        }
};

MODULE_CLASS("MyTestModule", true, MyModule);