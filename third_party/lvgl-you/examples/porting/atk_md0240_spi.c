#include "atk_md0240_spi.h"


void atk_md0240_spi_init(void)
{
    uapi_pin_set_mode(ATK_MD0240_SPI_SCK_GPIO_PIN,  PIN_MODE_3);
    uapi_pin_set_pull(ATK_MD0240_SPI_SCK_GPIO_PIN, PIN_PULL_TYPE_UP);

    uapi_pin_set_mode(ATK_MD0240_SPI_SDA_GPIO_PIN,  PIN_MODE_3);
    uapi_pin_set_pull(ATK_MD0240_SPI_SDA_GPIO_PIN, PIN_PULL_TYPE_UP);

    spi_attr_t spi_config = {
        .is_slave = false,                                      // 主机模式
        .slave_num = 1,                                         // 从设备编号（使用片选 0
        .bus_clk = 40000000,                                    // spi传输速率 SPI总线时钟频率 单位Hz  40M
        .freq_mhz = 18,                                         // spi 分频值
        .clk_polarity = SPI_CFG_CLK_CPOL_1,      // spi极性 时钟极性CPOL=1（空闲高电平）
        .clk_phase = SPI_CFG_CLK_CPHA_1,         // spi相位 时钟相位CPHA=1（第二沿采样，通常是下降沿）
        .frame_format = SPI_CFG_FRAME_FORMAT_MOTOROLA_SPI, // spi 协议格式 帧格式，标准Motorola SPI协议
        .spi_frame_format = HAL_SPI_FRAME_FORMAT_STANDARD, // spi传输模式 标准spi 标准帧格式
        .frame_size = HAL_SPI_FRAME_SIZE_8,      // spi 帧大小 数据宽度8位
        .tmod = HAL_SPI_TRANS_MODE_TXRX,         // spi 传输模式：全双工（收发同时进行）
        .ndf = 0,                                // 数据帧数量（未使用，通常用于只收/只发模式下指定帧数）
        .sste = 0,                               // 片选信号控制，0为软件控制NSS
    };
    spi_extra_attr_t extra_attr = {
        .tx_use_dma = true,
        .rx_use_dma = false,
    }; 

    // 初始化SPI
    uapi_spi_init(SPI_BUS_0, &spi_config, &extra_attr);
}


void atk_md0240_spi_send(uint8_t *buf, uint16_t len)
{
    // 发送数据
    spi_xfer_data_t xfer = {0};
    xfer.tx_buff = buf;
    xfer.tx_bytes = len;
    uapi_spi_master_write(SPI_BUS_0, &xfer, 0xFFFFFFFF);
}
