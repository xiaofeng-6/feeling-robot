#if 1
#include "lvgl.h"
//--------这里放函数

void cry(void)
{
#if 1//初始化lv_init  lv_port_disp_init 清屏lv_obj_clean
        lv_init();
    osal_printk("Happy lv_init done\r\n");
    lv_port_disp_init();
    osal_printk("Happy lv_port_disp_init done\r\n");

    lv_obj_clean(lv_scr_act());  //清除屏幕

#endif



#if 1 // 静态图像
    /* 创建用于椭圆弧线（宽度20）的样式 */
    static lv_style_t style_arc;
    lv_style_init(&style_arc);
    lv_style_set_line_width(&style_arc, 20);
    lv_style_set_line_color(&style_arc, lv_color_hex(0x92C));
    lv_style_set_line_rounded(&style_arc, true);

    /* 创建用于折线（宽度25）的样式 */
    static lv_style_t style_poly;
    lv_style_init(&style_poly);
    lv_style_set_line_width(&style_poly, 25);
    lv_style_set_line_color(&style_poly, lv_color_hex(0x92C));
    lv_style_set_line_rounded(&style_poly, true);

    /* 创建用于粉色直线的样式 */
    static lv_style_t style_pink;
    lv_style_init(&style_pink);
    lv_style_set_line_width(&style_pink, 5);
    lv_style_set_line_color(&style_pink, lv_palette_main(LV_PALETTE_PINK));
    lv_style_set_line_rounded(&style_pink, true);

    /* ---------------- 画椭圆弧线 ---------------- */
    {
        /* 使用二次贝塞尔曲线近似椭圆弧，生成若干点 */
        const int POINT_CNT = 21; /* 取 21 个采样点 */
        static lv_point_precise_t arc_points[21];

        const float x0 = 180.0f, y0 = 75.0f;              /* 起点 */
        const float x1 = 90.0f, y1 = 45.0f;               /* 终点 */
        const float cx = 103.0f, cy = 85.0f;              /* 控制点，决定弧度向下凸起 */

        // 生成弧线点
        for(int i = 0; i < POINT_CNT; ++i)
        {
            float t = (float)i / (POINT_CNT - 1);
            float one_minus_t = 1.0f - t;
            float xt = one_minus_t * one_minus_t * x0 + 2.0f * one_minus_t * t * cx + t * t * x1;
            float yt = one_minus_t * one_minus_t * y0 + 2.0f * one_minus_t * t * cy + t * t * y1;
            arc_points[i].x = xt;
            arc_points[i].y = yt;
        }

        lv_obj_t * arc_line = lv_line_create(lv_scr_act());
        lv_line_set_points(arc_line, arc_points, POINT_CNT);
        lv_obj_add_style(arc_line, &style_arc, LV_PART_MAIN);
    }

    /* ---------------- 画淡蓝色矩形（90,115 到 140,215） ---------------- */
    {
        lv_obj_t * blue_rect = lv_obj_create(lv_scr_act());
        lv_obj_set_pos(blue_rect, 90, 115);  // 左上角(90,115)
        lv_obj_set_size(blue_rect, 50, 100);  // 宽度50，高度100
        lv_obj_set_style_radius(blue_rect, 0, LV_PART_MAIN);  // 矩形（无圆角）
        lv_obj_set_style_bg_color(blue_rect, lv_color_hex(0x87CEEB), LV_PART_MAIN);  // 淡蓝色填充
        lv_obj_set_style_bg_opa(blue_rect, LV_OPA_COVER, LV_PART_MAIN);  // 不透明
        lv_obj_set_style_border_width(blue_rect, 0, LV_PART_MAIN);  // 无边框
        lv_obj_remove_flag(blue_rect, LV_OBJ_FLAG_CLICKABLE); // 不可点击
    }

    /* ---------------- 画直线（70,115 到 160,115） ---------------- */
    {
        /* 创建直线样式 */
        static lv_style_t style_straight_line;
        lv_style_init(&style_straight_line);
        lv_style_set_line_width(&style_straight_line, 20);
        lv_style_set_line_color(&style_straight_line, lv_color_hex(0x92C));
        lv_style_set_line_rounded(&style_straight_line, true);

        static lv_point_precise_t straight_line_points[] = {
            {160, 115},
            {70, 115}
        };
        
        lv_obj_t * straight_line = lv_line_create(lv_scr_act());
        lv_line_set_points(straight_line, straight_line_points, 2);
        lv_obj_add_style(straight_line, &style_straight_line, LV_PART_MAIN);
    }

    /* ---------------- 画三条粉色直线 ---------------- */
    {
        /* 第一条直线：200,160 到 220,180 */
        static lv_point_precise_t pink_line1_points[] = {
            {200, 160},
            {220, 180}
        };
        
        lv_obj_t * pink_line1 = lv_line_create(lv_scr_act());
        lv_line_set_points(pink_line1, pink_line1_points, 2);
        lv_obj_add_style(pink_line1, &style_pink, LV_PART_MAIN);

        /* 第二条直线：180,160 到 200,180 */
        static lv_point_precise_t pink_line2_points[] = {
            {180, 160},
            {200, 180}
        };
        
        lv_obj_t * pink_line2 = lv_line_create(lv_scr_act());
        lv_line_set_points(pink_line2, pink_line2_points, 2);
        lv_obj_add_style(pink_line2, &style_pink, LV_PART_MAIN);

        /* 第三条直线：160,160 到 180,180 */
        static lv_point_precise_t pink_line3_points[] = {
            {160, 160},
            {180, 180}
        };
        
        lv_obj_t * pink_line3 = lv_line_create(lv_scr_act());
        lv_line_set_points(pink_line3, pink_line3_points, 2);
        lv_obj_add_style(pink_line3, &style_pink, LV_PART_MAIN);
    }

#endif



}

//-----------------
#endif
