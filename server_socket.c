#include "server_socket.h"

void socket_init() {
    WSADATA wsa_data;
    
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        printf("WSAStartup failed.\n");
        exit(EXIT_FAILURE);
    }

}

http_socket socket_create() {
    http_socket new_socket;

    ZeroMemory(&new_socket.hints, sizeof(new_socket.hints));
    new_socket.hints.ai_family = AF_INET;
    new_socket.hints.ai_socktype = SOCK_STREAM;
    new_socket.hints.ai_protocol = IPPROTO_TCP;
    new_socket.hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, DEFAULT_PORT, &new_socket.hints, &new_socket.result) != 0) {
        printf("Getaddrinfo failed! \n");
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    new_socket.socket = socket(
        new_socket.result->ai_family, 
        new_socket.result->ai_socktype, 
        new_socket.result->ai_protocol
    );

    if (new_socket.socket == INVALID_SOCKET) {
        printf("Error creating socket!\n");
        freeaddrinfo(new_socket.result);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    return new_socket;
}

void socket_bind(http_socket *listen_socket) {
    if (
        bind(
            listen_socket->socket, 
            listen_socket->result->ai_addr, 
            (int)listen_socket->result->ai_addrlen
        ) == SOCKET_ERROR
    ) {
        printf("Bind failed with error!\n");
        freeaddrinfo(listen_socket->result);
        closesocket(listen_socket->socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(listen_socket->result);
}

void socket_listen(http_socket *listen_socket) {
    if (listen(listen_socket->socket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed on socket!\n");
        closesocket(listen_socket->socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
}

SOCKET socket_accept(http_socket *listen_socket) {
    SOCKET client_socket = accept(listen_socket->socket, NULL, NULL);
    if (client_socket == INVALID_SOCKET) {
        printf("Socket accept failed!\n");
        closesocket(listen_socket->socket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    closesocket(listen_socket->socket);
    return client_socket;
}

const char* socket_receive(SOCKET client_socket) {
    char receive_buffer[512];
    int bytes_received;

    do {
        bytes_received = recv(client_socket, receive_buffer, 512, 0);
        if (bytes_received > 0) {
            printf("Bytes received: %d\n", bytes_received);
            if (send(client_socket, receive_buffer, bytes_received, 0) == SOCKET_ERROR) {
                printf("Send failed!\n");
                closesocket(client_socket);
                WSACleanup();
                exit(EXIT_FAILURE);
            }
            printf("Bytes Sent: %d\n", bytes_received);
        } else if (bytes_received == 0) {
            printf("Connection closing...\n");
        }
        else {
            printf("Receive failed!\n");
            closesocket(client_socket);
            WSACleanup();
            exit(EXIT_FAILURE);
        }
    } while (bytes_received > 0); 
    
    shutdown(client_socket, SD_SEND);
    close_socket(client_socket);
    WSACleanup();
    return receive_buffer; 
}