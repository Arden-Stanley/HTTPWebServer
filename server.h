#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "8080"


typedef struct http_server {};

typedef struct http_socket {
    struct addrinfo *result, hints;
    SOCKET sock;
};

http_server server_create() {
    WSADATA wsa_data;
    
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        printf("WSAStartup failed.\n");
        exit(EXIT_FAILURE);
    }

    http_server server;
    return server;
}

http_socket socket_create() {
    http_socket new_socket;

    ZeroMemory(&new_socket.hints, sizeof(new_socket.hints));
    new_socket.hints.ai_family = AF_INET;
    new_socket.hints.ai_socktype = SOCK_STREAM;
    new_socket.hints.ai_protocol = IPPROTO_TCP;
    new_socket.hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, DEFAULT_PORT, &new_socket.hints, &new_socket.result) != 0) {
        printf("getaddrinfo failed! \n");
        WSACleanup();
    }

    new_socket.sock = socket(
        new_socket.result->ai_family, 
        new_socket.result->ai_socktype, 
        new_socket.result->ai_protocol
    );

    if (new_socket.sock == INVALID_SOCKET) {
        printf("Error creating socket!\n");
        freeaddrinfo(new_socket.result);
        WSACleanup();
    }
    return new_socket;
}

void socket_bind(http_socket *listen_socket) {
    if (
        bind(
            listen_socket->sock, 
            listen_socket->result->ai_addr, 
            (int)listen_socket->result->ai_addrlen
        ) == SOCKET_ERROR
    ) {
        printf("Bind failed with error!\n");
        freeaddrinfo(listen_socket->result);
        closesocket(listen_socket->sock);
        WSACleanup();
    }

    freeaddrinfo(listen_socket->result);
}

void socket_listen(http_socket *listen_socket) {
    if (listen(listen_socket->sock, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed on socket!\n");
        closesocket(listen_socket->sock);
        WSACleanup();
    }

    SOCKET client_socket;
}

#endif