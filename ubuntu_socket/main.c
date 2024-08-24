
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "frame_procedure.h"

int main(void)
{
    int server_socket;
    int client_socket;
    uint16_t wSocketPort = 58888;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t client_name_len = sizeof(client_addr);

    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
        printf("socket error\r\n");
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(wSocketPort);
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        printf("bind error\r\n");
    if (listen(server_socket, 5) < 0)
        printf("listen error\r\n");

    printf("hello world! \r\n");

    while(1)
    {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_name_len);
        if (client_socket < 0)
            printf("accept error\r\n");
        else
        {
            printf("client connected\r\n");
            printf("client ip: %s\r\n", inet_ntoa(client_addr.sin_addr));
            int32_t i32recv_len = 0;
            char recv_buf[1024];
            memset(recv_buf, 0, sizeof(recv_buf));
            i32recv_len = recv(client_socket, recv_buf, 1024, 0);
            if (i32recv_len > 0) {
                printf("recv length:%d, data: %s\r\n", i32recv_len, recv_buf);
                //send(client_socket, recv_buf, i32recv_len, 0);
                frame_procedure(recv_buf, i32recv_len);
            }
        }
    }
    return 0;
}
