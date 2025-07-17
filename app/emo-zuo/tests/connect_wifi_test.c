
#if 1//包含include 
#include <stdio.h>    
#include <unistd.h>   
#include <string.h>    
#include "ohos_init.h" 
#include "cmsis_os2.h" 
#include "wifi_connecter.h" 

#include "watchdog.h"

#include "tcp_functions.h"
#include "lvgl.h"
#include "lv_port_disp.h"

// WiFi 参数
#define SSID "HUAWEI"
#define PSK  "12345678"

// 本地监听端口
#define LISTEN_PORT 8889

// 全局变量：保存线程句柄
static osThreadId_t g_lvglThreadId = NULL;
// 全局变量：保存当前表情类型
static char g_current_emotion[16] = "HAPPY";

// 前向声明
void CreateLvglThread(void);
void DestroyLvglThread(void);

static void PrintCurThreadStackInfo(const char *tag)
{
    osThreadId_t id = osThreadGetId();
    uint32_t total = osThreadGetStackSize(id);
    uint32_t free  = osThreadGetStackSpace(id);
    osal_printk("[%s] stack used:%lu/%lu free:%lu\r\n", tag, total-free, total, free);
}
#endif

// --------------- 数据处理回调 ---------------

// 数据处理回调函数
static void DataProcessCallback(const char *data)//串口打印收到了解析的消息
{
    strcpy(g_current_emotion, data);
    osal_printk("DataProcessCallback: %s\r\n", g_current_emotion);
}

void ProcessReceivedData(const char *data, int len)//打印接收到的完整消息，先销毁画图进程，然后判断数据，解析数据，执行相应的函数，最后执行DataProcessCallback
{
#if 1
    // 1. 打印接收到的完整数据
    char buffer[256] = {0};  // 假设数据不会超过255字节
    int copy_len = len < 255 ? len : 255;
    memcpy(buffer, data, copy_len);
    buffer[copy_len] = '\0';
    
    osal_printk("接收到数据: %s (长度:%d)\r\n", buffer, len);
#endif

    // 2. 先销毁现有线程（如果存在）
    DestroyLvglThread();
    
    // 3. 判断数据类型并创建相应线程
    if (strcmp(buffer, "HAPPY") == 0) {
        osal_printk("数据匹配: HAPPY\r\n");
        strcpy(g_current_emotion, "HAPPY");
        CreateLvglThread();
    } else if (strcmp(buffer, "CRY") == 0) {
        osal_printk("数据匹配: CRY\r\n");
        strcpy(g_current_emotion, "CRY");
        CreateLvglThread();
    } else if (strcmp(buffer, "SAD") == 0) {
        osal_printk("数据匹配: SAD\r\n");
        strcpy(g_current_emotion, "SAD");
        CreateLvglThread();
    } else if (strcmp(buffer, "FEAR") == 0) {
        osal_printk("数据匹配: FEAR\r\n");
        strcpy(g_current_emotion, "FEAR");
        CreateLvglThread();
    } else if (strcmp(buffer, "SURPRISED") == 0) {
        osal_printk("数据匹配: SURPRISED\r\n");
        strcpy(g_current_emotion, "SURPRISED");
        CreateLvglThread();
    } else if (strcmp(buffer, "WENSHIDU") == 0) {
        osal_printk("数据匹配: WENSHIDU\r\n");
        strcpy(g_current_emotion, "WENSHIDU");
        CreateLvglThread();
    } else if(strcmp(buffer, "OTHER") == 0){
        osal_printk("数据匹配: OTHER\r\n");
        strcpy(g_current_emotion, "OTHER");
        CreateLvglThread();
    }
    // 4. 执行回调函数
    DataProcessCallback(buffer);
    osal_printk("ProcessReceivedData done\r\n");
}

//------------------------------------------------


#if 1//包含createlvglthread函数 lvglTask函数，接受到命令执行这两个函数，当命令改变时，在前面销毁进程，到这里创建新的进程执行另一个表情函数。
// LVGL任务函数
static void lvglTask(void *arg)   //画一个
{
    (void)arg;  // 避免未使用参数警告
    
    // 根据全局变量决定显示哪种表情
    if(strcmp(g_current_emotion, "HAPPY") == 0){
        happy(NULL);
    }else if(strcmp(g_current_emotion, "CRY") == 0){
        cry(NULL);
    }else if(strcmp(g_current_emotion, "SAD") == 0){
        sad(NULL);
    }else if(strcmp(g_current_emotion, "FEAR") == 0){
        fear(NULL);
    }else if(strcmp(g_current_emotion, "SURPRISED") == 0){
        surprised(NULL);
    }else if(strcmp(g_current_emotion, "OTHER") == 0){
        neutral(NULL);
    }else if(strcmp(g_current_emotion, "WENSHIDU") == 0){
        wenshidu(NULL);
    }

    // 主循环
    while (1) {
        // 处理LVGL事件和刷新
        lv_timer_handler();
        //osal_printk("lv_timer_handler\r\n");
        // 维护系统
        uapi_watchdog_kick();
        uapi_systick_delay_ms(5);
        lv_tick_inc(5);  // 通知LVGL时间流逝
    }
}
// 创建LVGL线程的函数
void CreateLvglThread(void)   //创建LVGL线程，进入lvglTask函数
{
    // 打印当前heap余量
    extern UINT32 LOS_MemTotalUsedGet(VOID *pool);
    extern UINT32 LOS_MemPoolSizeGet(const VOID *pool);
    UINT32 total = LOS_MemPoolSizeGet(OS_SYS_MEM_ADDR);
    UINT32 used = LOS_MemTotalUsedGet(OS_SYS_MEM_ADDR);
    osal_printk("heap before create: total=%u, used=%u, free=%u B\r\n", total, used, total-used);
    
    osThreadAttr_t attr = { 
        .name = "lvglThread", 
        .stack_size = 32*1024,  // 8KB栈大小
        .priority = osPriorityNormal 
    };
    
    
    g_lvglThreadId = osThreadNew(lvglTask, NULL, &attr);
    if(!g_lvglThreadId) {
        osal_printk("Create lvglThread failed\n");
        used = LOS_MemTotalUsedGet(OS_SYS_MEM_ADDR);
        osal_printk("heap after failed: free=%u B\r\n", total-used);
    } else {
        osal_printk("lvglThread created successfully\n");
        used = LOS_MemTotalUsedGet(OS_SYS_MEM_ADDR);
        osal_printk("heap after success: free=%u B\r\n", total-used);
    }
}

// 销毁LVGL线程的函数
void DestroyLvglThread(void)
{
    if (g_lvglThreadId != NULL) {
        osal_printk("开始销毁 lvglThread...\n");
        
        // 终止线程
        osStatus_t status = osThreadTerminate(g_lvglThreadId);
        if (status == osOK) {
            osal_printk("lvglThread 销毁成功\n");
            g_lvglThreadId = NULL;
            
            // 打印heap使用情况
            extern UINT32 LOS_MemTotalUsedGet(VOID *pool);
            extern UINT32 LOS_MemPoolSizeGet(const VOID *pool);
            UINT32 total = LOS_MemPoolSizeGet(OS_SYS_MEM_ADDR);
            UINT32 used = LOS_MemTotalUsedGet(OS_SYS_MEM_ADDR);
            osal_printk("heap after destroy: total=%u, used=%u, free=%u B\r\n", total, used, total-used);
        } else {
            osal_printk("lvglThread 销毁失败, status=%d\n", status);
        }
    } else {
        osal_printk("lvglThread 不存在或已被销毁\n");
    }
}
#endif


static void TCPDemoTask(void *arg)//TCP任务线程，这里面连接热点，初始化TCP服务，创建LVGL线程createLvglThread
{
#if 1//无关紧要的连接热点还有开始界面也就是happy    
    (void)arg;

    PrintCurThreadStackInfo("TCPDemoTask-start");

    if (ConnectToHotspot(SSID,PSK) != 0) {
        osal_printk("Connect AP failed\n");
        return;
    }
    osal_printk("conn AP success\r\n");
    osDelay(100);

    if (TcpServerInit(LISTEN_PORT)!=0) {
        osal_printk("TCP srv init fail\n");
        return;
    }
    osal_printk("TCP server init success\r\n");

    /* WiFi 和 TCP 服务均已就绪，再启动 LVGL 线程 */
    CreateLvglThread();
#endif
    int loop=0;
    while(1) {
        TcpServerLoop();//在这里进入执行接收数据。
        uapi_watchdog_kick();
        uapi_systick_delay_ms(50);
        if(++loop>=200){ loop=0; PrintCurThreadStackInfo("TCPDemoTask-loop"); }
    }
}

static void TCPDemoEntry(void)   // 创建TCP任务线程
{
    osThreadAttr_t attr = { .name="TCPDemoTask", .stack_size = 3*1024, 
    .priority = osPriorityNormal };
    if(!osThreadNew(TCPDemoTask,NULL,&attr)) {
        osal_printk("Create TCPDemoTask failed\n");
    }
}

APP_FEATURE_INIT(TCPDemoEntry);
