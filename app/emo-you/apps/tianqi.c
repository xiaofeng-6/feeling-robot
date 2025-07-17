#if 1// 天气显示函数

#include "lvgl.h"

void tianqi(void)
{
#if 1
        lv_init();
    osal_printk("tianqi lv_init done\r\n");
    lv_port_disp_init();
    osal_printk("tianqi lv_port_disp_init done\r\n");

    lv_obj_clean(lv_scr_act());  //清除屏幕
#endif

    // 获取当前活动屏幕
    lv_obj_t * scr = lv_screen_active();
    
    // 第一个表盘：最高气温表盘（仿Network Speed设计）
    lv_obj_t * max_temp_scale = lv_scale_create(scr);
    lv_obj_set_size(max_temp_scale, 80, 80);
    lv_obj_set_pos(max_temp_scale, 20, 20);
    lv_scale_set_mode(max_temp_scale, LV_SCALE_MODE_ROUND_OUTER);
    lv_scale_set_range(max_temp_scale, 0, 40);
    lv_scale_set_total_tick_count(max_temp_scale, 9);  // 9个刻度点
    lv_scale_set_major_tick_every(max_temp_scale, 2);  // 每2个一个主刻度
    lv_scale_set_angle_range(max_temp_scale, 270);  // 270度范围
    lv_scale_set_rotation(max_temp_scale, 135);     // 从左下开始
    lv_obj_set_style_length(max_temp_scale, 8, LV_PART_INDICATOR);  // 主刻度长度
    lv_obj_set_style_length(max_temp_scale, 4, LV_PART_ITEMS);      // 次刻度长度
    lv_scale_set_label_show(max_temp_scale, false);  // 隐藏刻度数字
    
    // 添加高温危险区段（30-40度为红色）
    lv_scale_section_t * hot_section = lv_scale_add_section(max_temp_scale);
    lv_scale_section_set_range(hot_section, 30, 40);
    
    // 创建指示器弧线
    lv_obj_t * max_temp_arc = lv_arc_create(max_temp_scale);
    lv_obj_set_size(max_temp_arc, lv_pct(100), lv_pct(100));
    lv_obj_set_style_margin_all(max_temp_arc, 8, 0);
    lv_obj_remove_style(max_temp_arc, NULL, LV_PART_KNOB);
    lv_obj_set_style_arc_opa(max_temp_arc, 0, LV_PART_MAIN);
    lv_obj_set_style_arc_width(max_temp_arc, 6, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(max_temp_arc, lv_color_hex(0xff4444), LV_PART_INDICATOR);
    lv_obj_clear_flag(max_temp_arc, LV_OBJ_FLAG_CLICKABLE);
    lv_arc_set_range(max_temp_arc, 0, 40);
    lv_arc_set_value(max_temp_arc, 28);  // 当前温度28度
    lv_obj_center(max_temp_arc);
    
    // 添加中心温度标签
    lv_obj_t * max_temp_label = lv_label_create(max_temp_scale);
    lv_label_set_text(max_temp_label, "28°");
    lv_obj_set_style_text_color(max_temp_label, lv_color_hex(0xff4444), 0);
    lv_obj_set_style_text_font(max_temp_label, &lv_font_montserrat_14, 0);
    lv_obj_align(max_temp_label, LV_ALIGN_CENTER, 0, 0);
    
    // 添加标题标签
    lv_obj_t * max_temp_title = lv_label_create(scr);
    lv_label_set_text(max_temp_title, "Max");
    lv_obj_set_style_text_color(max_temp_title, lv_color_hex(0x666666), 0);
    lv_obj_set_style_text_font(max_temp_title, &lv_font_montserrat_14, 0);
    lv_obj_set_pos(max_temp_title, 45, 95);
    
    // 第二个表盘：最低气温表盘（仿Network Speed设计）
    lv_obj_t * min_temp_scale = lv_scale_create(scr);
    lv_obj_set_size(min_temp_scale, 80, 80);
    lv_obj_set_pos(min_temp_scale, 20, 120);
    lv_scale_set_mode(min_temp_scale, LV_SCALE_MODE_ROUND_OUTER);
    lv_scale_set_range(min_temp_scale, -10, 30);
    lv_scale_set_total_tick_count(min_temp_scale, 9);  // 9个刻度点
    lv_scale_set_major_tick_every(min_temp_scale, 2);  // 每2个一个主刻度
    lv_scale_set_angle_range(min_temp_scale, 270);  // 270度范围
    lv_scale_set_rotation(min_temp_scale, 135);     // 从左下开始
    lv_obj_set_style_length(min_temp_scale, 8, LV_PART_INDICATOR);  // 主刻度长度
    lv_obj_set_style_length(min_temp_scale, 4, LV_PART_ITEMS);      // 次刻度长度
    lv_scale_set_label_show(min_temp_scale, false);  // 隐藏刻度数字
    
    // 添加低温区段（-10到5度为蓝色）
    lv_scale_section_t * cold_section = lv_scale_add_section(min_temp_scale);
    lv_scale_section_set_range(cold_section, -10, 5);
    
    // 创建指示器弧线
    lv_obj_t * min_temp_arc = lv_arc_create(min_temp_scale);
    lv_obj_set_size(min_temp_arc, lv_pct(100), lv_pct(100));
    lv_obj_set_style_margin_all(min_temp_arc, 8, 0);
    lv_obj_remove_style(min_temp_arc, NULL, LV_PART_KNOB);
    lv_obj_set_style_arc_opa(min_temp_arc, 0, LV_PART_MAIN);
    lv_obj_set_style_arc_width(min_temp_arc, 6, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(min_temp_arc, lv_color_hex(0x4488ff), LV_PART_INDICATOR);
    lv_obj_clear_flag(min_temp_arc, LV_OBJ_FLAG_CLICKABLE);
    lv_arc_set_range(min_temp_arc, -10, 30);
    lv_arc_set_value(min_temp_arc, 15);  // 当前温度15度
    lv_obj_center(min_temp_arc);
    
    // 添加中心温度标签
    lv_obj_t * min_temp_label = lv_label_create(min_temp_scale);
    lv_label_set_text(min_temp_label, "15°");
    lv_obj_set_style_text_color(min_temp_label, lv_color_hex(0x4488ff), 0);
    lv_obj_set_style_text_font(min_temp_label, &lv_font_montserrat_14, 0);
    lv_obj_align(min_temp_label, LV_ALIGN_CENTER, 0, 0);
    
    // 添加标题标签
    lv_obj_t * min_temp_title = lv_label_create(scr);
    lv_label_set_text(min_temp_title, "Min");
    lv_obj_set_style_text_color(min_temp_title, lv_color_hex(0x666666), 0);
    lv_obj_set_style_text_font(min_temp_title, &lv_font_montserrat_14, 0);
    lv_obj_set_pos(min_temp_title, 45, 195);
    
    // 第三个区域：Monthly revenue风格的柱状图显示温度数据
    // 添加标题
    lv_obj_t * chart_title = lv_label_create(scr);
    lv_label_set_text(chart_title, "Daily Temperature");
    lv_obj_set_style_text_color(chart_title, lv_color_hex(0x333333), 0);
    lv_obj_set_style_text_font(chart_title, &lv_font_montserrat_14, 0);
    lv_obj_set_pos(chart_title, 120, 20);
    
    // 创建柱状图
    lv_obj_t * temp_chart = lv_chart_create(scr);
    lv_obj_set_size(temp_chart, 180, 150);
    lv_obj_set_pos(temp_chart, 120, 40);
    lv_chart_set_type(temp_chart, LV_CHART_TYPE_BAR);
    lv_chart_set_point_count(temp_chart, 6);  // 6个时间段
    lv_chart_set_range(temp_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 35);
    lv_obj_set_style_bg_color(temp_chart, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_border_color(temp_chart, lv_color_hex(0xdddddd), 0);
    lv_obj_set_style_border_width(temp_chart, 1, 0);
    lv_obj_set_style_radius(temp_chart, 8, 0);
    lv_obj_set_style_pad_all(temp_chart, 10, 0);
    lv_obj_clear_flag(temp_chart, LV_OBJ_FLAG_CLICKABLE);
    
    // 设置柱状图样式
    lv_obj_set_style_pad_column(temp_chart, 2, LV_PART_MAIN);  // 柱子间距
    
    // 第一个数据系列：最高温度（橙色）
    lv_chart_series_t * max_temp_series = lv_chart_add_series(temp_chart, lv_color_hex(0xFF9500), LV_CHART_AXIS_PRIMARY_Y);
    
    // 第二个数据系列：最低温度（蓝色）
    lv_chart_series_t * min_temp_series = lv_chart_add_series(temp_chart, lv_color_hex(0x007AFF), LV_CHART_AXIS_PRIMARY_Y);
    
    // 6个时间段的温度数据（每4小时一个段）
    static int16_t max_temps[6] = {18, 22, 28, 32, 30, 24};  // 最高温度
    static int16_t min_temps[6] = {12, 15, 20, 24, 22, 16};  // 最低温度
    
    // 设置最高温度数据
    for(int i = 0; i < 6; i++) {
        lv_chart_set_next_value(temp_chart, max_temp_series, max_temps[i]);
    }
    
    // 设置最低温度数据
    for(int i = 0; i < 6; i++) {
        lv_chart_set_next_value(temp_chart, min_temp_series, min_temps[i]);
    }
    
    // 添加图例
    lv_obj_t * legend_max = lv_label_create(scr);
    lv_label_set_text(legend_max, "Max Temp");
    lv_obj_set_style_text_color(legend_max, lv_color_hex(0xFF9500), 0);
    lv_obj_set_style_text_font(legend_max, &lv_font_montserrat_14, 0);
    lv_obj_set_pos(legend_max, 120, 195);
    
    lv_obj_t * legend_min = lv_label_create(scr);
    lv_label_set_text(legend_min, "Min Temp");
    lv_obj_set_style_text_color(legend_min, lv_color_hex(0x007AFF), 0);
    lv_obj_set_style_text_font(legend_min, &lv_font_montserrat_14, 0);
    lv_obj_set_pos(legend_min, 210, 195);
    
    // 添加时间标签
    static const char * time_periods[] = {"4h", "8h", "12h", "16h", "20h", "24h"};
    for(int i = 0; i < 6; i++) {
        lv_obj_t * time_label = lv_label_create(scr);
        lv_label_set_text(time_label, time_periods[i]);
        lv_obj_set_style_text_color(time_label, lv_color_hex(0x666666), 0);
        lv_obj_set_style_text_font(time_label, &lv_font_montserrat_14, 0);
        lv_obj_set_pos(time_label, 130 + i * 25, 215);
    }
    
    // 刷新图表
    lv_chart_refresh(temp_chart);
}

#endif
