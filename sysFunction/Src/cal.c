#include "cal.h"

typedef union {
    float f;
    uint32_t i;
} convertor;

typedef struct {
    uint32_t data[128];
    uint32_t top;
    uint32_t bottom;
} stack;

static uint32_t stack_pop(stack *s) {
    if (s->top == s->bottom) {
        return 0;
    }
    return s->data[s->top--];
}

static void stack_push(stack *s, uint32_t data) {
    if (s->top < 127) {
        s->data[++s->top] = data;
    }else{
        return; // Stack Overflow
    }
}

static void stack_clear(stack *s) {
    s->top = s->bottom;
}

static void stack_init(stack *s) {
    s->top = s->bottom = 0;
}

static uint8_t is_operator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

static uint8_t is_number(char c) {
    return (c >= '0' && c <= '9') || (c == '.');
}

static int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

static void infix_to_RPN(char *infix, char *RPN) {
    stack s;
    stack_init(&s);
    uint32_t len = strlen(infix);
    uint32_t rpn_index = 0;

    for (int i = 0; i < len; i++) {
        if (infix[i] == ' ') {
            continue;
        } else if (is_number(infix[i])) {
            while (i < len && (is_number(infix[i]) || infix[i] == '.')) {
                RPN[rpn_index++] = infix[i++];
            }
            i--;
            RPN[rpn_index++] = ' ';
        } else if (infix[i] == '(') {
            stack_push(&s, '(');
        } else if (infix[i] == ')') {
            while (s.top != s.bottom && s.data[s.top] != '(') {
                RPN[rpn_index++] = stack_pop(&s);
                RPN[rpn_index++] = ' ';
            }
            if (s.top != s.bottom && s.data[s.top] == '(') {
                stack_pop(&s);
            } else {
                strcpy(RPN, "Error: 缺少括号");
                return;
            }
        } else if (is_operator(infix[i])) {
            while (s.top != s.bottom && is_operator(s.data[s.top]) &&
                   precedence(s.data[s.top]) >= precedence(infix[i])) {
                RPN[rpn_index++] = stack_pop(&s);
                RPN[rpn_index++] = ' ';
            }
            stack_push(&s, infix[i]);
        } else {
            strcpy(RPN, "Error: Invalid Expression");
            return;
        }
    }

    while (s.top != s.bottom) {
        if (s.data[s.top] == '(') {
            strcpy(RPN, "Error: Mismatched Parentheses");
            return;
        }
        RPN[rpn_index++] = stack_pop(&s);
        RPN[rpn_index++] = ' ';
    }
    RPN[rpn_index] = '\0';
}

static float evaluate_RPN(char *RPN) {
    if (strstr(RPN, "Error")) {
        return NAN;
    }

    stack s;
    stack_init(&s);
    uint32_t len = strlen(RPN);
    convertor c1, c2, c3;

    for (int i = 0; i < len; i++) {
        if (is_number(RPN[i])) {
            char num_str[32];
            int num_index = 0;

            while (i < len && (is_number(RPN[i]) || RPN[i] == '.')) {
                num_str[num_index++] = RPN[i++];
            }
            num_str[num_index] = '\0';
            i--;

            c3.f = strtof(num_str, NULL);
            stack_push(&s, c3.i);
        } else if (is_operator(RPN[i])) {
            c1.i = stack_pop(&s);
            c2.i = stack_pop(&s);
            float a = c1.f, b = c2.f;

            switch (RPN[i]) {
                case '+':
                    c3.f = b + a;
                    break;
                case '-':
                    c3.f = b - a;
                    break;
                case '*':
                    c3.f = b * a;
                    break;
                case '/':
                    if (a == 0) {
                        strcpy(RPN, "Error: Division by zero");
                        return NAN;
                    }
                    c3.f = b / a;
                    break;
            }
            stack_push(&s, c3.i);
        }
    }

    c3.i = stack_pop(&s);
    return c3.f;
}

float eval(char *infix,char *RPN) {
    infix_to_RPN(infix, RPN);
    return evaluate_RPN(RPN);
}