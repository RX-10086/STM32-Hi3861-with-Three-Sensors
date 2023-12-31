/* Copyright (c) 2022 HiSilicon (Shanghai) Technologies CO., LIMITED */

#include <stdio.h>
#include "lwip/sockets.h"
#include "net_demo.h"

static char g_request[] = "Hello,I am Lwip";
static char g_response[128] = "";

void TcpClientTest(const char *host, unsigned short port)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // TCP socket

    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;   // AF_INET表示IPv4协议
    serverAddr.sin_port = htons(port); // 端口号，从主机字节序转为网络字节序
    if (inet_pton(AF_INET, host, &serverAddr.sin_addr) <= 0)
    { // 将主机IP地址从“点分十进制”字符串 转化为 标准格式（32位整数）
        printf("inet_pton failed!\r\n");
        lwip_close(sockfd);
    }

    // 尝试和目标主机建立连接，连接成功会返回0 ，失败返回 -1
    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        printf("connect failed!\r\n");
        lwip_close(sockfd);
    }
    printf("connect to server %s success!\r\n", host);

    // 建立连接成功之后，这个TCP socket描述符 —— sockfd 就具有了 “连接状态”，发送、接收 对端都是 connect 参数指定的目标主机和端口
    ssize_t retval = send(sockfd, g_request, sizeof(g_request), 0);
    if (retval < 0)
    {
        printf("send g_request failed!\r\n");
        lwip_close(sockfd);
    }
    printf("send g_request{%s} %ld to server done!\r\n", g_request, retval);

    retval = recv(sockfd, &g_response, sizeof(g_response), 0);
    if (retval <= 0)
    {
        printf("send g_response from server failed or done, %ld!\r\n", retval);
        lwip_close(sockfd);
    }
    g_response[retval] = '\0';
    printf("recv g_response{%s} %ld from server done!\r\n", g_response, retval);
    lwip_close(sockfd);
}

void NetDemoTest(unsigned short port, const char *host)
{
    (void)host;
    printf("TcpClientTest start\r\n");
    printf("I will connect to %s\r\n", host);
    TcpClientTest(host, port);
    printf("TcpClientTest done!\r\n");
}
