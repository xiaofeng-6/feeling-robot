#include "lvgl.h"
#include "lv_port_disp.h"

#include <stdio.h>     
#include <unistd.h>   
#include "ohos_init.h"  
#include "cmsis_os2.h"  
#include "watchdog.h"
//------------------------------------------

/**
 * @brief 为指定的对象添加一个上下往复的"挑眉"动画
 * @param obj 要添加动画的LVGL对象
 * @param start_y 动画的起始Y坐标
*/
static void tiaomei(lv_obj_t *obj, lv_coord_t start_y)
{
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, obj); // 绑定动画到对象

    // 设置动画的Y坐标变化范围：从指定的起始位置到其上方20像素的位置
    lv_anim_set_values(&anim, start_y, start_y - 20);

    // --- 配置动画参数 ---
    lv_anim_set_time(&anim, 500);                            // 动画时长: 500ms
    lv_anim_set_playback_time(&anim, 500);                   // 回放时长: 500ms
    lv_anim_set_repeat_count(&anim, LV_ANIM_REPEAT_INFINITE); // 无限重复
    lv_anim_set_path_cb(&anim, lv_anim_path_ease_in_out);     // 使用缓入缓出效果，更自然
    
    // 指定动画要修改的属性：Y坐标
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_obj_set_y);

    // 启动动画
    lv_anim_start(&anim);
}

static void lv_eyes()
{
    // 设置背景为白色
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_white(), 0);

    // 画黑色实心圆（眼球）
    int eye_diameter = 60;
    int eye_x = 320 - eye_diameter - 80;   // 眼球左上角x，距离右边界80
    int eye_y = 120;  // 眼球左上角y，图片中偏下
    lv_obj_t *eye = lv_obj_create(lv_scr_act());
    lv_obj_set_size(eye, eye_diameter, eye_diameter);
    lv_obj_set_style_radius(eye, eye_diameter / 2, 0);
    lv_obj_set_style_bg_color(eye, lv_color_black(), 0);
    lv_obj_set_style_border_width(eye, 0, 0);
    lv_obj_set_style_bg_opa(eye, LV_OPA_COVER, 0);
    lv_obj_set_pos(eye, eye_x, eye_y);
    lv_obj_set_style_shadow_width(eye, 8, 0);
    lv_obj_set_style_shadow_color(eye, lv_color_make(80,80,80), 0);

    // 画倾斜的黑色线条（眉毛），替代旋转矩形以避免抗锯齿伪影
    int lid_x = 320 - 100 - 60;   // 眉毛左上角x，距离右边界60
    int lid_y = 70;   // 眉毛左上角y，图片中在眼球上方

    lv_obj_t *lid = lv_line_create(lv_scr_act());
    static lv_point_precise_t lid_points[] = { {0, 0}, {98, 18} }; // 定义线条的起止点，实现倾斜效果 (左高右低)
    lv_line_set_points(lid, lid_points, 2);
    lv_obj_set_style_line_width(lid, 18, 0); // 设置线条宽度
    lv_obj_set_style_line_color(lid, lv_color_black(), 0); // 设置线条颜色
    lv_obj_set_style_line_rounded(lid, true, 0); // 让线条端点变圆滑
    lv_obj_set_pos(lid, lid_x, lid_y);

    // 为眉毛启动"挑眉"动画，并明确传入起始Y坐标
    tiaomei(lid, lid_y);
}

//------------------------------------------
static void OledTask(void)
{
    lv_init();
    lv_port_disp_init();

    lv_eyes();


    while (1) {
        uapi_watchdog_kick();
        lv_tick_inc(5);
        lv_timer_handler();
        uapi_systick_delay_ms(5);
    }
}


static void OledDemo(void)
{
    osThreadAttr_t attr;
    attr.name = "OledTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 1024 * 80;   
    attr.priority = osPriorityNormal;
    if (osThreadNew(OledTask, NULL, &attr) == NULL) {
        printf("[OledDemo] Falied to create OledTask!\n");
    }
}


APP_FEATURE_INIT(OledDemo);



