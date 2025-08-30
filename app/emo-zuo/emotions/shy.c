#if 1
#include "lvgl.h"

//--------这里放函数

void shy(void)
{
        lv_init();
    osal_printk("shy lv_init done\r\n");
    lv_port_disp_init();
    osal_printk("shy lv_port_disp_init done\r\n");

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

    /* 创建黑色弧线样式 */
    static lv_style_t style_black_arc;
    lv_style_init(&style_black_arc);
    lv_style_set_line_width(&style_black_arc, 15);
    lv_style_set_line_color(&style_black_arc, lv_color_hex(0x000000)); /* 黑色 */
    lv_style_set_line_rounded(&style_black_arc, true);

    /* ---------------- 画第一条弧线：起点(180,40)，终点(280,120)，控制点(260,60) ---------------- */
    {
        /* 使用二次贝塞尔曲线 */
        const int POINT_CNT = 21; /* 取21个采样点 */
        static lv_point_precise_t arc1_points[21];

        const float x0 = 180.0f, y0 = 40.0f;   /* 起点 */
        const float x1 = 280.0f, y1 = 120.0f;  /* 终点 */
        const float cx = 260.0f, cy = 60.0f;   /* 控制点 */

        // 生成二次贝塞尔曲线点
        for(int i = 0; i < POINT_CNT; ++i)
        {
            float t = (float)i / (POINT_CNT - 1);
            float one_minus_t = 1.0f - t;
            float xt = one_minus_t * one_minus_t * x0 + 2.0f * one_minus_t * t * cx + t * t * x1;
            float yt = one_minus_t * one_minus_t * y0 + 2.0f * one_minus_t * t * cy + t * t * y1;
            arc1_points[i].x = xt;
            arc1_points[i].y = yt;
        }

        lv_obj_t * arc1 = lv_line_create(lv_scr_act());
        lv_line_set_points(arc1, arc1_points, POINT_CNT);
        lv_obj_add_style(arc1, &style_black_arc, LV_PART_MAIN);
    }

    /* ---------------- 画第二条弧线：起点(140,100)，终点(280,120)，控制点(220,80) ---------------- */
    {
        /* 使用二次贝塞尔曲线 */
        const int POINT_CNT = 21; /* 取21个采样点 */
        static lv_point_precise_t arc2_points[21];

        const float x0 = 140.0f, y0 = 100.0f;  /* 起点 */
        const float x1 = 280.0f, y1 = 120.0f;  /* 终点 */
        const float cx = 220.0f, cy = 80.0f;   /* 控制点 */

        // 生成二次贝塞尔曲线点
        for(int i = 0; i < POINT_CNT; ++i)
        {
            float t = (float)i / (POINT_CNT - 1);
            float one_minus_t = 1.0f - t;
            float xt = one_minus_t * one_minus_t * x0 + 2.0f * one_minus_t * t * cx + t * t * x1;
            float yt = one_minus_t * one_minus_t * y0 + 2.0f * one_minus_t * t * cy + t * t * y1;
            arc2_points[i].x = xt;
            arc2_points[i].y = yt;
        }

        lv_obj_t * arc2 = lv_line_create(lv_scr_act());
        lv_line_set_points(arc2, arc2_points, POINT_CNT);
        lv_obj_add_style(arc2, &style_black_arc, LV_PART_MAIN);
    }

    /* ---------------- 画第三条弧线：起点(180,160)，终点(280,120)，控制点(200,120) ---------------- */
    {
        /* 使用二次贝塞尔曲线 */
        const int POINT_CNT = 21; /* 取21个采样点 */
        static lv_point_precise_t arc3_points[21];

        const float x0 = 180.0f, y0 = 160.0f;  /* 起点 */
        const float x1 = 280.0f, y1 = 120.0f;  /* 终点 */
        const float cx = 200.0f, cy = 120.0f;  /* 控制点 */

        // 生成二次贝塞尔曲线点
        for(int i = 0; i < POINT_CNT; ++i)
        {
            float t = (float)i / (POINT_CNT - 1);
            float one_minus_t = 1.0f - t;
            float xt = one_minus_t * one_minus_t * x0 + 2.0f * one_minus_t * t * cx + t * t * x1;
            float yt = one_minus_t * one_minus_t * y0 + 2.0f * one_minus_t * t * cy + t * t * y1;
            arc3_points[i].x = xt;
            arc3_points[i].y = yt;
        }

        lv_obj_t * arc3 = lv_line_create(lv_scr_act());
        lv_line_set_points(arc3, arc3_points, POINT_CNT);
        lv_obj_add_style(arc3, &style_black_arc, LV_PART_MAIN);
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
//#include <math.h>


//下面是动画版，但是动的很慢，我不知道为什么，应该是内存爆了


#if 0
#include "lvgl.h"

//--------这里放函数

// 极简内存版本：只存储3个对象指针，不存储点数据
static lv_obj_t * arc1_obj = NULL;
static lv_obj_t * arc2_obj = NULL;
static lv_obj_t * arc3_obj = NULL;

// 简单的动画回调：0-50像素来回移动
void simple_move_animation_cb(void * var, int32_t val)
{
    // val直接就是偏移量：0到50像素
    lv_obj_set_y(arc1_obj, val);
    lv_obj_set_y(arc2_obj, val);
    lv_obj_set_y(arc3_obj, val);
}

// 开始简单移动动画
void start_simple_move_animation(void)
{
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, NULL);
    lv_anim_set_values(&anim, 0, 50);    // 直接0到50像素
    lv_anim_set_time(&anim, 2000);       // 2秒完成一个完整的来回
    lv_anim_set_exec_cb(&anim, simple_move_animation_cb);
    lv_anim_set_path_cb(&anim, lv_anim_path_ease_in_out);  // 缓进缓出效果
    lv_anim_set_repeat_count(&anim, LV_ANIM_REPEAT_INFINITE);  // 无限重复
    lv_anim_set_playback_time(&anim, 2000);  // 反向播放时间也是2秒
    lv_anim_start(&anim);
}

void fear(void)
{

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

    /* 创建黑色弧线样式 */
    static lv_style_t style_black_arc;
    lv_style_init(&style_black_arc);
    lv_style_set_line_width(&style_black_arc, 15);
    lv_style_set_line_color(&style_black_arc, lv_color_hex(0x000000)); /* 黑色 */
    lv_style_set_line_rounded(&style_black_arc, true);

    /* ---------------- 画第一条弧线：起点(180,40)，终点(280,120)，控制点(260,60) ---------------- */
    {
        /* 使用二次贝塞尔曲线 */
        const int POINT_CNT = 21; /* 取21个采样点 */
        static lv_point_precise_t arc1_points[21];

        const float x0 = 180.0f, y0 = 40.0f;   /* 起点 */
        const float x1 = 280.0f, y1 = 120.0f;  /* 终点 */
        const float cx = 260.0f, cy = 60.0f;   /* 控制点 */

        // 生成二次贝塞尔曲线点
        for(int i = 0; i < POINT_CNT; ++i)
        {
            float t = (float)i / (POINT_CNT - 1);
            float one_minus_t = 1.0f - t;
            float xt = one_minus_t * one_minus_t * x0 + 2.0f * one_minus_t * t * cx + t * t * x1;
            float yt = one_minus_t * one_minus_t * y0 + 2.0f * one_minus_t * t * cy + t * t * y1;
            arc1_points[i].x = xt;
            arc1_points[i].y = yt;
        }

        lv_obj_t * arc1 = lv_line_create(lv_scr_act());
        lv_line_set_points(arc1, arc1_points, POINT_CNT);
        lv_obj_add_style(arc1, &style_black_arc, LV_PART_MAIN);
        arc1_obj = arc1; // 存储对象指针
        
    }

    /* ---------------- 画第二条弧线：起点(140,100)，终点(280,120)，控制点(220,80) ---------------- */
    {
        /* 使用二次贝塞尔曲线 */
        const int POINT_CNT = 21; /* 取21个采样点 */
        static lv_point_precise_t arc2_points[21];

        const float x0 = 140.0f, y0 = 100.0f;  /* 起点 */
        const float x1 = 280.0f, y1 = 120.0f;  /* 终点 */
        const float cx = 220.0f, cy = 80.0f;   /* 控制点 */

        // 生成二次贝塞尔曲线点
        for(int i = 0; i < POINT_CNT; ++i)
        {
            float t = (float)i / (POINT_CNT - 1);
            float one_minus_t = 1.0f - t;
            float xt = one_minus_t * one_minus_t * x0 + 2.0f * one_minus_t * t * cx + t * t * x1;
            float yt = one_minus_t * one_minus_t * y0 + 2.0f * one_minus_t * t * cy + t * t * y1;
            arc2_points[i].x = xt;
            arc2_points[i].y = yt;
        }

        lv_obj_t * arc2 = lv_line_create(lv_scr_act());
        lv_line_set_points(arc2, arc2_points, POINT_CNT);
        lv_obj_add_style(arc2, &style_black_arc, LV_PART_MAIN);
        arc2_obj = arc2; // 存储对象指针
        
    }

    /* ---------------- 画第三条弧线：起点(180,160)，终点(280,120)，控制点(200,120) ---------------- */
    {
        /* 使用二次贝塞尔曲线 */
        const int POINT_CNT = 21; /* 取21个采样点 */
        static lv_point_precise_t arc3_points[21];

        const float x0 = 180.0f, y0 = 160.0f;  /* 起点 */
        const float x1 = 280.0f, y1 = 120.0f;  /* 终点 */
        const float cx = 200.0f, cy = 120.0f;  /* 控制点 */

        // 生成二次贝塞尔曲线点
        for(int i = 0; i < POINT_CNT; ++i)
        {
            float t = (float)i / (POINT_CNT - 1);
            float one_minus_t = 1.0f - t;
            float xt = one_minus_t * one_minus_t * x0 + 2.0f * one_minus_t * t * cx + t * t * x1;
            float yt = one_minus_t * one_minus_t * y0 + 2.0f * one_minus_t * t * cy + t * t * y1;
            arc3_points[i].x = xt;
            arc3_points[i].y = yt;
        }

        lv_obj_t * arc3 = lv_line_create(lv_scr_act());
        lv_line_set_points(arc3, arc3_points, POINT_CNT);
        lv_obj_add_style(arc3, &style_black_arc, LV_PART_MAIN);
        arc3_obj = arc3; // 存储对象指针
        
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

    // 开始简单移动动画
    start_simple_move_animation();
}

//-----------------
#endif
