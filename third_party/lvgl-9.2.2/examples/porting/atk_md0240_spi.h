
#ifndef __ATK_MD0240_SPI_H
#define __ATK_MD0240_SPI_H

#include "pinctrl.h"
#include "spi.h"

/* SPI接口定义 */
#define ATK_MD0240_SPI_SCK_GPIO_PIN             7  // GPIO07 (SCK)
#define ATK_MD0240_SPI_SDA_GPIO_PIN             9  // GPIO09 (SDA)


/* 操作函数 */
void atk_md0240_spi_init(void);                         /* ATK-MD0240模块SPI接口初始化 */
void atk_md0240_spi_send(uint8_t *buf, uint16_t len);   /* ATK-MD0240模块SPI接口发送数据 */

#endif
