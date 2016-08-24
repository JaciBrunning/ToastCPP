#include "toast/net/socket.hpp"

using namespace Toast::Net;
using namespace std;

int Socket::socket_init() {
    #ifdef OS_WIN
        WSADATA wsa_data;
        return WSAStartup(MAKEWORD(1, 1), &wsa_data);
    #else
        return 0;
    #endif
}

Socket::SOCKET Socket::socket_create() {
    Socket::SOCKET s;
    s = socket(AF_INET, SOCK_STREAM, 0);
    return s;
}

Socket::SOCKET Socket::socket_udp_create() {
    Socket::SOCKET s;
    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    return s;
}

Socket::SocketAddress::SocketAddress(string host, int port) {
	inaddr_struct.sin_addr.s_addr = inet_addr((char *)host.c_str());
	inaddr_struct.sin_family = AF_INET;
	inaddr_struct.sin_port = htons(port);
	inaddr_length = sizeof(inaddr_struct);
}

string Socket::SocketAddress::host() {
	return string(inet_ntoa(inaddr_struct.sin_addr));
}

int Socket::SocketAddress::port() {
	return ntohs(inaddr_struct.sin_port);
}

void Socket::SocketAddress::set_host(string host) {
	inaddr_struct.sin_addr.s_addr = inet_addr((char *)host.c_str());
	inaddr_struct.sin_family = AF_INET;
	inaddr_length = sizeof(inaddr_struct);
}

void Socket::SocketAddress::set_port(int port) {
	inaddr_struct.sin_port = htons(port);
	inaddr_length = sizeof(inaddr_struct);
}

Socket::SocketAddress Socket::socket_address(std::string host, int port) {
	struct sockaddr_in addr;
	addr.sin_addr.s_addr = inet_addr((char *)host.c_str());
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	return SocketAddress(addr);
}

int Socket::socket_connect(Socket::SOCKET s, string host, int port) {
    struct sockaddr_in host_addr;
    host_addr.sin_addr.s_addr = inet_addr((char *)host.c_str());
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(port);
    return connect(s, (struct sockaddr *)&host_addr, sizeof(host_addr));
}

int Socket::socket_bind(Socket::SOCKET s, int port) {
	struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    
    return bind(s, (struct sockaddr *)&addr, sizeof(addr));
}

void Socket::socket_listen(Socket::SOCKET s) {
    listen(s, 5);
}

Socket::SOCKET Socket::socket_accept(SOCKET s, Socket::SocketAddress *addr) {
	#ifdef OS_WIN
		int *ptr = addr->raw_address_len_ptr();
	#else
		unsigned int *ptr = (unsigned int *)addr->raw_address_len_ptr();
	#endif
    return accept(s, (struct sockaddr *)addr->raw_address(), ptr);
}

int Socket::socket_quit() {
  #ifdef OS_WIN
    return WSACleanup();
  #else
    return 0;
  #endif
}

int Socket::socket_close(Socket::SOCKET sock) {
  int status = 0;

  #ifdef OS_WIN
    status = shutdown(sock, SD_BOTH);
    if (status == 0) { status = closesocket(sock); }
  #else
    status = shutdown(sock, SHUT_RDWR);
    if (status == 0) { status = close(sock); }
  #endif

  return status;
}

string Socket::hostname_to_ip(string hostname) {
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
         
    if ((he = gethostbyname((char *)hostname.c_str())) == NULL) {
        return NULL;
    }
 
    addr_list = (struct in_addr **) he->h_addr_list;
     
    for(i = 0; addr_list[i] != NULL; i++) {
        return string(inet_ntoa(*addr_list[i]));
    }
     
    return NULL;
}

int Socket::ClientSocket::connect() {
    #ifdef SO_NOSIGPIPE
        const int set = 1;
        setsockopt(_socket, SOL_SOCKET, SO_NOSIGPIPE, &set, sizeof(set));
    #endif
    return Socket::socket_connect(_socket, Socket::hostname_to_ip(host), port);
}

int Socket::ClientSocket::close() {
    return Socket::socket_close(_socket);
}

int Socket::ClientSocket::send(const char *buffer, size_t length, int flags) {
    return ::send(_socket, buffer, length, flags);
}

int Socket::ClientSocket::send(const char *buffer, size_t length) {
    #ifdef MSG_NOSIGNAL
        return Socket::ClientSocket::send(buffer, length, MSG_NOSIGNAL);
    #else
        return Socket::ClientSocket::send(buffer, length, 0);
    #endif
}

int Socket::ClientSocket::send(string message) {
    return Socket::ClientSocket::send(message.c_str(), message.length());
}

int Socket::ClientSocket::read(char *buf, size_t length) {
    #ifdef OS_WIN
        return ::recv(_socket, buf, length, 0);
    #else
        return ::read(_socket, buf, length);
    #endif
}

int Socket::ServerSocket::open() {
    int ret = Socket::socket_bind(_socket, port);
    if (ret != 0) {
        return ret;
    }
    Socket::socket_listen(_socket);
    return 0;
}

int Socket::ServerSocket::close() {
    return Socket::socket_close(_socket);
}

Socket::ClientSocket Socket::ServerSocket::accept() {
    Socket::SocketAddress addr;
    Socket::SOCKET sid = Socket::socket_accept(_socket, &addr);
    
    ClientSocket socket(sid);
    socket.host = addr.host();
	socket.port = addr.port();
    return socket;
}

int Socket::DatagramSocket::bind() {
	return Socket::socket_bind(_socket, port);
}

int Socket::DatagramSocket::read(char *buf, size_t length, Socket::SocketAddress *addr) {
	#ifdef OS_WIN
		int *ptr = addr->raw_address_len_ptr();
	#else
		unsigned int *ptr = (unsigned int *)addr->raw_address_len_ptr();
	#endif
	return ::recvfrom(_socket, buf, length, 0, (struct sockaddr *)addr->raw_address(), ptr);
}

int Socket::DatagramSocket::send(const char *buf, size_t length, Socket::SocketAddress *addr) {
	return sendto(_socket, buf, length, 0, (struct sockaddr *)addr->raw_address(), addr->raw_address_length());
}

int Socket::DatagramSocket::close() {
	return Socket::socket_close(_socket);
}