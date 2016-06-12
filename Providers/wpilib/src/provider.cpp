#include "provider/provider.hpp"

using namespace std;

static ProviderInfo info = {
    "WPILib (2016)",
    false, true
};

ProviderInfo *provider_info() {
    return &info;
}

void provider_init() {

}