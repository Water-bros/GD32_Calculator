#ifndef __MAIN_H
#define __MAIN_H

enum {
    STATE_INIT,
    STATE_IDLE,
    STATE_EMPTY,
    STATE_OK,
    STATE_ERROR,
    STATE_LENGTH
};

enum {
    CMD_TEST,
    CMD_NONE,
    CMD_LENGTH
};

typedef struct {
    char *text;
} text;

typedef struct {
    text *cmd_text;
    char oled_text[1][32];
    char state;
    char cmd_state;
    char cmd;
    char key;
    int recv_length;
    char rx_buffer[4096];
    char recv_finish;
} app_t;


#endif /* __MAIN_H */


