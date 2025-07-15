#include "oled.h"
#include "usart.h"
#include "driver_led.h"

void Scan_CMD(app_t *app) {
    if ((app->recv_finish && app->recv_length > 0)) {
        app->recv_finish = 0;
        if (app->cmd_state == CMD_NONE) {
            for (uint8_t i = 0; i < CMD_LENGTH; i++) {
                if (strcmp(app->rx_buffer, app->cmd_text[i].text) == 0) {
                    app->cmd = i;
                    break;
                }
            }
        }
        CMD_Handler(app);
    }
}

void CMD_Handler(app_t *app) {
    switch (app->cmd) {
        case CMD_TEST:
        default:
            app->state = STATE_OK;
            printf("接收算式\n\r");
            strcpy(app->oled_text[0], "Input Expression...");
            OLED_Show(app);
//            printf("\n\r%.1f\n\r", result);
            break;
    }
}