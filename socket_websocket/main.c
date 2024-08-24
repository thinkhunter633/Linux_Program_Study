#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 5000

int main() {
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // 创建socket文件描述符
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 绑定socket到指定端口
    if (bind(server_fd, (struct sockaddr*)&address, addrlen) < 0) {
        perror("Failed to bind socket");
        exit(EXIT_FAILURE);
    }

    // 监听socket，等待客户端连接请求
    if (listen(server_fd, 3) < 0) {
        perror("Failed to listen on socket");
        exit(EXIT_FAILURE);
    }

    printf("Listening on port %d...\n", PORT);

    while (1) {
        // 接受客户端连接请求，并创建新的socket文件描述符来处理该连接
        int new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("Failed to accept client connection");
            exit(EXIT_FAILURE);
        }

        printf("Client connected.\n");

        char buffer[1024] = {0};
        int buflen = recv(new_socket, buffer, 1024, 0);
        if (buflen < 0) {
            perror("Failed to receive data from client");
            exit(EXIT_FAILURE);
        }

        printf("Received: %s\n", buffer);

        // 构造响应头
        char response[1024];
        sprintf(response, "HTTP/1.1 101 Switching Protocols\r\n"
                           "Upgrade: websocket\r\n"
                           "Connection: Upgrade\r\n"
                           "Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=\r\n\r\n");

        // 发送响应头给客户端，完成websocket握手
        int sendret = send(new_socket, response, strlen(response), 0);
        if (sendret < 0) {
            perror("Failed to send response to client");
            exit(EXIT_FAILURE);
        }

        printf("Websocket handshake completed.\n");

        // 循环读取和发送数据
        while (1) {
            char recvbuffer[1024];
            buflen = recv(new_socket, recvbuffer, 1024, 0);
            if (buflen < 0) {
                perror("Failed to receive data from client");
                exit(EXIT_FAILURE);
            }

            if (buflen == 0) {
                printf("Client disconnected.\n");
                close(new_socket);
                break;
            }

            // 获取消息类型（第一个字节的后7位）和消息长度（第二个字节的后7位）
            int opcode = recvbuffer[0] & 0x7f;
            int length = recvbuffer[1] & 0x7f;

            if (opcode == 8) {   // 收到关闭消息
                printf("Received close message.\n");
                close(new_socket);
                break;
            } else if (opcode == 1) {   // 收到文本消息
                char* message = recvbuffer + 2;   // 去掉消息头部信息，获取真正的消息内容
                printf("Received text message: %s\n", message);

                // 回复收到的消息
                char response[1024];
                response[0] = 0x81;   // FIN=1、RSV1-3=0、opcode=1（文本消息）
                response[1] = strlen(message);   // 消息长度
                memcpy(response + 2, message, strlen(message));   // 消息内容

                send(new_socket, response, strlen(message) + 2, 0);
            }
        }
    }

    return 0;
}
