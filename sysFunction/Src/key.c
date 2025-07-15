#include "key.h"
#include "cal.h"
#include "systick.h"
#include "usart.h"
#include "oled.h"

char str[128];

float result;

const uint32_t KEY_PINs[6] = {
        KEY1_PIN,
        KEY2_PIN,
        KEY3_PIN,
        KEY4_PIN,
        KEY5_PIN,
        KEY6_PIN
};

void Scan_key(app_t *app) {
    for (uint8_t i = 0; i < 6; i++) {
        if (gpio_input_bit_get(KEY_PORT, KEY_PINs[i]) == SET) {
            delay_1ms(100);
            if (gpio_input_bit_get(KEY_PORT, KEY_PINs[i]) == SET) {
                app->key = i + 1;
            }
        }
    }
    switch (app->key) {
        case 1:
            switch (app->state) {
                case STATE_OK:
                    result = eval(app->rx_buffer, str);

                    if (strstr(str, "Error") != NULL) {
                        printf("%s\n\r", str);
                        app->state = STATE_ERROR;
                        strcpy(app->oled_text[0], "CalError!");
                    } else {
                        printf("计算完成\n\r");
                        strcpy(app->oled_text[0], "Done!");
                    }

                    OLED_Show(app);
                    break;
                case STATE_ERROR:
                    app->state = STATE_EMPTY;
                case STATE_EMPTY:
                    printf("请输入计算式\n\r");
                    sprintf(app->oled_text[0], "Input required!");
                    OLED_Show(app);
                    break;
            }
            break;
        case 2:
            switch (app->state) {
                case STATE_OK:
                    printf("%.1f\n\r", result);
                    sprintf(app->oled_text[0], "Result: %.1f", result);
                    OLED_Show(app);
                    break;
                case STATE_ERROR:
                    app->state = STATE_EMPTY;
                case STATE_EMPTY:
                    printf("请输入计算式\n\r");
                    sprintf(app->oled_text[0], "Input required!");
                    OLED_Show(app);
                    break;
            }
            break;
        case 3:
            printf("清空\n\r");
            app->state = STATE_EMPTY;
            strcpy(app->oled_text[0], "Clear!");
            OLED_Show(app);
            break;
    }
    app->key = 0;
}