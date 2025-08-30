
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
#include "cJSON.h"

// WiFi 参数
#define SSID "HUAWEI"
#define PSK  "12345678"

// 本地监听端口
#define LISTEN_PORT 8889

// 全局变量：保存线程句柄
static osThreadId_t g_lvglThreadId = NULL;
// 全局变量：保存当前表情类型
static char g_current_emotion[16] = "HAPPY";

// 温湿度数据结构 (最节省空间版本)
typedef struct {
    int8_t temperature;     // 温度，范围 -128 到 127°C (1字节)
    uint8_t humidity;       // 湿度，范围 0 到 255% (1字节)
} TempHumidityData;

// 全局变量：保存温湿度数据
static TempHumidityData g_temp_humidity_data = {26, 65};  // 默认值
static int g_has_temp_humidity_data = 0;  // 标记是否有有效的温湿度数据

// 前向声明
void CreateLvglThread(void);
void DestroyLvglThread(void);
void wenshidu_with_data(TempHumidityData *temp_humidity_data);

static void PrintCurThreadStackInfo(const char *tag)//计算tag线程堆栈使用情况
{
    osThreadId_t id = osThreadGetId();
    uint32_t total = osThreadGetStackSize(id);
    uint32_t free  = osThreadGetStackSpace(id);
    osal_printk("[%s] stack used:%lu/%lu free:%lu\r\n", tag, total-free, total, free);
}
#endif

// --------------- 数据处理回调 ---------------

// JSON解析函数
static int ParseTempHumidityJson(const char *json_string, TempHumidityData *temp_humidity_data)
{
    // 去除前导空白字符
    const char *trimmed_json = json_string;
    while (*trimmed_json == ' ' || *trimmed_json == '\t' || *trimmed_json == '\r' || *trimmed_json == '\n') {
        trimmed_json++;
    }
    
    // 添加调试信息：打印要解析的JSON字符串
    osal_printk("开始解析温湿度JSON: [%s]\r\n", trimmed_json);
    osal_printk("JSON字符串长度: %d\r\n", strlen(trimmed_json));
    
    cJSON *json = cJSON_Parse(trimmed_json);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            osal_printk("JSON解析失败，错误位置: %s\r\n", error_ptr);
        } else {
            osal_printk("JSON解析失败，未知错误\r\n");
        }
        return -1;
    }

    // 解析command字段
    cJSON *command = cJSON_GetObjectItem(json, "command");
    if (!cJSON_IsString(command) || strcmp(command->valuestring, "WENSHIDU") != 0) {
        osal_printk("不是WENSHIDU命令\r\n");
        cJSON_Delete(json);
        return -1;
    }

    // 解析data对象
    cJSON *data = cJSON_GetObjectItem(json, "data");
    if (!cJSON_IsObject(data)) {
        osal_printk("data字段不是对象\r\n");
        cJSON_Delete(json);
        return -1;
    }

    // 解析温度和湿度
    cJSON *temperature = cJSON_GetObjectItem(data, "temperature");
    cJSON *humidity = cJSON_GetObjectItem(data, "humidity");

    if (!cJSON_IsNumber(temperature) || !cJSON_IsNumber(humidity)) {
        osal_printk("温度或湿度字段不是数字\r\n");
        cJSON_Delete(json);
        return -1;
    }

    temp_humidity_data->temperature = (int8_t)temperature->valuedouble;
    temp_humidity_data->humidity = (uint8_t)humidity->valuedouble;

    cJSON_Delete(json);
    osal_printk("温湿度JSON解析成功: 温度=%d, 湿度=%u\r\n", 
                temp_humidity_data->temperature, temp_humidity_data->humidity);
    return 0;
}

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
    char buffer[1024] = {0};  // 增加缓冲区大小以容纳JSON数据
    int copy_len = len < 1023 ? len : 1023;
    memcpy(buffer, data, copy_len);
    buffer[copy_len] = '\0';
    
    osal_printk("接收到数据: %s (长度:%d, 复制长度:%d)\r\n", buffer, len, copy_len);
    
    // 检查数据是否被截断
    if (len >= 1023) {
        osal_printk("警告：数据可能被截断！原始长度:%d, 缓冲区大小:1023\r\n", len);
    }
    
    // 检查字符串结尾
    osal_printk("数据末尾字符: 0x%02X 0x%02X 0x%02X\r\n", 
                copy_len > 2 ? (unsigned char)buffer[copy_len-3] : 0,
                copy_len > 1 ? (unsigned char)buffer[copy_len-2] : 0,
                copy_len > 0 ? (unsigned char)buffer[copy_len-1] : 0);
#endif

    // 2. 先销毁现有线程（如果存在）
    DestroyLvglThread();
    
    // 3. 基本JSON格式检查（去除前后空白字符）
    // 去除前导空白字符
    char *trimmed_start = buffer;
    while (*trimmed_start == ' ' || *trimmed_start == '\t' || *trimmed_start == '\r' || *trimmed_start == '\n') {
        trimmed_start++;
    }
    
    // 去除尾部空白字符
    int trimmed_len = strlen(trimmed_start);
    while (trimmed_len > 0 && (trimmed_start[trimmed_len-1] == ' ' || trimmed_start[trimmed_len-1] == '\t' || 
                               trimmed_start[trimmed_len-1] == '\r' || trimmed_start[trimmed_len-1] == '\n')) {
        trimmed_len--;
    }
    
    int is_json_like = 0;
    if (trimmed_len > 0 && trimmed_start[0] == '{' && trimmed_start[trimmed_len-1] == '}') {
        is_json_like = 1;
        osal_printk("数据看起来像JSON格式（以{}包围）\r\n");
    } else {
        osal_printk("数据不是JSON格式（不是以{}包围）\r\n");
    }
    
    // 检查JSON中的关键字段
    if (is_json_like && strstr(buffer, "command") != NULL && strstr(buffer, "WENSHIDU") != NULL) {
        osal_printk("发现WENSHIDU命令字段\r\n");
    }
    
    // 4. 尝试解析JSON数据（温湿度数据）
    TempHumidityData temp_humidity_data;
    if (ParseTempHumidityJson(buffer, &temp_humidity_data) == 0) {
        osal_printk("解析到温湿度JSON数据\r\n");
        
        // 保存温湿度数据到全局变量
        g_temp_humidity_data = temp_humidity_data;
        g_has_temp_humidity_data = 1;
        strcpy(g_current_emotion, "WENSHIDU");
        
        // 创建线程
        CreateLvglThread();
        
        DataProcessCallback("WENSHIDU");
        osal_printk("ProcessReceivedData done (WENSHIDU)\r\n");
        return;
    }
    
    // 5. 如果不是JSON，按原来的方式处理简单字符串命令
    if (strcmp(buffer, "HAPPY") == 0) {
        osal_printk("数据匹配: HAPPY\r\n");
        strcpy(g_current_emotion, "HAPPY");
        CreateLvglThread();
    } else if (strcmp(buffer, "CRY") == 0) {
        osal_printk("数据匹配: CRY\r\n");
        strcpy(g_current_emotion, "CRY");
        CreateLvglThread();
    } else if (strcmp(buffer, "WORRY") == 0) {
        osal_printk("数据匹配: WORRY\r\n");
        strcpy(g_current_emotion, "WORRY");
        CreateLvglThread();
    } else if (strcmp(buffer, "SHY") == 0) {
        osal_printk("数据匹配: SHY\r\n");
        strcpy(g_current_emotion, "SHY");
        CreateLvglThread();
    } else if (strcmp(buffer, "SURPRISED") == 0) {
        osal_printk("数据匹配: SURPRISED\r\n");
        strcpy(g_current_emotion, "SURPRISED");
        CreateLvglThread();
    } else if (strcmp(buffer, "WENSHIDU") == 0) {
        osal_printk("数据匹配: WENSHIDU (简单命令)\r\n");
        strcpy(g_current_emotion, "WENSHIDU");
        CreateLvglThread();
    } else if(strcmp(buffer, "OTHER") == 0){
        osal_printk("数据匹配: OTHER\r\n");
        strcpy(g_current_emotion, "OTHER");
        CreateLvglThread();
    } else if(strcmp(buffer, "XIAOHUI") == 0){
        osal_printk("数据匹配: XIAOHUI\r\n");
        strcpy(g_current_emotion, "XIAOHUI");
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
    }else if(strcmp(g_current_emotion, "WORRY") == 0){
        worry(NULL);
    }else if(strcmp(g_current_emotion, "SHY") == 0){
        shy(NULL);
    }else if(strcmp(g_current_emotion, "SURPRISED") == 0){
        surprised(NULL);
    }else if(strcmp(g_current_emotion, "OTHER") == 0){
        neutral(NULL);
    }else if(strcmp(g_current_emotion, "WENSHIDU") == 0){
        if (g_has_temp_humidity_data) {
            wenshidu_with_data(&g_temp_humidity_data);
        } else {
            wenshidu();  // 使用默认数据
        }
    }else if(strcmp(g_current_emotion, "XIAOHUI") == 0){
        xiaohui(NULL);
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
void CreateLvglThread(void)   //3执行，创建LVGL线程，进入lvglTask函数
{
    // 打印当前heap余量，打印当前系统堆（heap）内存的使用情况
    extern UINT32 LOS_MemTotalUsedGet(VOID *pool);
    extern UINT32 LOS_MemPoolSizeGet(const VOID *pool);
    UINT32 total = LOS_MemPoolSizeGet(OS_SYS_MEM_ADDR);
    UINT32 used = LOS_MemTotalUsedGet(OS_SYS_MEM_ADDR);
    osal_printk("heap before create: total=%u, used=%u, free=%u B\r\n", total, used, total-used);
    
    osThreadAttr_t attr = { 
        .name = "lvglThread", 
        .stack_size = 32*1024,  // 32KB栈大小
        .priority = osPriorityNormal 
    };
    
    
    g_lvglThreadId = osThreadNew(lvglTask, NULL, &attr);
    if(!g_lvglThreadId) {
        osal_printk("Create lvglThread failed\r\n");
        used = LOS_MemTotalUsedGet(OS_SYS_MEM_ADDR);
        osal_printk("heap after failed: free=%u B\r\n", total-used);
    } else {
        osal_printk("lvglThread created successfully\r\n");
        used = LOS_MemTotalUsedGet(OS_SYS_MEM_ADDR);
        osal_printk("heap after success: free=%u B\r\n", total-used);
    }
}

// 销毁LVGL线程的函数
void DestroyLvglThread(void)
{
    if (g_lvglThreadId != NULL) {
        osal_printk("开始销毁 lvglThread...\r\n");
        
        // 终止线程
        osStatus_t status = osThreadTerminate(g_lvglThreadId);
        if (status == osOK) {
            osal_printk("lvglThread 销毁成功\r\n");
            g_lvglThreadId = NULL;
            
            // 打印heap使用情况
            extern UINT32 LOS_MemTotalUsedGet(VOID *pool);
            extern UINT32 LOS_MemPoolSizeGet(const VOID *pool);
            UINT32 total = LOS_MemPoolSizeGet(OS_SYS_MEM_ADDR);
            UINT32 used = LOS_MemTotalUsedGet(OS_SYS_MEM_ADDR);
            osal_printk("heap after destroy: total=%u, used=%u, free=%u B\r\n", total, used, total-used);
        } else {
            osal_printk("lvglThread 销毁失败, status=%d\r\n", status);
        }
    } else {
        osal_printk("lvglThread 不存在或已被销毁\r\n");
    }
}
#endif


static void TCPDemoTask(void *arg)//2执行，TCP任务线程，这里面连接热点，初始化TCP服务，创建LVGL线程createLvglThread
{
#if 1//无关紧要的连接热点还有开始界面也就是happy    
    (void)arg;

    PrintCurThreadStackInfo("TCPDemoTask-start");

    if (ConnectToHotspot(SSID,PSK) != 0) {
        osal_printk("Connect AP failed\r\n");
        return;
    }
    osal_printk("conn AP success\r\n");
    osDelay(100);

    if (TcpServerInit(LISTEN_PORT)!=0) {
        osal_printk("TCP srv init fail\r\n");
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

static void TCPDemoEntry(void)   //1执行 创建TCP任务线程
{
    osThreadAttr_t attr = { .name="TCPDemoTask", .stack_size = 3*1024,  // 增加到3KB栈大小
    .priority = osPriorityNormal };
    if(!osThreadNew(TCPDemoTask,NULL,&attr)) {
        osal_printk("Create TCPDemoTask failed\r\n");
    }
}

APP_FEATURE_INIT(TCPDemoEntry);
