#include "toast.hpp"

#include "io/motor.hpp"
#include "toast/http/server.hpp"

#include <iostream>
#include <thread>

using namespace Toast;
using namespace std;

using namespace IO;

class MyModule : public Module {
    public:
        virtual void construct() {
			Talon t(1);
			t.set(0.5);

			Victor v(1);
			cout << v.get() << endl;
			cout << t.get_type() << endl;
			cout << v.get_type() << endl;

            ofstream outfile("shared.txt", ios::binary);
			outfile.write(Memory::Shared::get(), TOAST_SHARED_MEMPOOL_SIZE);
        }
};

MODULE_CLASS("MyTestModule", false, MyModule);