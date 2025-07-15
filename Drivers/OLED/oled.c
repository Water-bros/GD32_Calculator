#include "oled.h"
#include "oledfont.h"
#include "systick.h"

u8 OLED_GRAM[144][4];

void OLED_WR_Byte(u8 dat, u8 mode) {
    /* �ȴ�I2C���߿��� */
    while (i2c_flag_get(I2CX, I2C_FLAG_I2CBSY));

    /* ������ʼ�źŵ�I2C���� */
    i2c_start_on_bus(I2CX);

    /* �ȴ�������ʼ�źųɹ���־λΪ1 */
    while (!i2c_flag_get(I2CX, I2C_FLAG_SBSEND));

    /* ���ʹӻ���ַ��I2C���� */
    i2c_master_addressing(I2CX, I2CX_SLAVE_ADDRESS7, I2C_TRANSMITTER);

    /* �ȴ����ʹӻ���ַ�ɹ���־λΪ1 */
    while (!i2c_flag_get(I2CX, I2C_FLAG_ADDSEND));
    /* ������ʹӻ���ַ�ɹ���־λ */
    i2c_flag_clear(I2CX, I2C_FLAG_ADDSEND);

    /* �ȴ��������ݻ�����Ϊ�� */
    while (SET != i2c_flag_get(I2CX, I2C_FLAG_TBE));

    /* ���ͼĴ�����ַ��������� */
    if (mode) { i2c_data_transmit(I2CX, 0x40); }
    else { i2c_data_transmit(I2CX, 0x00); }

    /* �ȴ��ֽڴ������λ������� */
    while (!i2c_flag_get(I2CX, I2C_FLAG_BTC));

    /* �������� */
    i2c_data_transmit(I2CX, dat);

    /* �ȴ��ֽڴ������λ������� */
    while (!i2c_flag_get(I2CX, I2C_FLAG_BTC));

    /* ��I2C���߷���ֹͣ�ź� */
    i2c_stop_on_bus(I2CX);

    /* �ȴ�ֱ��ֹͣ�źŷ������  */
    while (I2C_CTL0(I2CX) & 0x0200);
}

void OLED_Refresh(void) {
    u8 i, n;
    for (i = 0; i < 8; i++) {
        OLED_WR_Byte(0xb0 + i, OLED_CMD); //��������ʼ��ַ
        OLED_WR_Byte(0x00, OLED_CMD);   //���õ�����ʼ��ַ
        OLED_WR_Byte(0x10, OLED_CMD);   //���ø�����ʼ��ַ

        /* �ȴ�I2C���߿��� */
        while (i2c_flag_get(I2CX, I2C_FLAG_I2CBSY));

        /* ������ʼ�źŵ�I2C���� */
        i2c_start_on_bus(I2CX);

        /* �ȴ�������ʼ�źųɹ���־λΪ1 */
        while (!i2c_flag_get(I2CX, I2C_FLAG_SBSEND));

        /* ���ʹӻ���ַ��I2C���� */
        i2c_master_addressing(I2CX, I2CX_SLAVE_ADDRESS7, I2C_TRANSMITTER);

        /* �ȴ����ʹӻ���ַ�ɹ���־λΪ1 */
        while (!i2c_flag_get(I2CX, I2C_FLAG_ADDSEND));
        /* ������ʹӻ���ַ�ɹ���־λ */
        i2c_flag_clear(I2CX, I2C_FLAG_ADDSEND);

        /* �ȴ��������ݻ�����Ϊ�� */
        while (SET != i2c_flag_get(I2CX, I2C_FLAG_TBE));

        /* ���ͼĴ�����ַ��������� */
        i2c_data_transmit(I2CX, 0x40);

        /* �ȴ��ֽڴ������λ������� */
        while (!i2c_flag_get(I2CX, I2C_FLAG_BTC));

        //����ȫ���Դ�����
        for (n = 0; n < 128; n++) {
            i2c_data_transmit(I2CX, OLED_GRAM[n][i]);
            while (!i2c_flag_get(I2CX, I2C_FLAG_BTC));
        }
        /* ��I2C���߷���ֹͣ�ź� */
        i2c_stop_on_bus(I2CX);

        /* �ȴ�ֱ��ֹͣ�źŷ������  */
        while (I2C_CTL0(I2CX) & 0x0200);
    }
}

void OLED_Clear(void) {
    u8 i, n;
    for (i = 0; i < 4; i++) {
        for (n = 0; n < 128; n++) {
            OLED_GRAM[n][i] = 0;//�����������
        }
    }
    OLED_Refresh();//������ʾ
}

void OLED_DrawPoint(u8 x, u8 y, u8 t) {
    u8 i, m, n;
    i = y / 8;
    m = y % 8;
    n = 1 << m;
    if (t) { OLED_GRAM[x][i] |= n; }
    else {
        OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
        OLED_GRAM[x][i] |= n;
        OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
    }
}

void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size1, u8 mode) {
    u8 i, m, temp, size2, chr1;
    u8 x0 = x, y0 = y;
    if (size1 == 8)size2 = 6;
    else size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2);  //�õ�����һ���ַ���Ӧ������ռ���ֽ���
    chr1 = chr - ' ';
    for (i = 0; i < size2; i++) {
        temp = asc2_1608[chr1][i];
        for (m = 0; m < 8; m++) {
            if (temp & 0x01)OLED_DrawPoint(x, y, mode);
            else OLED_DrawPoint(x, y, !mode);
            temp >>= 1;
            y++;
        }
        x++;
        if ((size1 != 8) && ((x - x0) == size1 / 2)) {
            x = x0;
            y0 = y0 + 8;
        }
        y = y0;
    }
}

void OLED_ShowString(u8 x, u8 y, u8 *chr, u8 size1, u8 mode) {
    while ((*chr >= ' ') && (*chr <= '~'))//�ж��ǲ��ǷǷ��ַ�!
    {
        OLED_ShowChar(x, y, *chr, size1, mode);
        if (size1 == 8)x += 6;
        else x += size1 / 2;
        chr++;
    }
}

void OLED_Init(void) {
    rcu_periph_clock_enable(RCU_LCD_SCL);
    rcu_periph_clock_enable(RCU_LCD_SDA);

    gpio_af_set(PORT_LCD_SCL, GPIO_AF_4, GPIO_LCD_SCL);
    gpio_af_set(PORT_LCD_SDA, GPIO_AF_4, GPIO_LCD_SDA);

    gpio_mode_set(PORT_LCD_SCL, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_LCD_SCL);
    gpio_output_options_set(PORT_LCD_SCL, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_LCD_SCL);
    gpio_mode_set(PORT_LCD_SDA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_LCD_SDA);
    gpio_output_options_set(PORT_LCD_SDA, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_LCD_SDA);

    rcu_periph_clock_enable(RCU_I2CX);
    i2c_deinit(I2CX);
    i2c_clock_config(I2CX, I2C_SPEED, I2C_DTCY_2);
    i2c_mode_addr_config(I2CX, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2CX_SLAVE_ADDRESS7);
    i2c_enable(I2CX);
    i2c_ack_config(I2CX, I2C_ACK_ENABLE);

    delay_1ms(200);

    OLED_WR_Byte(0xAE, OLED_CMD); /*display off*/
    OLED_WR_Byte(0x00, OLED_CMD); /*set lower column address*/
    OLED_WR_Byte(0x10, OLED_CMD); /*set higher column address*/
    OLED_WR_Byte(0x00, OLED_CMD); /*set display start line*/
    OLED_WR_Byte(0xB0, OLED_CMD); /*set page address*/
    OLED_WR_Byte(0x81, OLED_CMD); /*contract control*/
    OLED_WR_Byte(0xff, OLED_CMD); /*128*/
    OLED_WR_Byte(0xA1, OLED_CMD); /*set segment remap*/
    OLED_WR_Byte(0xA6, OLED_CMD); /*normal / reverse*/
    OLED_WR_Byte(0xA8, OLED_CMD); /*multiplex ratio*/
    OLED_WR_Byte(0x1F, OLED_CMD); /*duty = 1/32*/
    OLED_WR_Byte(0xC8, OLED_CMD); /*Com scan direction*/
    OLED_WR_Byte(0xD3, OLED_CMD); /*set display offset*/
    OLED_WR_Byte(0x00, OLED_CMD);
    OLED_WR_Byte(0xD5, OLED_CMD); /*set osc division*/
    OLED_WR_Byte(0x80, OLED_CMD);
    OLED_WR_Byte(0xD9, OLED_CMD); /*set pre-charge period*/
    OLED_WR_Byte(0x1f, OLED_CMD);
    OLED_WR_Byte(0xDA, OLED_CMD); /*set COM pins*/
    OLED_WR_Byte(0x00, OLED_CMD);
    OLED_WR_Byte(0xdb, OLED_CMD); /*set vcomh*/
    OLED_WR_Byte(0x40, OLED_CMD);
    OLED_WR_Byte(0x8d, OLED_CMD); /*set charge pump enable*/
    OLED_WR_Byte(0x14, OLED_CMD);
    OLED_Clear();
    OLED_WR_Byte(0xAF, OLED_CMD); /*display ON*/
}

void OLED_Show(app_t *app) {
    OLED_Clear();
    OLED_ShowString(0, 0, (u8 *) app->oled_text[0], 16, 1);
    OLED_Refresh();
}