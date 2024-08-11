
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(void)
{
    int http_socket;
    uint16_t wSocketPort = 58888;
    struct sockaddr_in sockaddr;

    http_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (http_socket < 0)
        printf("socket error\r\n");
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockaddr.sin_port = htons(wSocketPort);
    if (bind(http_socket, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
        printf("bind error\r\n");
    if (listen(http_socket, 5) < 0)
        printf("listen error\r\n");

    printf("hello world! \r\n");

    while(1)
    {
        ;
    }
    return 0;
}
