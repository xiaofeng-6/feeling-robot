#if 1
#include "lvgl.h"
//--------这里放函数

void worry(void)
{
    lv_init();
    osal_printk("worry lv_init done\r\n");
    lv_port_disp_init();
    osal_printk("worry lv_port_disp_init done\r\n");

    lv_obj_clean(lv_scr_act());  //清除屏幕
    /* 创建用于椭圆弧线（宽度13）的样式 */
    static lv_style_t style_arc;
    lv_style_init(&style_arc);
    lv_style_set_line_width(&style_arc, 13);
    lv_style_set_line_color(&style_arc, lv_color_black());
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

        const float x0 = 140.0f, y0 = 80.0f;              /* 起点 */
        const float x1 = 225.0f, y1 = 40.0f;              /* 终点 */
        const float cx = 217.0f, cy = 85.0f;             /* 控制点，决定弧度向下凸起 */

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

    /* ---------------- 画圆（圆心200,130 半径40） ---------------- */
    {
        lv_obj_t * big_circle = lv_obj_create(lv_scr_act());
        lv_obj_set_size(big_circle, 80, 80);  // 直径80 = 半径40 * 2
        lv_obj_set_pos(big_circle, 160, 90);  // 圆心(200,130) - 半径40 = 左上角(160,90)
        lv_obj_set_style_radius(big_circle, LV_RADIUS_CIRCLE, LV_PART_MAIN);  // 圆形
        lv_obj_set_style_bg_color(big_circle, lv_color_hex(0x92C), LV_PART_MAIN);  // 0x92C填充
        lv_obj_set_style_bg_opa(big_circle, LV_OPA_COVER, LV_PART_MAIN);  // 不透明
        lv_obj_set_style_border_width(big_circle, 0, LV_PART_MAIN);  // 无边框
        lv_obj_remove_flag(big_circle, LV_OBJ_FLAG_CLICKABLE); // 不可点击
    }

    /* ---------------- 画白色小圆（圆心185,115 半径10） ---------------- */
    {
        lv_obj_t * small_circle = lv_obj_create(lv_scr_act());
        lv_obj_set_size(small_circle, 20, 20);  // 直径20 = 半径10 * 2
        lv_obj_set_pos(small_circle, 175, 105); // 圆心(185,115) - 半径10 = 左上角(175,105)
        lv_obj_set_style_radius(small_circle, LV_RADIUS_CIRCLE, LV_PART_MAIN);  // 圆形
        lv_obj_set_style_bg_color(small_circle, lv_color_white(), LV_PART_MAIN);  // 白色填充
        lv_obj_set_style_bg_opa(small_circle, LV_OPA_COVER, LV_PART_MAIN);  // 不透明
        lv_obj_set_style_border_width(small_circle, 0, LV_PART_MAIN);  // 无边框
        lv_obj_remove_flag(small_circle, LV_OBJ_FLAG_CLICKABLE); // 不可点击
    }

    /* ---------------- 画三条粉色直线 ---------------- */
    {
        /* 第一条直线：120,170 到 100,190 */
        static lv_point_precise_t pink_line1_points[] = {
            {120, 170},
            {100, 190}
        };
        
        lv_obj_t * pink_line1 = lv_line_create(lv_scr_act());
        lv_line_set_points(pink_line1, pink_line1_points, 2);
        lv_obj_add_style(pink_line1, &style_pink, LV_PART_MAIN);

        /* 第二条直线：140,170 到 120,190 */
        static lv_point_precise_t pink_line2_points[] = {
            {140, 170},
            {120, 190}
        };
        
        lv_obj_t * pink_line2 = lv_line_create(lv_scr_act());
        lv_line_set_points(pink_line2, pink_line2_points, 2);
        lv_obj_add_style(pink_line2, &style_pink, LV_PART_MAIN);

        /* 第三条直线：160,170 到 140,190 */
        static lv_point_precise_t pink_line3_points[] = {
            {160, 170},
            {140, 190}
        };
        
        lv_obj_t * pink_line3 = lv_line_create(lv_scr_act());
        lv_line_set_points(pink_line3, pink_line3_points, 2);
        lv_obj_add_style(pink_line3, &style_pink, LV_PART_MAIN);
    }
}

//-----------------
#endif

