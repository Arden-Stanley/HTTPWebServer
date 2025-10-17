#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#define DEFAULT_PORT "8080"

#pragma comment(lib, "Ws2_32.lib")

typedef struct {
    struct addrinfo *result, hints;
    SOCKET socket;
} http_socket;

void socket_init();

http_socket socket_create();

void socket_bind(http_socket *listen_socket);

void socket_listen(http_socket *listen_socket);

SOCKET socket_accept(http_socket *listen_socket);

const char* socket_receive(SOCKET client_socket);


#endif