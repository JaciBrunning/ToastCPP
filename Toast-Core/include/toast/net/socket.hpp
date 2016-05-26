#pragma once

// IMPORTANT! 
// If you get a whole bunch of macro redefinition errors on windows,
// move this include up to be the first include in your file. Something
// weird with Windows' winsock library causes a whole bunch of conflicts
// with windows.h, so this must be done first. Don't blame me, blame
// microsoft.

#include "toast/library.hpp"
#include "toast/environment.hpp"

#ifdef OS_WIN
  #ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0501
  #endif
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #pragma comment (lib, "Ws2_32.lib")
  #pragma comment (lib, "Mswsock.lib")
  #pragma comment (lib, "AdvApi32.lib")
#else
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <netdb.h>
  #include <unistd.h>
  #include <string.h>
#endif

#include <string>

namespace Toast {
    namespace Net {
        namespace Socket {
            #ifdef OS_WIN
                typedef ::SOCKET SOCKET;
            #else
                typedef int SOCKET;
            #endif
            typedef struct sockaddr_in SocketAddress;
            
            API int socket_init();
            API SOCKET socket_create();
            API SOCKET socket_udp_create();
            API int socket_connect(SOCKET s, std::string host, int port);
            API int socket_bind(SOCKET s, int port);
            API void socket_listen(SOCKET s);
            API SOCKET socket_accept(SOCKET s, Socket::SocketAddress *addr);
            API int socket_quit();
            API int socket_close(SOCKET sock);
            API std::string hostname_to_ip(std::string hostname);
            API int socket_port(SocketAddress *addr);
            API std::string socket_host(SocketAddress *addr);
            
            class ClientSocket {
                public:
                    API ClientSocket() {
                        _socket = Toast::Net::Socket::socket_create();
                    }
                    API ClientSocket(std::string hostname, int host_port) : ClientSocket() {
                        host = hostname;
                        port = host_port;
                    }
                    API ClientSocket(Toast::Net::Socket::SOCKET socket) {
                        _socket = socket;
                    }
                    
                    API Toast::Net::Socket::SOCKET get_socket() {
                        return _socket;
                    }
                    
                    API int connect();
                    API int close();
                    
                    API int send(const char *buffer, size_t length, int flags);
                    API int send(const char *buffer, size_t length);
                    API int send(std::string message);
                    
                    API int read(char *buf, size_t length);
                    
                    API int get_port() { return port; }
                    API std::string get_host() { return host; }
                    
                    std::string host;
                    int port;
                private:
                    Toast::Net::Socket::SOCKET _socket;
            };
            
            class ServerSocket {
                public:
                    API ServerSocket(int server_port) {
                        port = server_port;
                        _socket = Toast::Net::Socket::socket_create();
                    }
                    
                    API int open();
                    API int close();
                    
                    API ClientSocket accept();
                    
                    API int get_port() { return port; }
                    
                    int port;
                private:
                    Toast::Net::Socket::SOCKET _socket;
            };
        }
    }
}