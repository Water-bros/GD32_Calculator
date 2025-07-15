#include "main.h"
#include "systick.h"
#include "driver_usart.h"
#include "driver_key.h"
#include "driver_led.h"
#include "usart.h"
#include "key.h"
#include "oled.h"

app_t app;

text cmd_text[CMD_LENGTH] = {
        {"test"},
        {"empty"},
};

void APP_Init() {
    app.state = STATE_INIT;
    app.cmd_text = cmd_text;
    app.cmd = CMD_NONE;
    app.cmd_state = CMD_NONE;
    app.key = 0;

    app.state = STATE_IDLE;
    strcpy(app.oled_text[0], "Calculator Ready!");
    OLED_Show(&app);
}

int main(void) {
    systick_config();
    /* Peripheral Init */
    USART_Init(&app);
    KEY_Init();
    LED_Init();
    OLED_Init();
    APP_Init();

    while (1) {
        Scan_CMD(&app);
        Scan_key(&app);
    }
}

void DMA1_Channel2_IRQHandler(void) {
    if (dma_interrupt_flag_get(DMA1, DMA_CH2, DMA_INT_FLAG_FTF) == SET) {
        dma_interrupt_flag_clear(DMA1, DMA_CH2, DMA_INT_FLAG_FTF);
    }
}

void USART0_IRQHandler(void) {
    if (usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE) == SET) {
        usart_data_receive(USART0);
        app.recv_length = 4096 - dma_transfer_number_get(DMA1, DMA_CH2);
        dma_channel_disable(DMA1, DMA_CH2);
        USART_DMA_Config(&app);
        app.rx_buffer[app.recv_length] = '\0';
        app.recv_finish = 1;
    }
}
