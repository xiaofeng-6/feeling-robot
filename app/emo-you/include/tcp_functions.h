#ifndef TCP_FUNCTIONS_H
#define TCP_FUNCTIONS_H

// TCP 服务器接口
extern void TcpServerTest(unsigned short port);
extern int  TcpServerInit(unsigned short port);
extern void TcpServerLoop(void);
extern void TcpServerClose(void);

// 数据处理回调，由应用层实现
extern void ProcessReceivedData(const char *data, int len);

#endif // TCP_FUNCTIONS_H 