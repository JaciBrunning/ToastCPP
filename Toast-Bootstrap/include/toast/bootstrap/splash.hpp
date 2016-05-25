#pragma once

#include "toast/library.hpp"

#include <string>

namespace Toast {
    namespace Splash {
        API std::string get_startup_splash() {
            return
R"( ________    ______                 __     ______
((       )  /_  __/___  ____ ______/ /_   / ____/__    __ 
||  o o  |   / / / __ \/ __ `/ ___/ __/  / /  __/ /___/ /_
||   3   |  / / / /_/ / /_/ (__  ) /_   / /__/_  __/_  __/
\\_______/ /_/  \____/\__,_/____/\__/   \____//_/   /_/   )";
        }
        
        API std::string get_error_splash() {
            return
R"( ________     __  ____       ____  __
((       )   / / / / /_     / __ \/ /_
||  x x  |  / / / / __ \   / / / / __ \
||   ^   | / /_/ / / / /  / /_/ / / / /
\\_______/ \____/_/ /_/   \____/_/ /_/)";
        }
        
    }
}