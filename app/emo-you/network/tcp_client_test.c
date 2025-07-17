/*
 Copyright (C) 2024 HiHope Open Source Organization .
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include <stdio.h>          
#include <unistd.h>        
#include <string.h>     
#include <fcntl.h>
#include <errno.h>
#include "lwip/sockets.h"  
#include "osal_debug.h"
#include "watchdog.h"
#include "tcp_functions.h"

// 全局变量
static int  g_listen_fd          = -1;
static int  g_client_fd          = -1;
static int  g_server_initialized = 0;

/* TCP 服务器初始化 */
int TcpServerInit(unsigned short port)
{
    struct sockaddr_in server_addr = {0};
    int opt = 1;

    osal_printk("Starting TCP Server init on port %d\r\n", port);

    g_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (g_listen_fd < 0) {
        osal_printk("create listen socket failed!\r\n");
        // OLED 输出已移除
        return -1;
    }

    setsockopt(g_listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port        = htons(port);

    if (bind(g_listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        osal_printk("bind failed!\r\n");
        closesocket(g_listen_fd);
        g_listen_fd = -1;
        return -1;
    }

    if (listen(g_listen_fd, 5) < 0) {
        osal_printk("listen failed!\r\n");
        closesocket(g_listen_fd);
        g_listen_fd = -1;
        return -1;
    }

    g_server_initialized = 1;
    osal_printk("TCP server ready on %d\r\n", port);
    osal_printk("TcpServerInit success\r\n");
    return 0;
}

/* 主循环：非阻塞 accept/recv 处理 */
void TcpServerLoop(void)
{
    if (!g_server_initialized) {
        return;
    }

    static int check_cnt = 0;
    struct sockaddr_in client_addr;
    socklen_t          client_len = sizeof(client_addr);

    // 周期性检查是否有新连接
    if (g_client_fd < 0 && (++check_cnt >= 100)) {
        check_cnt = 0;
        g_client_fd = accept(g_listen_fd, (struct sockaddr *)&client_addr, &client_len);
        if (g_client_fd >= 0) {
            osal_printk("Client %s:%d connected\r\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            // OLED 输出已移除
        }
    }

    if (g_client_fd >= 0) {
        char buf[128] = {0};
        int  ret      = recv(g_client_fd, buf, sizeof(buf) - 1, MSG_DONTWAIT);
        if (ret > 0) {
            ProcessReceivedData(buf, ret);
            const char *ack = "ACK";
            send(g_client_fd, ack, strlen(ack), 0);
            osal_printk("TcpServerLoop handled data OK\r\n");
        } else if (ret == 0) {
            // 断开
            closesocket(g_client_fd);
            g_client_fd = -1;
        } else if (ret < 0) {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                closesocket(g_client_fd);
                g_client_fd = -1;
            }
        }
    }
}

void TcpServerClose(void)
{
    if (g_client_fd >= 0) {
        closesocket(g_client_fd);
        g_client_fd = -1;
    }
    if (g_listen_fd >= 0) {
        closesocket(g_listen_fd);
        g_listen_fd = -1;
    }
    g_server_initialized = 0;
    osal_printk("TcpServerClose done\r\n");
}
