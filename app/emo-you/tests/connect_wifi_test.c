
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

// 天气数据结构
typedef struct {
    int max_temp_current;
    int min_temp_current;
    int max_temp_periods[6];
    int min_temp_periods[6];
} WeatherData;

// 全局变量：保存天气数据
static WeatherData g_weather_data = {0};
static int g_has_weather_data = 0;  // 标记是否有有效的天气数据

// 前向声明
void CreateLvglThread(void);
void DestroyLvglThread(void);
void tianqi_with_data(WeatherData *weather_data);

static void PrintCurThreadStackInfo(const char *tag)
{
    osThreadId_t id = osThreadGetId();
    uint32_t total = osThreadGetStackSize(id);
    uint32_t free  = osThreadGetStackSpace(id);
    osal_printk("[%s] stack used:%lu/%lu free:%lu\r\n", tag, total-free, total, free);
}
#endif

// --------------- 数据处理回调 ---------------

// JSON解析函数
static int ParseWeatherJson(const char *json_string, WeatherData *weather_data)
{
    // 添加调试信息：打印要解析的JSON字符串
    osal_printk("开始解析JSON: [%s]\r\n", json_string);
    osal_printk("JSON字符串长度: %d\r\n", strlen(json_string));
    
    cJSON *json = cJSON_Parse(json_string);
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
    if (!cJSON_IsString(command) || strcmp(command->valuestring, "TIANQI") != 0) {
        osal_printk("不是TIANQI命令\r\n");
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

    // 解析当前温度
    cJSON *max_temp_current = cJSON_GetObjectItem(data, "max_temp_current");
    cJSON *min_temp_current = cJSON_GetObjectItem(data, "min_temp_current");

    if (!cJSON_IsNumber(max_temp_current) || !cJSON_IsNumber(min_temp_current)) {
        osal_printk("温度字段不是数字\r\n");
        cJSON_Delete(json);
        return -1;
    }

    weather_data->max_temp_current = (int)max_temp_current->valuedouble;
    weather_data->min_temp_current = (int)min_temp_current->valuedouble;

    // 解析温度数组
    cJSON *max_temp_periods = cJSON_GetObjectItem(data, "max_temp_periods");
    cJSON *min_temp_periods = cJSON_GetObjectItem(data, "min_temp_periods");

    if (!cJSON_IsArray(max_temp_periods) || !cJSON_IsArray(min_temp_periods)) {
        osal_printk("温度数组字段不是数组\r\n");
        cJSON_Delete(json);
        return -1;
    }

    // 检查数组长度
    int max_array_size = cJSON_GetArraySize(max_temp_periods);
    int min_array_size = cJSON_GetArraySize(min_temp_periods);
    
    if (max_array_size != 6 || min_array_size != 6) {
        osal_printk("温度数组长度不正确，期望6个元素\r\n");
        cJSON_Delete(json);
        return -1;
    }

    // 解析数组数据
    for (int i = 0; i < 6; i++) {
        cJSON *max_item = cJSON_GetArrayItem(max_temp_periods, i);
        cJSON *min_item = cJSON_GetArrayItem(min_temp_periods, i);
        
        if (!cJSON_IsNumber(max_item) || !cJSON_IsNumber(min_item)) {
            osal_printk("数组第%d个元素不是数字\r\n", i);
            cJSON_Delete(json);
            return -1;
        }
        
        weather_data->max_temp_periods[i] = (int)max_item->valuedouble;
        weather_data->min_temp_periods[i] = (int)min_item->valuedouble;
    }

    cJSON_Delete(json);
    osal_printk("JSON解析成功\r\n");
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
    
    // 3. 基本JSON格式检查
    int is_json_like = 0;
    if (buffer[0] == '{' && buffer[strlen(buffer)-1] == '}') {
        is_json_like = 1;
        osal_printk("数据看起来像JSON格式（以{}包围）\r\n");
    } else {
        osal_printk("数据不是JSON格式（不是以{}包围）\r\n");
    }
    
    // 检查JSON中的关键字段
    if (is_json_like && strstr(buffer, "command") != NULL && strstr(buffer, "TIANQI") != NULL) {
        osal_printk("发现TIANQI命令字段\r\n");
    }
    
    // 4. 尝试解析JSON数据（天气数据）
    WeatherData weather_data;
    if (ParseWeatherJson(buffer, &weather_data) == 0) {
        osal_printk("解析到天气JSON数据\r\n");
        
        // 保存天气数据到全局变量
        g_weather_data = weather_data;
        g_has_weather_data = 1;
        strcpy(g_current_emotion, "TIANQI");
        
        // 创建线程
        CreateLvglThread();
        
        DataProcessCallback("TIANQI");
        osal_printk("ProcessReceivedData done (TIANQI)\r\n");
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
    } else if (strcmp(buffer, "TIANQI") == 0) {
        osal_printk("数据匹配: TIANQI (简单命令)\r\n");
        strcpy(g_current_emotion, "TIANQI");
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
    // 6. 执行回调函数
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
    }else if(strcmp(g_current_emotion, "TIANQI") == 0){
        if (g_has_weather_data) {
            tianqi_with_data(&g_weather_data);
        } else {
            tianqi();  // 使用默认数据
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
