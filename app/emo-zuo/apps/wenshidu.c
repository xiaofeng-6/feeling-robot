#if 1
#include "lvgl.h"
#include "osal_debug.h"

// 温湿度数据结构 (最节省空间版本)
typedef struct {
    int8_t temperature;     // 温度，范围 -128 到 127°C (1字节)
    uint8_t humidity;       // 湿度，范围 0 到 255% (1字节)
} TempHumidityData;

// Global variables for temperature and humidity data
static int8_t temperature = 26;    // Temperature (C)
static uint8_t humidity = 65;      // Humidity (%)

// Temperature and humidity dashboard function
void wenshidu(void)
{
#if 1
    lv_init();
    osal_printk("wenshidu lv_init done\r\n");
    lv_port_disp_init();
    osal_printk("wenshidu lv_port_disp_init done\r\n");

    lv_obj_clean(lv_scr_act());  //清除屏幕
#endif

    // Create main screen object// 创建主屏幕对象
    lv_obj_t * scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0xffffff), 0);
    
    // Title label// 标题标签
    lv_obj_t * title_label = lv_label_create(scr);
    lv_label_set_text(title_label, "当前温度和湿度");
    lv_obj_set_style_text_color(title_label, lv_color_hex(0x000000), 0);
    lv_obj_set_style_text_font(title_label, &lv_font_biaozhunheiti_16, 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 10);
    
    // Temperature circular progress bar// 温度圆形进度条
    lv_obj_t * temp_arc = lv_arc_create(scr);
    lv_obj_set_size(temp_arc, 120, 120);
    lv_obj_set_pos(temp_arc, 30, 60);
    lv_arc_set_rotation(temp_arc, 270);
    lv_arc_set_bg_angles(temp_arc, 0, 360);
    lv_arc_set_value(temp_arc, temperature);
    lv_obj_set_style_arc_color(temp_arc, lv_color_hex(0xff6b35), LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(temp_arc, lv_color_hex(0xe0e0e0), LV_PART_MAIN);
    lv_obj_set_style_arc_width(temp_arc, 8, LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(temp_arc, 8, LV_PART_MAIN);
    lv_obj_remove_style(temp_arc, NULL, LV_PART_KNOB);
    lv_obj_add_flag(temp_arc, LV_OBJ_FLAG_ADV_HITTEST);
    
    // Temperature value display// 温度值显示
    lv_obj_t * temp_label = lv_label_create(scr);
    lv_label_set_text_fmt(temp_label, "%d度", temperature);
    lv_obj_set_style_text_color(temp_label, lv_color_hex(0xff6b35), 0);
    lv_obj_set_style_text_font(temp_label, &lv_font_biaozhunheiti_16, 0);
    lv_obj_set_pos(temp_label, 75, 110);
    
    // Temperature label// 温度标签
    lv_obj_t * temp_title = lv_label_create(scr);
    lv_label_set_text(temp_title, "温度");
    lv_obj_set_style_text_color(temp_title, lv_color_hex(0x555555), 0);
    lv_obj_set_style_text_font(temp_title, &lv_font_biaozhunheiti_16, 0);
    lv_obj_set_pos(temp_title, 67, 190);
    
    // Humidity circular progress bar// 湿度圆形进度条
    lv_obj_t * hum_arc = lv_arc_create(scr);
    lv_obj_set_size(hum_arc, 120, 120);
    lv_obj_set_pos(hum_arc, 170, 60);
    lv_arc_set_rotation(hum_arc, 270);
    lv_arc_set_bg_angles(hum_arc, 0, 360);
    lv_arc_set_value(hum_arc, humidity);
    lv_obj_set_style_arc_color(hum_arc, lv_color_hex(0x00bcd4), LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(hum_arc, lv_color_hex(0xe0e0e0), LV_PART_MAIN);
    lv_obj_set_style_arc_width(hum_arc, 8, LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(hum_arc, 8, LV_PART_MAIN);
    lv_obj_remove_style(hum_arc, NULL, LV_PART_KNOB);
    lv_obj_add_flag(hum_arc, LV_OBJ_FLAG_ADV_HITTEST);
    
    // Humidity value display// 湿度值显示
    lv_obj_t * hum_label = lv_label_create(scr);
    lv_label_set_text_fmt(hum_label, "%u%%", humidity);
    lv_obj_set_style_text_color(hum_label, lv_color_hex(0x00bcd4), 0);
    lv_obj_set_style_text_font(hum_label, &lv_font_biaozhunheiti_16, 0);
    lv_obj_set_pos(hum_label, 217, 110);
    
    // Humidity label// 湿度标签
    lv_obj_t * hum_title = lv_label_create(scr);
    //lv_label_set_text(hum_title, "Humidity");
    lv_label_set_text(hum_title, "湿度");
    lv_obj_set_style_text_color(hum_title, lv_color_hex(0x555555), 0);
    lv_obj_set_style_text_font(hum_title, &lv_font_biaozhunheiti_16, 0);
    lv_obj_set_pos(hum_title, 220, 190);
    
    // Status indicator// 状态指示器
    lv_obj_t * status_label = lv_label_create(scr);
    if (temperature >= 20 && temperature <= 30 && humidity >= 40 && humidity <= 70) {
        lv_label_set_text(status_label, "舒适");
        lv_obj_set_style_text_color(status_label, lv_color_hex(0x4caf50), 0);
    } else {
        lv_label_set_text(status_label, "警告");
        lv_obj_set_style_text_color(status_label, lv_color_hex(0xff9800), 0);
    }
    lv_obj_set_style_text_font(status_label, &lv_font_biaozhunheiti_16, 0);
    lv_obj_align(status_label, LV_ALIGN_BOTTOM_MID, 0, -10);
}

// 带数据参数的温湿度函数
void wenshidu_with_data(TempHumidityData *temp_humidity_data)
{
    if (temp_humidity_data == NULL) {
        osal_printk("temp_humidity_data为空，使用默认温湿度显示\r\n");
        wenshidu();  // 调用原始函数
        return;
    }

    osal_printk("wenshidu_with_data: 温度=%d, 湿度=%u\r\n", 
                temp_humidity_data->temperature, temp_humidity_data->humidity);

#if 1
    lv_init();
    osal_printk("wenshidu lv_init done\r\n");
    lv_port_disp_init();
    osal_printk("wenshidu lv_port_disp_init done\r\n");

    lv_obj_clean(lv_scr_act());  //清除屏幕
#endif

    // Create main screen object// 创建主屏幕对象
    lv_obj_t * scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, lv_color_hex(0xffffff), 0);
    
    // Title label// 标题标签
    lv_obj_t * title_label = lv_label_create(scr);
    lv_label_set_text(title_label, "当前温度和湿度");
    lv_obj_set_style_text_color(title_label, lv_color_hex(0x000000), 0);
    lv_obj_set_style_text_font(title_label, &lv_font_biaozhunheiti_16, 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 10);
    
    // Temperature circular progress bar// 温度圆形进度条
    lv_obj_t * temp_arc = lv_arc_create(scr);
    lv_obj_set_size(temp_arc, 120, 120);
    lv_obj_set_pos(temp_arc, 30, 60);
    lv_arc_set_rotation(temp_arc, 270);
    lv_arc_set_bg_angles(temp_arc, 0, 360);
    lv_arc_set_value(temp_arc, temp_humidity_data->temperature);  // 使用动态数据
    lv_obj_set_style_arc_color(temp_arc, lv_color_hex(0xff6b35), LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(temp_arc, lv_color_hex(0xe0e0e0), LV_PART_MAIN);
    lv_obj_set_style_arc_width(temp_arc, 8, LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(temp_arc, 8, LV_PART_MAIN);
    lv_obj_remove_style(temp_arc, NULL, LV_PART_KNOB);
    lv_obj_add_flag(temp_arc, LV_OBJ_FLAG_ADV_HITTEST);
    
    // Temperature value display// 温度值显示
    lv_obj_t * temp_label = lv_label_create(scr);
    char temp_text[16];
    snprintf(temp_text, sizeof(temp_text), "%d度", temp_humidity_data->temperature);
    lv_label_set_text(temp_label, temp_text);
    lv_obj_set_style_text_color(temp_label, lv_color_hex(0xff6b35), 0);
    lv_obj_set_style_text_font(temp_label, &lv_font_biaozhunheiti_16, 0);
    lv_obj_set_pos(temp_label, 75, 110);
    
    // Temperature label// 温度标签
    lv_obj_t * temp_title = lv_label_create(scr);
    lv_label_set_text(temp_title, "温度");
    lv_obj_set_style_text_color(temp_title, lv_color_hex(0x555555), 0);
    lv_obj_set_style_text_font(temp_title, &lv_font_biaozhunheiti_16, 0);
    lv_obj_set_pos(temp_title, 67, 190);
    
    // Humidity circular progress bar// 湿度圆形进度条
    lv_obj_t * hum_arc = lv_arc_create(scr);
    lv_obj_set_size(hum_arc, 120, 120);
    lv_obj_set_pos(hum_arc, 170, 60);
    lv_arc_set_rotation(hum_arc, 270);
    lv_arc_set_bg_angles(hum_arc, 0, 360);
    lv_arc_set_value(hum_arc, temp_humidity_data->humidity);  // 使用动态数据
    lv_obj_set_style_arc_color(hum_arc, lv_color_hex(0x00bcd4), LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(hum_arc, lv_color_hex(0xe0e0e0), LV_PART_MAIN);
    lv_obj_set_style_arc_width(hum_arc, 8, LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(hum_arc, 8, LV_PART_MAIN);
    lv_obj_remove_style(hum_arc, NULL, LV_PART_KNOB);
    lv_obj_add_flag(hum_arc, LV_OBJ_FLAG_ADV_HITTEST);
    
    // Humidity value display// 湿度值显示
    lv_obj_t * hum_label = lv_label_create(scr);
    char hum_text[16];
    snprintf(hum_text, sizeof(hum_text), "%u%%", temp_humidity_data->humidity);
    lv_label_set_text(hum_label, hum_text);
    lv_obj_set_style_text_color(hum_label, lv_color_hex(0x00bcd4), 0);
    lv_obj_set_style_text_font(hum_label, &lv_font_biaozhunheiti_16, 0);
    lv_obj_set_pos(hum_label, 217, 110);
    
    // Humidity label// 湿度标签
    lv_obj_t * hum_title = lv_label_create(scr);
    lv_label_set_text(hum_title, "湿度");
    lv_obj_set_style_text_color(hum_title, lv_color_hex(0x555555), 0);
    lv_obj_set_style_text_font(hum_title, &lv_font_biaozhunheiti_16, 0);
    lv_obj_set_pos(hum_title, 220, 190);
    
    // Status indicator// 状态指示器
    lv_obj_t * status_label = lv_label_create(scr);
    if (temp_humidity_data->temperature >= 20 && temp_humidity_data->temperature <= 30 && 
        temp_humidity_data->humidity >= 40 && temp_humidity_data->humidity <= 70) {
        lv_label_set_text(status_label, "舒适");
        lv_obj_set_style_text_color(status_label, lv_color_hex(0x4caf50), 0);
    } else {
        lv_label_set_text(status_label, "警告");
        lv_obj_set_style_text_color(status_label, lv_color_hex(0xff9800), 0);
    }
    lv_obj_set_style_text_font(status_label, &lv_font_biaozhunheiti_16, 0);
    lv_obj_align(status_label, LV_ALIGN_BOTTOM_MID, 0, -10);
}

#endif