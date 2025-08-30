#if 1
#include "lvgl.h"
//--------这里放函数

void surprised(void)
{
    lv_init();
    osal_printk("surprised lv_init done\r\n");
    lv_port_disp_init();
    osal_printk("surprised lv_port_disp_init done\r\n");

    lv_obj_clean(lv_scr_act());  //清除屏幕
    /* 创建用于椭圆弧线（宽度15）的样式 */
    static lv_style_t style_arc;
    lv_style_init(&style_arc);
    lv_style_set_line_width(&style_arc, 15);
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

    /* 创建红色弧线样式 */
    static lv_style_t style_red_arc;
    lv_style_init(&style_red_arc);
    lv_style_set_line_width(&style_red_arc, 15);
    lv_style_set_line_color(&style_red_arc, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_line_rounded(&style_red_arc, true);

    /* ---------------- 画椭圆弧线 ---------------- */
    {
        /* 使用二次贝塞尔曲线近似椭圆弧，生成若干点 */
        const int POINT_CNT = 21; /* 取 21 个采样点 */
        static lv_point_precise_t arc_points[21];

        const float x0 = 130.0f, y0 = 80.0f;              /* 起点 */
        const float x1 = 225.0f, y1 = 40.0f;              /* 终点 */
        const float cx = 177.0f, cy = 10.0f;              /* 控制点，决定弧度向上凸起 */

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

    /* ---------------- 画第一条红色弧线 ---------------- */
    {
        /* 使用三次贝塞尔曲线，起点(220,100)，终点(220,200)，控制点(180,80)和(140,120) */
        const int POINT_CNT = 21; /* 取21个采样点 */
        static lv_point_precise_t red_arc1_points[21];

        const float x0 = 220.0f, y0 = 100.0f;  /* 起点 */
        const float x3 = 220.0f, y3 = 200.0f;  /* 终点 */
        const float x1 = 180.0f, y1 = 80.0f;   /* 第一个控制点 */
        const float x2 = 120.0f, y2 = 120.0f;  /* 第二个控制点 */

        // 生成三次贝塞尔曲线点
        for(int i = 0; i < POINT_CNT; ++i)
        {
            float t = (float)i / (POINT_CNT - 1);
            float one_minus_t = 1.0f - t;
            float one_minus_t_sq = one_minus_t * one_minus_t;
            float one_minus_t_cu = one_minus_t_sq * one_minus_t;
            float t_sq = t * t;
            float t_cu = t_sq * t;
            
            // 三次贝塞尔曲线公式: P(t) = (1-t)³P₀ + 3(1-t)²tP₁ + 3(1-t)t²P₂ + t³P₃
            float xt = one_minus_t_cu * x0 + 3.0f * one_minus_t_sq * t * x1 + 3.0f * one_minus_t * t_sq * x2 + t_cu * x3;
            float yt = one_minus_t_cu * y0 + 3.0f * one_minus_t_sq * t * y1 + 3.0f * one_minus_t * t_sq * y2 + t_cu * y3;
            
            red_arc1_points[i].x = xt;
            red_arc1_points[i].y = yt;
        }

        lv_obj_t * red_arc1 = lv_line_create(lv_scr_act());
        lv_line_set_points(red_arc1, red_arc1_points, POINT_CNT);
        lv_obj_add_style(red_arc1, &style_red_arc, LV_PART_MAIN);
    }

    /* ---------------- 画第二条红色弧线 ---------------- */
    {
        /* 使用三次贝塞尔曲线，起点(220,100)，终点(220,200)，控制点(260,80)和(300,120) */
        const int POINT_CNT = 21; /* 取21个采样点 */
        static lv_point_precise_t red_arc2_points[21];

        const float x0 = 220.0f, y0 = 100.0f;  /* 起点 */
        const float x3 = 220.0f, y3 = 200.0f;  /* 终点 */
        const float x1 = 260.0f, y1 = 80.0f;   /* 第一个控制点 */
        const float x2 = 320.0f, y2 = 120.0f;  /* 第二个控制点 */

        // 生成三次贝塞尔曲线点
        for(int i = 0; i < POINT_CNT; ++i)
        {
            float t = (float)i / (POINT_CNT - 1);
            float one_minus_t = 1.0f - t;
            float one_minus_t_sq = one_minus_t * one_minus_t;
            float one_minus_t_cu = one_minus_t_sq * one_minus_t;
            float t_sq = t * t;
            float t_cu = t_sq * t;
            
            // 三次贝塞尔曲线公式: P(t) = (1-t)³P₀ + 3(1-t)²tP₁ + 3(1-t)t²P₂ + t³P₃
            float xt = one_minus_t_cu * x0 + 3.0f * one_minus_t_sq * t * x1 + 3.0f * one_minus_t * t_sq * x2 + t_cu * x3;
            float yt = one_minus_t_cu * y0 + 3.0f * one_minus_t_sq * t * y1 + 3.0f * one_minus_t * t_sq * y2 + t_cu * y3;
            
            red_arc2_points[i].x = xt;
            red_arc2_points[i].y = yt;
        }

        lv_obj_t * red_arc2 = lv_line_create(lv_scr_act());
        lv_line_set_points(red_arc2, red_arc2_points, POINT_CNT);
        lv_obj_add_style(red_arc2, &style_red_arc, LV_PART_MAIN);
    }

    /* ---------------- 画三条粉色直线 ---------------- */
    {
        /* 第一条直线：120,160 到 100,180 */
        static lv_point_precise_t pink_line1_points[] = {
            {120, 160},
            {100, 180}
        };
        
        lv_obj_t * pink_line1 = lv_line_create(lv_scr_act());
        lv_line_set_points(pink_line1, pink_line1_points, 2);
        lv_obj_add_style(pink_line1, &style_pink, LV_PART_MAIN);

        /* 第二条直线：140,160 到 120,180 */
        static lv_point_precise_t pink_line2_points[] = {
            {140, 160},
            {120, 180}
        };
        
        lv_obj_t * pink_line2 = lv_line_create(lv_scr_act());
        lv_line_set_points(pink_line2, pink_line2_points, 2);
        lv_obj_add_style(pink_line2, &style_pink, LV_PART_MAIN);

        /* 第三条直线：160,160 到 140,180 */
        static lv_point_precise_t pink_line3_points[] = {
            {160, 160},
            {140, 180}
        };
        
        lv_obj_t * pink_line3 = lv_line_create(lv_scr_act());
        lv_line_set_points(pink_line3, pink_line3_points, 2);
        lv_obj_add_style(pink_line3, &style_pink, LV_PART_MAIN);
    }
}

//-----------------
#endif
