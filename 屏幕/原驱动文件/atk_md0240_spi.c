/**
 ****************************************************************************************************
 * @file        atk_md0240_spi.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-06-21
 * @brief       ATK-MD0240ģ��SPI�ӿ���������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ������ F429������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./BSP/ATK_MD0240/atk_md0240_spi.h"

static SPI_HandleTypeDef g_spi_handle = {0};

/**
 * @brief       ATK-MD0240ģ��SPI�ӿڳ�ʼ��
 * @param       ��
 * @retval      ��
 */
void atk_md0240_spi_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    
    /* ʹ��ʱ�� */
    ATK_MD0240_SPI_CLK_ENABLE();
    ATK_MD0240_SPI_SCK_GPIO_CLK_ENABLE();
    ATK_MD0240_SPI_SDA_GPIO_CLK_ENABLE();
    
    /* ��ʼ��SCK���� */
    gpio_init_struct.Pin        = ATK_MD0240_SPI_SCK_GPIO_PIN;
    gpio_init_struct.Mode       = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull       = GPIO_PULLUP;
    gpio_init_struct.Speed      = GPIO_SPEED_FREQ_HIGH;
    gpio_init_struct.Alternate  = ATK_MD0240_SPI_SCK_GPIO_AF;
    HAL_GPIO_Init(ATK_MD0240_SPI_SCK_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��SDA���� */
    gpio_init_struct.Pin        = ATK_MD0240_SPI_SDA_GPIO_PIN;
    gpio_init_struct.Mode       = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull       = GPIO_PULLUP;
    gpio_init_struct.Speed      = GPIO_SPEED_FREQ_HIGH;
    gpio_init_struct.Alternate  = ATK_MD0240_SPI_SDA_GPIO_AF;
    HAL_GPIO_Init(ATK_MD0240_SPI_SDA_GPIO_PORT, &gpio_init_struct);
    
    /* ��ʼ��SPI */
    g_spi_handle.Instance               = ATK_MD0240_SPI_INTERFACE;
    g_spi_handle.Init.Mode              = SPI_MODE_MASTER;
    g_spi_handle.Init.Direction         = SPI_DIRECTION_2LINES;
    g_spi_handle.Init.DataSize          = SPI_DATASIZE_8BIT;
    g_spi_handle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
    g_spi_handle.Init.CLKPhase          = SPI_PHASE_2EDGE;
    g_spi_handle.Init.NSS               = SPI_NSS_SOFT;
    g_spi_handle.Init.BaudRatePrescaler = ATK_MD0240_SPI_PRESCALER;
    g_spi_handle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    g_spi_handle.Init.TIMode            = SPI_TIMODE_DISABLE;
    g_spi_handle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    g_spi_handle.Init.CRCPolynomial     = 10;
    HAL_SPI_Init(&g_spi_handle);
}

/**
 * @brief       ATK-MD0240ģ��SPI�ӿڷ�������
 * @param       ��
 * @retval      ��
 */
void atk_md0240_spi_send(uint8_t *buf, uint16_t len)
{
    HAL_SPI_Transmit(&g_spi_handle, buf, len, HAL_MAX_DELAY);
}
