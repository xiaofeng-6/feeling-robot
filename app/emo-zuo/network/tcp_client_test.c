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
//初始化检查服务器连接。周期性接收新连接。处理已连接客户端的数据，recv函数接收数据。
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
        static char receive_buffer[2048] = {0};  // 增加缓冲区大小
        static int  buffer_pos = 0;
        char        temp_buf[512] = {0};
        
        int ret = recv(g_client_fd, temp_buf, sizeof(temp_buf) - 1, MSG_DONTWAIT);
        if (ret > 0) {
            osal_printk("TCP recv: %d bytes\r\n", ret);
            
            // 检查缓冲区是否有足够空间
            if (buffer_pos + ret >= sizeof(receive_buffer) - 1) {
                osal_printk("警告：接收缓冲区将溢出，清空缓冲区\r\n");
                buffer_pos = 0;
                memset(receive_buffer, 0, sizeof(receive_buffer));
            }
            
            // 将新数据追加到缓冲区
            memcpy(receive_buffer + buffer_pos, temp_buf, ret);
            buffer_pos += ret;
            receive_buffer[buffer_pos] = '\0';
            
            // 检查是否收到完整的JSON数据（简单检查：以}结尾，忽略空白字符）
            // 去除前导空白字符检查
            char *trimmed_start = receive_buffer;
            while (*trimmed_start == ' ' || *trimmed_start == '\t' || *trimmed_start == '\r' || *trimmed_start == '\n') {
                trimmed_start++;
            }
            // 去除尾部空白字符检查
            int check_pos = buffer_pos - 1;
            while (check_pos >= 0 && (receive_buffer[check_pos] == ' ' || receive_buffer[check_pos] == '\t' || 
                                      receive_buffer[check_pos] == '\r' || receive_buffer[check_pos] == '\n')) {
                check_pos--;
            }
            
            if (trimmed_start < receive_buffer + buffer_pos && *trimmed_start == '{' && 
                check_pos >= 0 && receive_buffer[check_pos] == '}') {
                osal_printk("接收到完整JSON数据，长度: %d\r\n", buffer_pos);
                ProcessReceivedData(receive_buffer, buffer_pos);
                
                // 清空缓冲区，准备下次接收
                buffer_pos = 0;
                memset(receive_buffer, 0, sizeof(receive_buffer));
                
                const char *ack = "ACK";
                send(g_client_fd, ack, strlen(ack), 0);
                osal_printk("TcpServerLoop handled data OK\r\n");
            } else {
                // 如果不是以}结尾且缓冲区中有{开头，说明可能是分包数据，等待更多数据
                if (trimmed_start < receive_buffer + buffer_pos && *trimmed_start == '{') {
                    osal_printk("等待更多数据... 当前缓冲区长度: %d\r\n", buffer_pos);
                } else {
                    // 如果不是JSON格式，直接处理
                    osal_printk("接收到非JSON数据: %s\r\n", receive_buffer);
                    ProcessReceivedData(receive_buffer, buffer_pos);
                    
                    // 清空缓冲区
                    buffer_pos = 0;
                    memset(receive_buffer, 0, sizeof(receive_buffer));
                    
                    const char *ack = "ACK";
                    send(g_client_fd, ack, strlen(ack), 0);
                    osal_printk("TcpServerLoop handled data OK\r\n");
                }
            }
        } else if (ret == 0) {
            // 断开连接，清空缓冲区
            osal_printk("客户端断开连接\r\n");
            buffer_pos = 0;
            memset(receive_buffer, 0, sizeof(receive_buffer));
            closesocket(g_client_fd);
            g_client_fd = -1;
        } else if (ret < 0) {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                osal_printk("recv错误，断开连接\r\n");
                buffer_pos = 0;
                memset(receive_buffer, 0, sizeof(receive_buffer));
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
