/**
 * @file lv_conf.h
 * Configuration file for v9.2.2
 */

/*
 * Copy this file as `lv_conf.h`
 * 1. simply next to the `lvgl` folder
 * 2. or any other places and
 *    - define `LV_CONF_INCLUDE_SIMPLE`
 *    - add the path as include path
 */

/* clang-format off */
#if 1 /*Set it to "1" to enable content*/

#ifndef LV_CONF_H
#define LV_CONF_H

/*If you need to include anything here, do it inside the `__ASSEMBLY__` guard */
#if  0 && defined(__ASSEMBLY__)
#include "my_include.h"
#endif

/*====================
   COLOR SETTINGS
 *====================*/

/*Color depth: 1 (I1), 8 (L8), 16 (RGB565), 24 (RGB888), 32 (XRGB8888)*/
#define LV_COLOR_DEPTH 16

/*=========================
   STDLIB WRAPPER SETTINGS
 *=========================*/

/* Possible values
 * - LV_STDLIB_BUILTIN:     LVGL's built in implementation
 * - LV_STDLIB_CLIB:        Standard C functions, like malloc, strlen, etc
 * - LV_STDLIB_MICROPYTHON: MicroPython implementation
 * - LV_STDLIB_RTTHREAD:    RT-Thread implementation
 * - LV_STDLIB_CUSTOM:      Implement the functions externally
 */
#define LV_USE_STDLIB_MALLOC    LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_STRING    LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_SPRINTF   LV_STDLIB_BUILTIN

#define LV_STDINT_INCLUDE       <stdint.h>
#define LV_STDDEF_INCLUDE       <stddef.h>
#define LV_STDBOOL_INCLUDE      <stdbool.h>
#define LV_INTTYPES_INCLUDE     <inttypes.h>
#define LV_LIMITS_INCLUDE       <limits.h>
#define LV_STDARG_INCLUDE       <stdarg.h>

#if LV_USE_STDLIB_MALLOC == LV_STDLIB_BUILTIN
    /* 可用于 `lv_malloc()` 的内存大小（字节，>= 2kB） */
    #define LV_MEM_SIZE (64 * 1024U)          /*[字节] 64KB*/

    /* `lv_malloc()` 可扩展的内存大小（字节） */
    #define LV_MEM_POOL_EXPAND_SIZE (32 * 1024U) /*32KB*/

    /* 为内存池设置一个地址，而不是像普通数组那样分配。也可以在外部SRAM中。 */
    #define LV_MEM_ADR 0     /*0：未使用*/
    /* 不用地址，而是提供一个内存分配器函数，LVGL会调用它来获取内存池。例如：my_malloc */
    #if LV_MEM_ADR == 0
        #undef LV_MEM_POOL_INCLUDE
        #undef LV_MEM_POOL_ALLOC
    #endif
#endif  /*LV_USE_STDLIB_MALLOC == LV_STDLIB_BUILTIN*/

/*====================
   HAL SETTINGS
 *====================*/

/* 默认显示刷新、输入设备读取和动画步长周期。 */
#define LV_DEF_REFR_PERIOD  30      /*[毫秒]*/

/* 默认点每英寸。用于初始化默认大小，如小部件大小、样式填充。
 *(不重要，可以调整它来修改默认大小和空间)*/
#define LV_DPI_DEF 130     /*[px/inch]*/

/*=================
 * OPERATING SYSTEM
 *=================*/
/*Select an operating system to use. Possible options:
 * - LV_OS_NONE
 * - LV_OS_PTHREAD
 * - LV_OS_FREERTOS
 * - LV_OS_CMSIS_RTOS2
 * - LV_OS_RTTHREAD
 * - LV_OS_WINDOWS
 * - LV_OS_MQX
 * - LV_OS_CUSTOM */
#define LV_USE_OS   LV_OS_NONE

#if LV_USE_OS == LV_OS_CUSTOM
    #define LV_OS_CUSTOM_INCLUDE <stdint.h>
#endif
#if LV_USE_OS == LV_OS_FREERTOS
	/*
	 * 使用直接通知解除RTOS任务比使用二进制信号量等中间对象要快45%，并且使用的RAM更少。
	 * RTOS任务通知只能在只有一个任务可以接收事件时使用。
	 */
	#define LV_USE_FREERTOS_TASK_NOTIFY 1
#endif

/*========================
 * RENDERING CONFIGURATION
 *========================*/

/* 将所有图层和图像的步幅对齐到此字节 */
#define LV_DRAW_BUF_STRIDE_ALIGN                1

/* 将 draw_buf 地址的起始地址对齐到此字节 */
#define LV_DRAW_BUF_ALIGN                       4

/* 使用矩阵进行变换。
 * 要求：
    `LV_USE_MATRIX = 1`。
    渲染引擎需要支持3x3矩阵变换。*/
#define LV_DRAW_TRANSFORM_USE_MATRIX            0

/* 如果小部件的 `style_opa < 255`（不是 `bg_opa`、`text_opa` 等）或不是 NORMAL 混合模式，
 * 则在渲染之前被缓冲到“简单”图层中。小部件可以分块缓冲。
 * “变换图层”（如果设置了 `transform_angle/zoom`）使用更大的缓冲区，
 * 不能分块绘制。 */

/* 简单图层块的目标缓冲区大小。 */
#define LV_DRAW_LAYER_SIMPLE_BUF_SIZE    (24 * 1024)   /*[bytes]*/

/* 绘制线程的堆栈大小。
 * 注意：如果启用了 FreeType 或 ThorVG，建议设置为 32KB 或更大。
 */
#define LV_DRAW_THREAD_STACK_SIZE    (8 * 1024)   /*[bytes]*/

/* 启用软件绘制引擎，使用CPU进行图形绘制 */
#define LV_USE_DRAW_SW 1
#if LV_USE_DRAW_SW == 1

	/*
	 * 选择性禁用颜色格式支持以减少代码大小。
	 * 注意：某些功能在内部使用特定的颜色格式，例如：
	 * - 渐变使用 RGB888
	 * - 带透明度的位图可能使用 ARGB8888
	 */

	/* 支持RGB565颜色格式（16位，5-6-5位分配，常用于嵌入式系统） */
	#define LV_DRAW_SW_SUPPORT_RGB565		1
	/* 支持RGB565A8颜色格式（16位RGB + 8位透明度） */
	#define LV_DRAW_SW_SUPPORT_RGB565A8		0
	/* 支持RGB888颜色格式（24位真彩色） */
	#define LV_DRAW_SW_SUPPORT_RGB888		0
	/* 支持XRGB8888颜色格式（32位，忽略透明度通道） */
	#define LV_DRAW_SW_SUPPORT_XRGB8888		0
	/* 支持ARGB8888颜色格式（32位，包含透明度通道） */
	#define LV_DRAW_SW_SUPPORT_ARGB8888		0
	/* 支持L8颜色格式（8位灰度） */
	#define LV_DRAW_SW_SUPPORT_L8			0
	/* 支持AL88颜色格式（8位灰度 + 8位透明度） */
	#define LV_DRAW_SW_SUPPORT_AL88			0
	/* 支持A8颜色格式（8位透明度掩码） */
	#define LV_DRAW_SW_SUPPORT_A8			0
	/* 支持I1颜色格式（1位黑白） */
	#define LV_DRAW_SW_SUPPORT_I1			0

	/* 设置绘制单元的数量。
     * > 1 需要在 `LV_USE_OS` 中启用操作系统
     * > 1 意味着多个线程将并行渲染屏幕 */
    #define LV_DRAW_SW_DRAW_UNIT_CNT    1

    /* 使用 Arm-2D 加速软件渲染（ARM Cortex-M系列优化） */
    #define LV_USE_DRAW_ARM2D_SYNC      0

    /* 启用原生Helium汇编编译（ARM Cortex-M55优化） */
    #define LV_USE_NATIVE_HELIUM_ASM    0

    /* 0: 使用简单渲染器，只能绘制简单的矩形、渐变、图像、文本和直线
     * 1: 使用复杂渲染器，能够绘制圆角、阴影、斜线、圆弧等 */
    #define LV_DRAW_SW_COMPLEX          1

    #if LV_DRAW_SW_COMPLEX == 1
        /* 允许缓存一些阴影计算。
        * LV_DRAW_SW_SHADOW_CACHE_SIZE 是要缓存的最大阴影大小，其中阴影大小是 `shadow_width + radius`
        * 缓存有 LV_DRAW_SW_SHADOW_CACHE_SIZE^2 的RAM成本 */
        #define LV_DRAW_SW_SHADOW_CACHE_SIZE 0

        /* 设置最大缓存的圆形数据数量。
        * 保存1/4圆的周长用于抗锯齿
        * 每个圆使用 radius * 4 字节（保存最常用的半径）
        * 0: 禁用缓存 */
        #define LV_DRAW_SW_CIRCLE_CACHE_SIZE 4
    #endif

    /* 设置软件绘制的汇编优化级别 */
    #define  LV_USE_DRAW_SW_ASM     LV_DRAW_SW_ASM_NONE

    #if LV_USE_DRAW_SW_ASM == LV_DRAW_SW_ASM_CUSTOM
        /* 自定义汇编优化的包含文件路径 */
        #define  LV_DRAW_SW_ASM_CUSTOM_INCLUDE ""
    #endif

    /* 启用软件绘制复杂渐变：角度线性渐变、径向渐变或圆锥渐变 */
    #define LV_USE_DRAW_SW_COMPLEX_GRADIENTS    0
#endif

#if 1

/* Use NXP's VG-Lite GPU on iMX RTxxx platforms. */
#define LV_USE_DRAW_VGLITE 0

#if LV_USE_DRAW_VGLITE
    /* Enable blit quality degradation workaround recommended for screen's dimension > 352 pixels. */
    #define LV_USE_VGLITE_BLIT_SPLIT 0

    #if LV_USE_OS
        /* Use additional draw thread for VG-Lite processing.*/
        #define LV_USE_VGLITE_DRAW_THREAD 1

        #if LV_USE_VGLITE_DRAW_THREAD
            /* Enable VGLite draw async. Queue multiple tasks and flash them once to the GPU. */
            #define LV_USE_VGLITE_DRAW_ASYNC 1
        #endif
    #endif

    /* Enable VGLite asserts. */
    #define LV_USE_VGLITE_ASSERT 0
#endif

/* 在 iMX RTxxx 平台上使用 NXP 的 PXP */
#define LV_USE_PXP 0

#if LV_USE_PXP
    /* 使用 PXP 进行绘制 */
    #define LV_USE_DRAW_PXP 1

    /* 使用 PXP 旋转显示器 */
    #define LV_USE_ROTATE_PXP 0

    #if LV_USE_DRAW_PXP && LV_USE_OS
        /* 为 PXP 处理使用额外的绘制线程 */
        #define LV_USE_PXP_DRAW_THREAD 1
    #endif

    /* 启用 PXP 断言 */
    #define LV_USE_PXP_ASSERT 0
#endif

/* 在 RA 平台上使用 Renesas Dave2D */
#define LV_USE_DRAW_DAVE2D 0

/* 使用缓存的 SDL 纹理进行绘制 */
#define LV_USE_DRAW_SDL 0

/* 使用 VG-Lite GPU */
#define LV_USE_DRAW_VG_LITE 0

#if LV_USE_DRAW_VG_LITE
    /* Enable VG-Lite custom external 'gpu_init()' function */
    #define LV_VG_LITE_USE_GPU_INIT 0

    /* Enable VG-Lite assert. */
    #define LV_VG_LITE_USE_ASSERT 0

    /* VG-Lite flush commit trigger threshold. GPU will try to batch these many draw tasks. */
    #define LV_VG_LITE_FLUSH_MAX_COUNT 8

    /* Enable border to simulate shadow
     * NOTE: which usually improves performance,
     * but does not guarantee the same rendering quality as the software. */
    #define LV_VG_LITE_USE_BOX_SHADOW 0

    /* VG-Lite gradient maximum cache number.
     * NOTE: The memory usage of a single gradient image is 4K bytes.
     */
    #define LV_VG_LITE_GRAD_CACHE_CNT 32

    /* VG-Lite stroke maximum cache number.
     */
    #define LV_VG_LITE_STROKE_CACHE_CNT 32

#endif

/*=======================
 * FEATURE CONFIGURATION
 *=======================*/

/*-------------
 * 日志记录
 *-----------*/

/*启用日志模块*/
#define LV_USE_LOG 0
#if LV_USE_LOG

    /*日志重要程度设置：
    *LV_LOG_LEVEL_TRACE       大量日志，提供详细信息
    *LV_LOG_LEVEL_INFO        记录重要事件
    *LV_LOG_LEVEL_WARN        记录不希望发生但未造成问题的事件
    *LV_LOG_LEVEL_ERROR       仅记录关键问题，当系统可能失败时
    *LV_LOG_LEVEL_USER        仅记录用户添加的日志
    *LV_LOG_LEVEL_NONE        不记录任何内容*/
    #define LV_LOG_LEVEL LV_LOG_LEVEL_WARN

    /*1: 使用 'printf' 打印日志；
    *0: 用户需要注册回调函数 `lv_log_register_print_cb()`*/
    #define LV_LOG_PRINTF 0

    /*设置打印日志的回调函数。
     *例如 `my_print`。函数原型应为 `void my_print(lv_log_level_t level, const char * buf)`
     *可通过 `lv_log_register_print_cb` 覆盖*/
    //#define LV_LOG_PRINT_CB

    /*1: 启用打印时间戳；
     *0: 禁用打印时间戳*/
    #define LV_LOG_USE_TIMESTAMP 1

    /*1: 打印日志的文件名和行号；
     *0: 不打印日志的文件名和行号*/
    #define LV_LOG_USE_FILE_LINE 1


    /*在产生大量日志的模块中启用/禁用 LV_LOG_TRACE*/
    #define LV_LOG_TRACE_MEM        1
    #define LV_LOG_TRACE_TIMER      1
    #define LV_LOG_TRACE_INDEV      1
    #define LV_LOG_TRACE_DISP_REFR  1
    #define LV_LOG_TRACE_EVENT      1
    #define LV_LOG_TRACE_OBJ_CREATE 1
    #define LV_LOG_TRACE_LAYOUT     1
    #define LV_LOG_TRACE_ANIM       1
    #define LV_LOG_TRACE_CACHE      1

#endif  /*LV_USE_LOG*/

#endif
/*-------------
 * 断言
 *-----------*/

/*当操作失败或发现无效数据时启用断言。
 *如果启用了 LV_USE_LOG，失败时将打印错误消息*/
#define LV_USE_ASSERT_NULL          1   /*检查参数是否为 NULL。（非常快，推荐）*/
#define LV_USE_ASSERT_MALLOC        1   /*检查内存是否成功分配。（非常快，推荐）*/
#define LV_USE_ASSERT_STYLE         0   /*检查样式是否正确初始化。（非常快，推荐）*/
#define LV_USE_ASSERT_MEM_INTEGRITY 0   /*在关键操作后检查 `lv_mem` 的完整性。（较慢）*/
#define LV_USE_ASSERT_OBJ           0   /*检查对象的类型和存在性（例如未删除）。（较慢）*/

/*当断言发生时添加自定义处理程序，例如重启 MCU*/
#define LV_ASSERT_HANDLER_INCLUDE <stdint.h>
#define LV_ASSERT_HANDLER while(1);   /*默认暂停*/

/*-------------
 * Debug
 *-----------*/

/*1: 在重绘区域上绘制随机颜色的矩形*/
#define LV_USE_REFR_DEBUG 0

/*1: 为 ARGB 图层绘制红色覆盖层，为 RGB 图层绘制绿色覆盖层*/
#define LV_USE_LAYER_DEBUG 0

/*1: 为每个绘制单元的任务绘制不同颜色的覆盖层。
 * 在白色背景上添加绘制单元的索引号。
 * 对于图层，在黑色背景上添加绘制单元的索引号。*/
#define LV_USE_PARALLEL_DRAW_DEBUG 0

/*-------------
 * Others
 *-----------*/

#define LV_ENABLE_GLOBAL_CUSTOM 0
#if LV_ENABLE_GLOBAL_CUSTOM
    /* 包含自定义 'lv_global' 函数的头文件 */
    #define LV_GLOBAL_CUSTOM_INCLUDE <stdint.h>
#endif

/* 默认缓存大小（字节）。
 * 由图像解码器（如 `lv_lodepng`）使用，以在内存中保持解码的图像。
 * 如果大小不为 0，当缓存满时，解码器将失败。
 * 如果大小为 0，缓存功能未启用，解码的内存将在使用后立即释放。*/
#define LV_CACHE_DEF_SIZE       0

/* 默认图像头缓存条目数。缓存用于存储图像的头部。
 * 主要逻辑类似于 `LV_CACHE_DEF_SIZE`，但用于图像头部。*/
#define LV_IMAGE_HEADER_CACHE_DEF_CNT 0

/* 每个渐变允许的停止数。增加此值以允许更多停止。
 * 每个额外停止添加 (sizeof(lv_color_t) + 1) 字节*/
#define LV_GRADIENT_MAX_STOPS   2

/* 调整颜色混合函数的舍入。GPU 可能以不同方式计算颜色混合（混合）。
 * 0: 向下舍入, 64: 从 x.75 向上舍入, 128: 从一半向上舍入, 192: 从 x.25 向上舍入, 254: 向上舍入 */
#define LV_COLOR_MIX_ROUND_OFS  0

/* 为每个 lv_obj_t 添加 2 x 32 位变量，以加速获取样式属性 */
#define LV_OBJ_STYLE_CACHE      0

/* 为 `lv_obj_t` 添加 `id` 字段 */
#define LV_USE_OBJ_ID           0

/* 当对象创建时自动分配 ID */
#define LV_OBJ_ID_AUTO_ASSIGN   LV_USE_OBJ_ID

/* 使用内置对象 ID 处理函数：
* - lv_obj_assign_id:       当小部件创建时调用。使用每个小部件类的单独计数器作为 ID。
* - lv_obj_id_compare:      比较 ID 以决定是否与请求的值匹配。
* - lv_obj_stringify_id:    返回例如 "button3"
* - lv_obj_free_id:         什么也不做，因为没有为 ID 分配内存。
* 当禁用时，这些函数需要由用户实现。*/
#define LV_USE_OBJ_ID_BUILTIN   1

/* 使用对象属性设置/获取 API */
#define LV_USE_OBJ_PROPERTY 0

/* 启用属性名称支持 */
#define LV_USE_OBJ_PROPERTY_NAME 1

/* VG-Lite 模拟器 */
/* 需要：LV_USE_THORVG_INTERNAL 或 LV_USE_THORVG_EXTERNAL */
#define LV_USE_VG_LITE_THORVG  0

#if LV_USE_VG_LITE_THORVG

    /*Enable LVGL's blend mode support*/
    #define LV_VG_LITE_THORVG_LVGL_BLEND_SUPPORT 0

    /*Enable YUV color format support*/
    #define LV_VG_LITE_THORVG_YUV_SUPPORT 0

    /*Enable Linear gradient extension support*/
    #define LV_VG_LITE_THORVG_LINEAR_GRADIENT_EXT_SUPPORT 0

    /*Enable 16 pixels alignment*/
    #define LV_VG_LITE_THORVG_16PIXELS_ALIGN 1

    /*Buffer address alignment*/
    #define LV_VG_LITE_THORVG_BUF_ADDR_ALIGN 64

    /*Enable multi-thread render*/
    #define LV_VG_LITE_THORVG_THREAD_RENDER 0

#endif

/*=====================
 *  COMPILER SETTINGS
 *====================*/

/*For big endian systems set to 1*/
#define LV_BIG_ENDIAN_SYSTEM 0

/*Define a custom attribute to `lv_tick_inc` function*/
#define LV_ATTRIBUTE_TICK_INC

/*Define a custom attribute to `lv_timer_handler` function*/
#define LV_ATTRIBUTE_TIMER_HANDLER

/*Define a custom attribute to `lv_display_flush_ready` function*/
#define LV_ATTRIBUTE_FLUSH_READY

/*缓冲区所需的对齐大小*/
#define LV_ATTRIBUTE_MEM_ALIGN_SIZE 1

/*Will be added where memories needs to be aligned (with -Os data might not be aligned to boundary by default).
 * E.g. __attribute__((aligned(4)))*/
#define LV_ATTRIBUTE_MEM_ALIGN

/*Attribute to mark large constant arrays for example font's bitmaps*/
#define LV_ATTRIBUTE_LARGE_CONST

/*Compiler prefix for a big array declaration in RAM*/
#define LV_ATTRIBUTE_LARGE_RAM_ARRAY

/*Place performance critical functions into a faster memory (e.g RAM)*/
#define LV_ATTRIBUTE_FAST_MEM

/*Export integer constant to binding. This macro is used with constants in the form of LV_<CONST> that
 *should also appear on LVGL binding API such as MicroPython.*/
#define LV_EXPORT_CONST_INT(int_value) struct _silence_gcc_warning /*The default value just prevents GCC warning*/

/*Prefix all global extern data with this*/
#define LV_ATTRIBUTE_EXTERN_DATA

/* Use `float` as `lv_value_precise_t` */
#define LV_USE_FLOAT            0

/*Enable matrix support
 *Requires `LV_USE_FLOAT = 1`*/
#define LV_USE_MATRIX           0

/*Include `lvgl_private.h` in `lvgl.h` to access internal data and functions by default*/
#define LV_USE_PRIVATE_API		0

/*==================
 *   FONT USAGE
 *===================*/

/*Montserrat 字体，使用 ASCII 范围和一些符号，使用 bpp = 4
 * https://fonts.google.com/specimen/Montserrat */
#define LV_FONT_MONTSERRAT_8  0
#define LV_FONT_MONTSERRAT_10 0
#define LV_FONT_MONTSERRAT_12 0
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 0
#define LV_FONT_MONTSERRAT_18 0
#define LV_FONT_MONTSERRAT_20 0
#define LV_FONT_MONTSERRAT_22 0
#define LV_FONT_MONTSERRAT_24 0
#define LV_FONT_MONTSERRAT_26 0
#define LV_FONT_MONTSERRAT_28 0
#define LV_FONT_MONTSERRAT_30 0
#define LV_FONT_MONTSERRAT_32 0
#define LV_FONT_MONTSERRAT_34 0
#define LV_FONT_MONTSERRAT_36 0
#define LV_FONT_MONTSERRAT_38 0
#define LV_FONT_MONTSERRAT_40 0
#define LV_FONT_MONTSERRAT_42 0
#define LV_FONT_MONTSERRAT_44 0
#define LV_FONT_MONTSERRAT_46 0
#define LV_FONT_MONTSERRAT_48 0

/*Demonstrate special features*/
#define LV_FONT_MONTSERRAT_28_COMPRESSED 0  /*bpp = 3*/
#define LV_FONT_DEJAVU_16_PERSIAN_HEBREW 0  /*Hebrew, Arabic, Persian letters and all their forms*/
#define LV_FONT_SIMSUN_14_CJK            0  /*1000 most common CJK radicals*/
#define LV_FONT_SIMSUN_16_CJK            0  /*1000 most common CJK radicals*/

/*Pixel perfect monospace fonts*/
#define LV_FONT_UNSCII_8  0
#define LV_FONT_UNSCII_16 0

/*Optionally declare custom fonts here.
 *You can use these fonts as default font too and they will be available globally.
 *E.g. #define LV_FONT_CUSTOM_DECLARE   LV_FONT_DECLARE(my_font_1) LV_FONT_DECLARE(my_font_2)*/
#define LV_FONT_CUSTOM_DECLARE     LV_FONT_DECLARE(lv_font_biaozhunheiti_16)

/*Always set a default font*/
#define LV_FONT_DEFAULT &lv_font_montserrat_14

/*Enable handling large font and/or fonts with a lot of characters.
 *The limit depends on the font size, font face and bpp.
 *Compiler error will be triggered if a font needs it.*/
#define LV_FONT_FMT_TXT_LARGE 0

/*Enables/disables support for compressed fonts.*/
#define LV_USE_FONT_COMPRESSED 0

/*当找不到字形描述时，启用绘制占位符*/
#define LV_USE_FONT_PLACEHOLDER 1

/*=================
 *  TEXT SETTINGS
 *=================*/

/**
 * 选择字符串的字符编码。
 * 您的 IDE 或编辑器应该有相同的字符编码
 * - LV_TXT_ENC_UTF8
 * - LV_TXT_ENC_ASCII
 */
#define LV_TXT_ENC LV_TXT_ENC_UTF8

/* 可以在这些字符上断开（换行）文本 */
#define LV_TXT_BREAK_CHARS " ,.;:-_)]}"

/*If a word is at least this long, will break wherever "prettiest"
 *To disable, set to a value <= 0*/
#define LV_TXT_LINE_BREAK_LONG_LEN 0

/* 在换行前，一行中长单词的最少字符数。
 * 取决于 LV_TXT_LINE_BREAK_LONG_LEN。*/
#define LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN 3

/* 在换行后，一行中长单词的最少字符数。
 * 取决于 LV_TXT_LINE_BREAK_LONG_LEN。*/
#define LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN 3

/* 支持双向文本。允许混合左到右和右到左文本。
 * 方向将根据 Unicode 双向算法处理：
 * https://www.w3.org/International/articles/inline-bidi-markup/uba-basics */
#define LV_USE_BIDI 0
#if LV_USE_BIDI
    /*Set the default direction. Supported values:
    *`LV_BASE_DIR_LTR` Left-to-Right
    *`LV_BASE_DIR_RTL` Right-to-Left
    *`LV_BASE_DIR_AUTO` detect texts base direction*/
    #define LV_BIDI_BASE_DIR_DEF LV_BASE_DIR_AUTO
#endif

/*Enable Arabic/Persian processing
 *In these languages characters should be replaced with another form based on their position in the text*/
#define LV_USE_ARABIC_PERSIAN_CHARS 0

/*==================
 * WIDGETS
 *================*/

/* 小部件的文档：https://docs.lvgl.io/latest/en/html/widgets/index.html */

#define LV_WIDGETS_HAS_DEFAULT_VALUE  1  // 启用后，部分控件会带有默认初始值

#define LV_USE_ANIMIMG    1      // 动画图片控件

#define LV_USE_ARC        1      // 弧形控件

#define LV_USE_BAR        1      // 进度条控件

#define LV_USE_BUTTON        1   // 按钮控件

#define LV_USE_BUTTONMATRIX  1   // 按钮矩阵控件

#define LV_USE_CALENDAR   1      // 日历控件
#if LV_USE_CALENDAR
    #define LV_CALENDAR_WEEK_STARTS_MONDAY 0
    #if LV_CALENDAR_WEEK_STARTS_MONDAY
        #define LV_CALENDAR_DEFAULT_DAY_NAMES {"Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"}
    #else
        #define LV_CALENDAR_DEFAULT_DAY_NAMES {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"}
    #endif

    #define LV_CALENDAR_DEFAULT_MONTH_NAMES {"January", "February", "March",  "April", "May",  "June", "July", "August", "September", "October", "November", "December"}
    #define LV_USE_CALENDAR_HEADER_ARROW 1
    #define LV_USE_CALENDAR_HEADER_DROPDOWN 1
    #define LV_USE_CALENDAR_CHINESE 1
#endif  /*LV_USE_CALENDAR*/

#define LV_USE_CANVAS     1      // 画布控件

#define LV_USE_CHART      1      // 图表控件

#define LV_USE_CHECKBOX   1      // 复选框控件

#define LV_USE_DROPDOWN   1      // 下拉框控件

#define LV_USE_IMAGE      1      // 图像控件

#define LV_USE_IMAGEBUTTON     1  // 图像按钮控件

#define LV_USE_KEYBOARD   1      // 键盘控件

#define LV_USE_LABEL      1      // 标签控件
#if LV_USE_LABEL
    #define LV_LABEL_TEXT_SELECTION 1 /* 禁用标签文本选择功能 */
    #define LV_LABEL_LONG_TXT_HINT 1  /* 禁用标签中存储一些额外信息，以加快超长文本的绘制速度 */
    #define LV_LABEL_WAIT_CHAR_COUNT 3  /* 禁用等待字符的数量 */
#endif

#define LV_USE_LED        1      // 指示灯控件

#define LV_USE_LINE       1      // 线控件

#define LV_USE_LIST       1      // 列表控件

#define LV_USE_LOTTIE     0      // 动画控件

#define LV_USE_MENU       1      // 菜单控件

#define LV_USE_MSGBOX     1      // 消息框控件

#define LV_USE_ROLLER     1      // 滚动条控件

#define LV_USE_SCALE      1      // 比例尺控件

#define LV_USE_SLIDER     1      // 滑块控件

#define LV_USE_SPAN       1      // 跨度控件
#if LV_USE_SPAN
    /* 一行文本最多可以包含的 span 描述符数量 */
    #define LV_SPAN_SNIPPET_STACK_SIZE 64
#endif

#define LV_USE_SPINBOX    1      // 输入框控件

#define LV_USE_SPINNER    1      // 旋转器控件

#define LV_USE_SWITCH     1      // 开关控件

#define LV_USE_TEXTAREA   1      // 文本区域控件
#if LV_USE_TEXTAREA != 0
    #define LV_TEXTAREA_DEF_PWD_SHOW_TIME 1500    /*ms*/
#endif

#define LV_USE_TABLE      1      // 表格控件

#define LV_USE_TABVIEW    1      // 选项卡视图控件

#define LV_USE_TILEVIEW   1      // 平铺视图控件

#define LV_USE_WIN        1      // 窗口控件

/*==================
 * THEMES
 *==================*/

/*“一个简单、令人印象深刻且非常完整的主题*/
#define LV_USE_THEME_DEFAULT 1
#if LV_USE_THEME_DEFAULT

    /* 0：浅色模式；1：深色模式 */
    #define LV_THEME_DEFAULT_DARK 0

    /* 1：启用按下时放大效果 */
    #define LV_THEME_DEFAULT_GROW 0

    /* 默认过渡时间，单位为毫秒 [ms] */
    #define LV_THEME_DEFAULT_TRANSITION_TIME 80
#endif /*LV_USE_THEME_DEFAULT*/

/* 一个非常简单的主题，适合作为自定义主题的起点 */  //这个主题必须要有，不然会显示不全
#define LV_USE_THEME_SIMPLE 1

/* 一个为单色显示屏设计的主题 */
#define LV_USE_THEME_MONO 0

/*==================
 * LAYOUTS
 *==================*/
#if 1

/*A layout similar to Flexbox in CSS.类似 Flexbox 的布局。*/
#define LV_USE_FLEX 1

/*A layout similar to Grid in CSS.类似 Grid 的布局。*/
#define LV_USE_GRID 1

#endif

/*====================
 * 3RD PARTS LIBRARIES
 *====================*/
#if 1

/* 文件系统接口 */

/* 设置默认驱动器字母允许跳过文件路径中的驱动器前缀 */
#define LV_FS_DEFAULT_DRIVE_LETTER '\0'

/*API for fopen, fread, etc*/
#define LV_USE_FS_STDIO 0
#if LV_USE_FS_STDIO
    #define LV_FS_STDIO_LETTER '\0'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
    #define LV_FS_STDIO_PATH ""         /*Set the working directory. File/directory paths will be appended to it.*/
    #define LV_FS_STDIO_CACHE_SIZE 0    /*>0 to cache this number of bytes in lv_fs_read()*/
#endif

/*API for open, read, etc*/
#define LV_USE_FS_POSIX 0
#if LV_USE_FS_POSIX
    #define LV_FS_POSIX_LETTER '\0'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
    #define LV_FS_POSIX_PATH ""         /*Set the working directory. File/directory paths will be appended to it.*/
    #define LV_FS_POSIX_CACHE_SIZE 0    /*>0 to cache this number of bytes in lv_fs_read()*/
#endif

/*API for CreateFile, ReadFile, etc*/
#define LV_USE_FS_WIN32 0
#if LV_USE_FS_WIN32
    #define LV_FS_WIN32_LETTER '\0'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
    #define LV_FS_WIN32_PATH ""         /*Set the working directory. File/directory paths will be appended to it.*/
    #define LV_FS_WIN32_CACHE_SIZE 0    /*>0 to cache this number of bytes in lv_fs_read()*/
#endif

/*API for FATFS (needs to be added separately). Uses f_open, f_read, etc*/
#define LV_USE_FS_FATFS 0
#if LV_USE_FS_FATFS
    #define LV_FS_FATFS_LETTER '\0'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
    #define LV_FS_FATFS_CACHE_SIZE 0    /*>0 to cache this number of bytes in lv_fs_read()*/
#endif

/*API for memory-mapped file access. */
#define LV_USE_FS_MEMFS 0
#if LV_USE_FS_MEMFS
    #define LV_FS_MEMFS_LETTER '\0'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
#endif

/*API for LittleFs. */
#define LV_USE_FS_LITTLEFS 0
#if LV_USE_FS_LITTLEFS
    #define LV_FS_LITTLEFS_LETTER '\0'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
#endif

/*API for Arduino LittleFs. */
#define LV_USE_FS_ARDUINO_ESP_LITTLEFS 0
#if LV_USE_FS_ARDUINO_ESP_LITTLEFS
    #define LV_FS_ARDUINO_ESP_LITTLEFS_LETTER '\0'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
#endif

/*API for Arduino Sd. */
#define LV_USE_FS_ARDUINO_SD 0
#if LV_USE_FS_ARDUINO_SD
    #define LV_FS_ARDUINO_SD_LETTER '\0'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
#endif

/*LODEPNG decoder library*/
#define LV_USE_LODEPNG 0

/*PNG decoder(libpng) library*/
#define LV_USE_LIBPNG 0

/*BMP decoder library*/
#define LV_USE_BMP 0

/* JPG + split JPG decoder library.
 * Split JPG is a custom format optimized for embedded systems. */
#define LV_USE_TJPGD 0

/* libjpeg-turbo decoder library.
 * Supports complete JPEG specifications and high-performance JPEG decoding. */
#define LV_USE_LIBJPEG_TURBO 0

/*GIF decoder library*/
#define LV_USE_GIF 0
#if LV_USE_GIF
    /*GIF decoder accelerate*/
    #define LV_GIF_CACHE_DECODE_DATA 0
#endif


/*Decode bin images to RAM*/
#define LV_BIN_DECODER_RAM_LOAD 0

/*RLE decompress library*/
#define LV_USE_RLE 0

/*QR code library*/
#define LV_USE_QRCODE 0

/*Barcode code library*/
#define LV_USE_BARCODE 0

/*FreeType library*/
#define LV_USE_FREETYPE 0
#if LV_USE_FREETYPE
    /*Let FreeType to use LVGL memory and file porting*/
    #define LV_FREETYPE_USE_LVGL_PORT 0

    /*Cache count of the glyphs in FreeType. It means the number of glyphs that can be cached.
     *The higher the value, the more memory will be used.*/
    #define LV_FREETYPE_CACHE_FT_GLYPH_CNT 256
#endif

/* Built-in TTF decoder */
#define LV_USE_TINY_TTF 0
#if LV_USE_TINY_TTF
    /* Enable loading TTF data from files */
    #define LV_TINY_TTF_FILE_SUPPORT 0
    #define LV_TINY_TTF_CACHE_GLYPH_CNT 256
#endif

/*Rlottie library*/
#define LV_USE_RLOTTIE 0

/*Enable Vector Graphic APIs
 *Requires `LV_USE_MATRIX = 1`*/
#define LV_USE_VECTOR_GRAPHIC  0

/* Enable ThorVG (vector graphics library) from the src/libs folder */
#define LV_USE_THORVG_INTERNAL 0

/* Enable ThorVG by assuming that its installed and linked to the project */
#define LV_USE_THORVG_EXTERNAL 0

/*Use lvgl built-in LZ4 lib*/
#define LV_USE_LZ4_INTERNAL  0

/*Use external LZ4 library*/
#define LV_USE_LZ4_EXTERNAL  0

/*FFmpeg library for image decoding and playing videos
 *Supports all major image formats so do not enable other image decoder with it*/
#define LV_USE_FFMPEG 0
#if LV_USE_FFMPEG
    /*Dump input information to stderr*/
    #define LV_FFMPEG_DUMP_FORMAT 0
#endif

#endif

/*==================
 * OTHERS
 *==================*/
#if 1

/*1: 启用 API 以获取对象的快照*/
#define LV_USE_SNAPSHOT 0

/*1: 启用系统监视组件*/
#define LV_USE_SYSMON   0
#if LV_USE_SYSMON
    /*Get the idle percentage. E.g. uint32_t my_get_idle(void);*/
    #define LV_SYSMON_GET_IDLE lv_timer_get_idle

    /*1: 显示 CPU 使用率和 FPS 计数
     * 需要 `LV_USE_SYSMON = 1`*/
    #define LV_USE_PERF_MONITOR 1
    #if LV_USE_PERF_MONITOR
        #define LV_USE_PERF_MONITOR_POS LV_ALIGN_BOTTOM_RIGHT

        /*0: 在屏幕上显示性能数据, 1: 使用日志打印性能数据。*/
        #define LV_USE_PERF_MONITOR_LOG_MODE 0
    #endif

    /*1: 显示已使用内存和内存碎片
     * 需要 `LV_USE_STDLIB_MALLOC = LV_STDLIB_BUILTIN`
     * 需要 `LV_USE_SYSMON = 1`*/
    #define LV_USE_MEM_MONITOR 1
    #if LV_USE_MEM_MONITOR
        #define LV_USE_MEM_MONITOR_POS LV_ALIGN_BOTTOM_LEFT
    #endif

#endif /*LV_USE_SYSMON*/

/*1: 启用运行时性能分析器*/
#define LV_USE_PROFILER 0
#if LV_USE_PROFILER
    /*1: 启用内置性能分析器*/
    #define LV_USE_PROFILER_BUILTIN 1
    #if LV_USE_PROFILER_BUILTIN
        /*默认性能分析器跟踪缓冲区大小*/
        #define LV_PROFILER_BUILTIN_BUF_SIZE (16 * 1024)     /*[bytes]*/
    #endif

    /*包含性能分析器的头文件*/
    #define LV_PROFILER_INCLUDE "lvgl/src/misc/lv_profiler_builtin.h"

    /*性能分析器开始点函数*/
    #define LV_PROFILER_BEGIN    LV_PROFILER_BUILTIN_BEGIN

    /*性能分析器结束点函数*/
    #define LV_PROFILER_END      LV_PROFILER_BUILTIN_END

    /*性能分析器开始点函数与自定义标签*/
    #define LV_PROFILER_BEGIN_TAG LV_PROFILER_BUILTIN_BEGIN_TAG

    /*性能分析器结束点函数与自定义标签*/
    #define LV_PROFILER_END_TAG   LV_PROFILER_BUILTIN_END_TAG
#endif

/*1: 启用猴子测试*/
#define LV_USE_MONKEY 0

/*1: 启用网格导航*/
#define LV_USE_GRIDNAV 0

/*1: 启用 lv_obj 片段*/
#define LV_USE_FRAGMENT 0

/*1: 支持使用图像作为标签或跨度小部件的字体 */
#define LV_USE_IMGFONT 0

/*1: 启用观察者模式实现*/
#define LV_USE_OBSERVER 0

/*1: 启用拼音输入方法*/
/* 需要：lv_keyboard */
#define LV_USE_IME_PINYIN 0
#if LV_USE_IME_PINYIN
    /*1: 使用默认同义词*/
    /*If you do not use the default thesaurus, be sure to use `lv_ime_pinyin` after setting the thesaurus*/
    #define LV_IME_PINYIN_USE_DEFAULT_DICT 1
    /*Set the maximum number of candidate panels that can be displayed*/
    /*This needs to be adjusted according to the size of the screen*/
    #define LV_IME_PINYIN_CAND_TEXT_NUM 6

    /*Use 9 key input(k9)*/
    #define LV_IME_PINYIN_USE_K9_MODE      1
    #if LV_IME_PINYIN_USE_K9_MODE == 1
        #define LV_IME_PINYIN_K9_CAND_TEXT_NUM 3
    #endif /*LV_IME_PINYIN_USE_K9_MODE*/
#endif

/*1: 启用文件浏览器*/
/* 需要：lv_table */
#define LV_USE_FILE_EXPLORER                     0
#if LV_USE_FILE_EXPLORER
    /*路径最大长度*/
    #define LV_FILE_EXPLORER_PATH_MAX_LEN        (128)
    /*快速访问栏, 1:使用, 0:不使用*/
    /* 需要：lv_list */
    #define LV_FILE_EXPLORER_QUICK_ACCESS        1
#endif

#endif

/*==================
 * DEVICES
 *==================*/
#if 1
/* 使用 SDL 在 PC 上打开窗口并处理鼠标和键盘 */
#define LV_USE_SDL              0
#if LV_USE_SDL
    #define LV_SDL_INCLUDE_PATH     <SDL2/SDL.h>
    #define LV_SDL_RENDER_MODE      LV_DISPLAY_RENDER_MODE_DIRECT   /*LV_DISPLAY_RENDER_MODE_DIRECT is recommended for best performance*/
    #define LV_SDL_BUF_COUNT        1    /*1 or 2*/
    #define LV_SDL_ACCELERATED      1    /*1: Use hardware acceleration*/
    #define LV_SDL_FULLSCREEN       0    /*1: Make the window full screen by default*/
    #define LV_SDL_DIRECT_EXIT      1    /*1: Exit the application when all SDL windows are closed*/
    #define LV_SDL_MOUSEWHEEL_MODE  LV_SDL_MOUSEWHEEL_MODE_ENCODER  /*LV_SDL_MOUSEWHEEL_MODE_ENCODER/CROWN*/
#endif

/*Use X11 to open window on Linux desktop and handle mouse and keyboard*/
#define LV_USE_X11              0
#if LV_USE_X11
    #define LV_X11_DIRECT_EXIT         1  /*Exit the application when all X11 windows have been closed*/
    #define LV_X11_DOUBLE_BUFFER       1  /*Use double buffers for rendering*/
    /*select only 1 of the following render modes (LV_X11_RENDER_MODE_PARTIAL preferred!)*/
    #define LV_X11_RENDER_MODE_PARTIAL 1  /*Partial render mode (preferred)*/
    #define LV_X11_RENDER_MODE_DIRECT  0  /*direct render mode*/
    #define LV_X11_RENDER_MODE_FULL    0  /*Full render mode*/
#endif

/*Use Wayland to open a window and handle input on Linux or BSD desktops */
#define LV_USE_WAYLAND          0
#if LV_USE_WAYLAND
    #define LV_WAYLAND_WINDOW_DECORATIONS   0    /*Draw client side window decorations only necessary on Mutter/GNOME*/
    #define LV_WAYLAND_WL_SHELL             0    /*Use the legacy wl_shell protocol instead of the default XDG shell*/
#endif

/*Driver for /dev/fb*/
#define LV_USE_LINUX_FBDEV      0
#if LV_USE_LINUX_FBDEV
    #define LV_LINUX_FBDEV_BSD           0
    #define LV_LINUX_FBDEV_RENDER_MODE   LV_DISPLAY_RENDER_MODE_PARTIAL
    #define LV_LINUX_FBDEV_BUFFER_COUNT  0
    #define LV_LINUX_FBDEV_BUFFER_SIZE   60
#endif

/*Use Nuttx to open window and handle touchscreen*/
#define LV_USE_NUTTX    0

#if LV_USE_NUTTX
    #define LV_USE_NUTTX_LIBUV    0

    /*Use Nuttx custom init API to open window and handle touchscreen*/
    #define LV_USE_NUTTX_CUSTOM_INIT    0

    /*Driver for /dev/lcd*/
    #define LV_USE_NUTTX_LCD      0
    #if LV_USE_NUTTX_LCD
        #define LV_NUTTX_LCD_BUFFER_COUNT    0
        #define LV_NUTTX_LCD_BUFFER_SIZE     60
    #endif

    /*Driver for /dev/input*/
    #define LV_USE_NUTTX_TOUCHSCREEN    0

#endif

/*Driver for /dev/dri/card*/
#define LV_USE_LINUX_DRM        0

/*Interface for TFT_eSPI*/
#define LV_USE_TFT_ESPI         0

/*Driver for evdev input devices*/
#define LV_USE_EVDEV    0

/*Driver for libinput input devices*/
#define LV_USE_LIBINPUT    0

#if LV_USE_LIBINPUT
    #define LV_LIBINPUT_BSD    0

    /*Full keyboard support*/
    #define LV_LIBINPUT_XKB             0
    #if LV_LIBINPUT_XKB
        /*"setxkbmap -query" can help find the right values for your keyboard*/
        #define LV_LIBINPUT_XKB_KEY_MAP { .rules = NULL, .model = "pc101", .layout = "us", .variant = NULL, .options = NULL }
    #endif
#endif

/*Drivers for LCD devices connected via SPI/parallel port*/
#define LV_USE_ST7735        0
#define LV_USE_ST7789        0
#define LV_USE_ST7796        0
#define LV_USE_ILI9341       0

#define LV_USE_GENERIC_MIPI (LV_USE_ST7735 | LV_USE_ST7789 | LV_USE_ST7796 | LV_USE_ILI9341)

/*Driver for Renesas GLCD*/
#define LV_USE_RENESAS_GLCDC    0

/* LVGL Windows backend */
#define LV_USE_WINDOWS    0

/* Use OpenGL to open window on PC and handle mouse and keyboard */
#define LV_USE_OPENGLES   0
#if LV_USE_OPENGLES
    #define LV_USE_OPENGLES_DEBUG        1    /* Enable or disable debug for opengles */
#endif

/* QNX Screen display and input drivers */
#define LV_USE_QNX              0
#if LV_USE_QNX
    #define LV_QNX_BUF_COUNT        1    /*1 or 2*/
#endif

#endif

/*==================
* EXAMPLES
*==================*/
#if 1
/* 启用示例以与库一起构建 */
#define LV_BUILD_EXAMPLES 0

/*===================
 * DEMO USAGE
 ====================*/

/* 显示一些控件。可能需要增加 `LV_MEM_SIZE` */
#define LV_USE_DEMO_WIDGETS 0

/* 演示编码器和键盘的使用 */
#define LV_USE_DEMO_KEYPAD_AND_ENCODER 0

/* 基准测试您的系统 */
#define LV_USE_DEMO_BENCHMARK 0

/* 渲染测试每个原语。需要至少 480x272 显示 */
#define LV_USE_DEMO_RENDER 0

/* LVGL 压力测试 */
#define LV_USE_DEMO_STRESS 0

/* 音乐播放器演示 */
#define LV_USE_DEMO_MUSIC 0
#if LV_USE_DEMO_MUSIC
    #define LV_DEMO_MUSIC_SQUARE    0
    #define LV_DEMO_MUSIC_LANDSCAPE 0
    #define LV_DEMO_MUSIC_ROUND     0
    #define LV_DEMO_MUSIC_LARGE     0
    #define LV_DEMO_MUSIC_AUTO_PLAY 0
#endif

/* Flex 布局演示 */
#define LV_USE_DEMO_FLEX_LAYOUT     0

/* 智能手机多语言演示 */
#define LV_USE_DEMO_MULTILANG       0

/* 小部件转换演示 */
#define LV_USE_DEMO_TRANSFORM       0

/* 演示滚动设置 */
#define LV_USE_DEMO_SCROLL          0

/* 矢量图形演示 */
#define LV_USE_DEMO_VECTOR_GRAPHIC  0

/*--END OF LV_CONF_H--*/

#endif /*LV_CONF_H*/

#endif /*End of "Content enable"*/

#endif