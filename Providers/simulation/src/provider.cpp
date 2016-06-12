#include "provider/provider.hpp"

using namespace std;

static ProviderInfo info = {
    "Simulation",
    true, false
};

ProviderInfo *provider_info() {
    return &info;
}

void provider_init() {

}