#include "server_socket.h"

int main() {
    socket_init();

    http_socket server_socket = socket_create();
    socket_bind(&server_socket);
    socket_listen(&server_socket);
    socket_receive(socket_accept(&server_socket));

    return 0;   
}