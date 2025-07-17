
#ifndef __ATK_MD0240_H
#define __ATK_MD0240_H

#include "gpio.h"

#include <string.h>
#include "pinctrl.h"


/* 定义ATK-MD0240模块LCD尺寸 */
#define ATK_MD0240_LCD_WIDTH                320
#define ATK_MD0240_LCD_HEIGHT               240

/* 定义ATK-MD0240模块启用的字体 */
#define ATK_MD0240_FONT_12                  1
#define ATK_MD0240_FONT_16                  1
#define ATK_MD0240_FONT_24                  1
#define ATK_MD0240_FONT_32                  1

/* 默认启用12号字体 */
#if ((ATK_MD0240_FONT_12 == 0) && (ATK_MD0240_FONT_16 == 0) && (ATK_MD0240_FONT_24 == 0) && (ATK_MD0240_FONT_32 == 0))
#undef ATK_MD0240_FONT_12
#define ATK_MD0240_FONT_12 1
#endif

/* NearLink_DK_WS63E引脚定义 */
#define ATK_MD0240_PWR_PIN   0   // GPIO08 (背光)
#define ATK_MD0240_CS_PIN    3   // GPIO10 (CS)
#define ATK_MD0240_WR_PIN    1   // GPIO11 (DC/WR)
#define ATK_MD0240_RST_PIN   2  // GPIO9 RST

/* 宏操作（适配鸿蒙GPIO接口） */
#define ATK_MD0240_PWR(x)   uapi_gpio_set_val(ATK_MD0240_PWR_PIN, (x) ? GPIO_LEVEL_HIGH : GPIO_LEVEL_LOW)
#define ATK_MD0240_CS(x)    uapi_gpio_set_val(ATK_MD0240_CS_PIN, (x) ? GPIO_LEVEL_HIGH : GPIO_LEVEL_LOW)
#define ATK_MD0240_WR(x)    uapi_gpio_set_val(ATK_MD0240_WR_PIN, (x) ? GPIO_LEVEL_HIGH : GPIO_LEVEL_LOW)
#define ATK_MD0240_RST(x)   uapi_gpio_set_val(ATK_MD0240_RST_PIN, (x) ? GPIO_LEVEL_HIGH : GPIO_LEVEL_LOW)

/* ATK-MD0240模块LCD显示字体枚举 */
typedef enum
{
#if (ATK_MD0240_FONT_12 != 0)
    ATK_MD0240_LCD_FONT_12,             /* 12号字体 */
#endif
#if (ATK_MD0240_FONT_16 != 0)
    ATK_MD0240_LCD_FONT_16,             /* 16号字体 */
#endif
#if (ATK_MD0240_FONT_24 != 0)
    ATK_MD0240_LCD_FONT_24,             /* 24号字体 */
#endif
#if (ATK_MD0240_FONT_32 != 0)
    ATK_MD0240_LCD_FONT_32,             /* 32号字体 */
#endif
} atk_md0240_lcd_font_t;

/* ATK-MD0240模块LCD显示数字模式枚举 */
typedef enum
{
    ATK_MD0240_NUM_SHOW_NOZERO = 0x00,  /* 数字高位0不显示 */
    ATK_MD0240_NUM_SHOW_ZERO,           /* 数字高位0显示 */
} atk_md0240_num_mode_t;

/* 常用颜色定义（RGB565） */
#define ATK_MD0240_WHITE        0xFFFF
#define ATK_MD0240_BLACK        0x0000
#define ATK_MD0240_BLUE         0x001F
#define ATK_MD0240_BRED         0XF81F
#define ATK_MD0240_GRED         0XFFE0
#define ATK_MD0240_GBLUE        0X07FF
#define ATK_MD0240_RED          0xF800
#define ATK_MD0240_MAGENTA      0xF81F
#define ATK_MD0240_GREEN        0x07E0
#define ATK_MD0240_CYAN         0x7FFF
#define ATK_MD0240_YELLOW       0xFFE0
#define ATK_MD0240_BROWN        0XBC40
#define ATK_MD0240_BRRED        0XFC07
#define ATK_MD0240_GRAY         0X8430

/* 操作函数 */
void atk_md0240_init(void);                                                                                                                             /* ATK-MD0240模块初始化 */
void atk_md0240_display_on(void);                                                                                                                       /* 开启ATK-MD0240模块LCD背光 */
void atk_md0240_display_off(void);                                                                                                                      /* 关闭ATK-MD0240模块LCD背光 */
void atk_md0240_fill(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint16_t color);                                                               /* ATK-MD0240模块LCD区域填充 */
void atk_md0240_clear(uint16_t color);                                                                                                                  /* ATK-MD0240模块LCD清屏 */
void atk_md0240_draw_point(uint16_t x, uint16_t y, uint16_t color);                                                                                     /* ATK-MD0240模块LCD画点 */
void atk_md0240_draw_line(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye, uint16_t color);                                                          /* ATK-MD0240模块LCD画线段 */
void atk_md0240_draw_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);                                                          /* ATK-MD0240模块LCD画矩形框 */
void atk_md0240_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);                                                                        /* ATK-MD0240模块LCD画圆形框 */
void atk_md0240_show_char(uint16_t x, uint16_t y, char ch, atk_md0240_lcd_font_t font, uint16_t color);                                                 /* ATK-MD0240模块LCD显示1个字符 */
void atk_md0240_show_string(uint16_t x, uint16_t y, char *str, atk_md0240_lcd_font_t font, uint16_t color);                                             /* ATK-MD0240模块LCD显示字符串 */
void atk_md0240_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, atk_md0240_num_mode_t mode, atk_md0240_lcd_font_t font, uint16_t color);   /* ATK-MD0240模块LCD显示数字，可控制显示高位0 */
void atk_md0240_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, atk_md0240_lcd_font_t font, uint16_t color);                                /* ATK-MD0240模块LCD显示数字，不显示高位0 */
void atk_md0240_show_pic(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t *pic);                                                        /* ATK-MD0240模块LCD图片 */

#endif
