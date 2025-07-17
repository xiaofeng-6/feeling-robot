#include "atk_md0240_spi.h"


void atk_md0240_spi_init(void)
{
    uapi_pin_set_mode(ATK_MD0240_SPI_SCK_GPIO_PIN,  PIN_MODE_3);
    uapi_pin_set_pull(ATK_MD0240_SPI_SCK_GPIO_PIN, PIN_PULL_TYPE_UP);

    uapi_pin_set_mode(ATK_MD0240_SPI_SDA_GPIO_PIN,  PIN_MODE_3);
    uapi_pin_set_pull(ATK_MD0240_SPI_SDA_GPIO_PIN, PIN_PULL_TYPE_UP);

    spi_attr_t spi_config = {
        .is_slave = false,                                      // ����ģʽ
        .slave_num = 1,                                         // ���豸��ţ�ʹ��Ƭѡ 0
        .bus_clk = 40000000,                                    // spi�������� SPI����ʱ��Ƶ�� ��λHz  40M
        .freq_mhz = 18,                                         // spi ��Ƶֵ
        .clk_polarity = SPI_CFG_CLK_CPOL_1,      // spi���� ʱ�Ӽ���CPOL=1�����иߵ�ƽ��
        .clk_phase = SPI_CFG_CLK_CPHA_1,         // spi��λ ʱ����λCPHA=1���ڶ��ز�����ͨ�����½��أ�
        .frame_format = SPI_CFG_FRAME_FORMAT_MOTOROLA_SPI, // spi Э���ʽ ֡��ʽ����׼Motorola SPIЭ��
        .spi_frame_format = HAL_SPI_FRAME_FORMAT_STANDARD, // spi����ģʽ ��׼spi ��׼֡��ʽ
        .frame_size = HAL_SPI_FRAME_SIZE_8,      // spi ֡��С ���ݿ��8λ
        .tmod = HAL_SPI_TRANS_MODE_TXRX,         // spi ����ģʽ��ȫ˫�����շ�ͬʱ���У�
        .ndf = 0,                                // ����֡������δʹ�ã�ͨ������ֻ��/ֻ��ģʽ��ָ��֡����
        .sste = 0,                               // Ƭѡ�źſ��ƣ�0Ϊ�������NSS
    };
    spi_extra_attr_t extra_attr = {
        .tx_use_dma = true,
        .rx_use_dma = false,
    }; 

    // ��ʼ��SPI
    uapi_spi_init(SPI_BUS_0, &spi_config, &extra_attr);
}


void atk_md0240_spi_send(uint8_t *buf, uint16_t len)
{
    // ��������
    spi_xfer_data_t xfer = {0};
    xfer.tx_buff = buf;
    xfer.tx_bytes = len;
    uapi_spi_master_write(SPI_BUS_0, &xfer, 0xFFFFFFFF);
}
