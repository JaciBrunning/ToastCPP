#include "toast/net/socket.hpp"
#ifndef OS_WIN
	#include <sys/types.h>
	#include <ifaddrs.h>
	#include <net/if.h>
#else
	#include <codecvt>
	#include <IPHlpApi.h>
	#pragma comment(lib, "iphlpapi.lib")
#endif

#include "toast/net/util.hpp"
#include "toast/util.hpp"

using namespace Toast::Net;
using namespace std;

void Util::get_ip(string notation, unsigned char *ip) {
	int ret;
	if (notation == "auto") {
		for (int i = 0; i < Util::get_adapter_count(); i++) {
			Util::Adapter *adapt = Util::get_adapter(i);
			if (adapt->type == Util::AdapterType::ETHERNET || adapt->type == Util::AdapterType::WIRELESS) {
				ret = sscanf(adapt->ip_addr.c_str(), "%d.%d.%d.%d", ip, ip + 1, ip + 2, ip + 3);
				if (ret == 4) return;
			}
		}
	} else {
		ret = sscanf(notation.c_str(), "%d.%d.%d.%d", ip, ip + 1, ip + 2, ip + 3);
		if (ret != 4) {
			// Search for interface
			if (!get_ip_for_adapter(notation, ip)) {
				// Try hostname
				std::string ips = Socket::hostname_to_ip(notation);
				ret = sscanf(ips.c_str(), "%d.%d.%d.%d", ip, ip + 1, ip + 2, ip + 3);
			}
		}
	}

	if (ret != 4) {
		// Default to localhost
		*ip = (unsigned char)127;
		*(ip + 1) = (unsigned char)0;
		*(ip + 2) = (unsigned char)0;
		*(ip + 3) = (unsigned char)1;
	}
}

string Util::get_ip_str(std::string notation) {
	unsigned char *buf = new unsigned char[4];
	Toast::Net::Util::get_ip(notation, buf);
	std::string str = Util::ip_to_string(buf);
	free(buf);
	return str;
}

string Util::ip_to_string(unsigned char *ip) {
	return	std::to_string(*ip) + "." + std::to_string(*(ip + 1)) + "." 
			+ std::to_string(*(ip + 2)) + "." + std::to_string(*(ip + 3));
}

static Util::Adapter _adapter_cache[10];
static int _adapter_count;
static bool _adapter_init = false;

bool Util::get_ip_for_adapter(string adapter, unsigned char *ip) {
	for (int i = 0; i < Util::get_adapter_count(); i++) {
		Util::Adapter *adapt = Util::get_adapter(i);
		if (adapt->name == adapter) {
			int ret = sscanf(adapt->ip_addr.c_str(), "%d.%d.%d.%d", ip, ip + 1, ip + 2, ip + 3);
			if (ret != 4) {
				return false;
			}
			return true;
		}
	}
	return false;
}

static void _populate_adapters();

void Util::refresh_adapters() {
	_populate_adapters();
}

int Util::get_adapter_count() {
	if (!_adapter_init) _populate_adapters();
	return _adapter_count;
}

Util::Adapter *Util::get_adapter(int i) {
	if (!_adapter_init) _populate_adapters();
	if (i >= _adapter_count || i < 0) return NULL;
	return &_adapter_cache[i];
}

#ifdef OS_WIN
static std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;

static string _wstr(wstring &src) {
	return myconv.to_bytes(src);
}

static void _populate_adapters() {
	_adapter_count = 0;

	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	ULONG flags = GAA_FLAG_INCLUDE_PREFIX;
	ULONG family = AF_UNSPEC;

	PIP_ADAPTER_ADDRESSES pAddresses = NULL;
	ULONG outBufLen = 0;
	PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
	PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;

	outBufLen = sizeof(IP_ADAPTER_ADDRESSES);
	pAddresses = (IP_ADAPTER_ADDRESSES *)malloc(outBufLen);
	
	if (GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen) == ERROR_BUFFER_OVERFLOW) {
		free(pAddresses);
		pAddresses = (IP_ADAPTER_ADDRESSES *)malloc(outBufLen);
	}

	dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

	if (dwRetVal == NO_ERROR) {
		pCurrAddresses = pAddresses;
		while (pCurrAddresses) {
			pUnicast = pCurrAddresses->FirstUnicastAddress;
			if (pUnicast != NULL) {
				Util::Adapter a = {
					Util::AdapterType::UNKNOWN,
					_adapter_count,
					_wstr(wstring(pCurrAddresses->FriendlyName)),
					_wstr(wstring(pCurrAddresses->Description)),
					""
				};

				bool unicast = false;

				for (int i = 0; pUnicast != NULL; i++) {
					if (pUnicast->Address.lpSockaddr->sa_family == AF_INET) {
						sockaddr_in *sa_in = (sockaddr_in *)pUnicast->Address.lpSockaddr;
						a.ip_addr = string(inet_ntoa(sa_in->sin_addr));
						unicast = true;
						break;
					}
					pUnicast = pUnicast->Next;
				}
				
				if (unicast) {
					switch (pCurrAddresses->IfType) {
					case IF_TYPE_OTHER:
						a.type = Util::AdapterType::OTHER;
						break;
					case IF_TYPE_ETHERNET_CSMACD:
						a.type = Util::AdapterType::ETHERNET;
						break;
					case IF_TYPE_IEEE80211:
						a.type = Util::AdapterType::WIRELESS;
						break;
					case IF_TYPE_SOFTWARE_LOOPBACK:
						a.type = Util::AdapterType::LOOPBACK;
						break;
					}
					_adapter_cache[_adapter_count] = a;
					_adapter_count++;
					if (_adapter_count == 10) break;
				}
			}
			pCurrAddresses = pCurrAddresses->Next;
		}
	}
	
	free(pAddresses);

	_adapter_init = true;
}
#else
static void _populate_adapters() {
	struct ifaddrs *ifaddr, *ifa;
	int family, n;
	_adapter_count = 0;

	if (getifaddrs(&ifaddr) == -1) return;
	
	for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
		if (ifa->ifa_addr == NULL) continue;

		family = ifa->ifa_addr->sa_family;

		if (family == AF_INET) {
			bool loopback = (0 != (ifa->ifa_flags & IFF_LOOPBACK));
			Util::Adapter a = {
				loopback ? Util::AdapterType::LOOPBACK : Util::AdapterType::ETHERNET,
				_adapter_count,
				string(ifa->ifa_name),
				string(ifa->ifa_name),
				string(inet_ntoa(((struct sockaddr_in *)(ifa->ifa_addr))->sin_addr))
			};

			_adapter_cache[_adapter_count] = a;
			_adapter_count++;
			if (_adapter_count == 10) break;
		}
	}

	if (ifaddr) {
		freeifaddrs(ifaddr);
	}
}
#endif