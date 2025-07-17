#if 1
#include "lvgl.h"

// Global variables for temperature and humidity data
static float temperature = 25.5f;  // Temperature (C)
static float humidity = 65.0f;     // Humidity (%)

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
    lv_label_set_text(title_label, "2025年6月29日");
    lv_obj_set_style_text_color(title_label, lv_color_hex(0x000000), 0);
    lv_obj_set_style_text_font(title_label, &lv_font_simsun_14_cjk, 0);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 10);
    
    // Temperature circular progress bar// 温度圆形进度条
    lv_obj_t * temp_arc = lv_arc_create(scr);
    lv_obj_set_size(temp_arc, 120, 120);
    lv_obj_set_pos(temp_arc, 30, 60);
    lv_arc_set_rotation(temp_arc, 270);
    lv_arc_set_bg_angles(temp_arc, 0, 360);
    lv_arc_set_value(temp_arc, (int)temperature);
    lv_obj_set_style_arc_color(temp_arc, lv_color_hex(0xff6b35), LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(temp_arc, lv_color_hex(0xe0e0e0), LV_PART_MAIN);
    lv_obj_set_style_arc_width(temp_arc, 8, LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(temp_arc, 8, LV_PART_MAIN);
    lv_obj_remove_style(temp_arc, NULL, LV_PART_KNOB);
    lv_obj_add_flag(temp_arc, LV_OBJ_FLAG_ADV_HITTEST);
    
    // Temperature value display// 温度值显示
    lv_obj_t * temp_label = lv_label_create(scr);
    lv_label_set_text_fmt(temp_label, "%d°C", (int)(temperature * 10) / 10);
    lv_obj_set_style_text_color(temp_label, lv_color_hex(0xff6b35), 0);
    lv_obj_set_style_text_font(temp_label, &lv_font_montserrat_14, 0);
    lv_obj_set_pos(temp_label, 75, 110);
    
    // Temperature label// 温度标签
    lv_obj_t * temp_title = lv_label_create(scr);
    lv_label_set_text(temp_title, "Temp");
    lv_obj_set_style_text_color(temp_title, lv_color_hex(0x555555), 0);
    lv_obj_set_style_text_font(temp_title, &lv_font_montserrat_14, 0);
    lv_obj_set_pos(temp_title, 67, 190);
    
    // Humidity circular progress bar// 湿度圆形进度条
    lv_obj_t * hum_arc = lv_arc_create(scr);
    lv_obj_set_size(hum_arc, 120, 120);
    lv_obj_set_pos(hum_arc, 170, 60);
    lv_arc_set_rotation(hum_arc, 270);
    lv_arc_set_bg_angles(hum_arc, 0, 360);
    lv_arc_set_value(hum_arc, (int)humidity);
    lv_obj_set_style_arc_color(hum_arc, lv_color_hex(0x00bcd4), LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(hum_arc, lv_color_hex(0xe0e0e0), LV_PART_MAIN);
    lv_obj_set_style_arc_width(hum_arc, 8, LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(hum_arc, 8, LV_PART_MAIN);
    lv_obj_remove_style(hum_arc, NULL, LV_PART_KNOB);
    lv_obj_add_flag(hum_arc, LV_OBJ_FLAG_ADV_HITTEST);
    
    // Humidity value display// 湿度值显示
    lv_obj_t * hum_label = lv_label_create(scr);
    lv_label_set_text_fmt(hum_label, "%d%%", (int)humidity);
    lv_obj_set_style_text_color(hum_label, lv_color_hex(0x00bcd4), 0);
    lv_obj_set_style_text_font(hum_label, &lv_font_montserrat_14, 0);
    lv_obj_set_pos(hum_label, 217, 110);
    
    // Humidity label// 湿度标签
    lv_obj_t * hum_title = lv_label_create(scr);
    lv_label_set_text(hum_title, "Humidity");
    lv_obj_set_style_text_color(hum_title, lv_color_hex(0x555555), 0);
    lv_obj_set_style_text_font(hum_title, &lv_font_montserrat_14, 0);
    lv_obj_set_pos(hum_title, 200, 190);
    
    // Status indicator// 状态指示器
    lv_obj_t * status_label = lv_label_create(scr);
    if (temperature >= 18 && temperature <= 26 && humidity >= 40 && humidity <= 70) {
        lv_label_set_text(status_label, "Comfortable");
        lv_obj_set_style_text_color(status_label, lv_color_hex(0x4caf50), 0);
    } else {
        lv_label_set_text(status_label, "Warning");
        lv_obj_set_style_text_color(status_label, lv_color_hex(0xff9800), 0);
    }
    lv_obj_set_style_text_font(status_label, &lv_font_montserrat_14, 0);
    lv_obj_align(status_label, LV_ALIGN_BOTTOM_MID, 0, -10);
}

#endif