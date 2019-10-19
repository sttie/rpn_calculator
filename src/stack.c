#include "../include/stack.h"

void push(char value, stack_256* stack) {
    stack->stack[stack->top] = value;
    stack->top++;
}

char pop(stack_256* stack) {
    if (stack->top == 0) {
        return -1;
    }

    stack->top--;
    return stack->stack[stack->top];
}

char get_last(stack_256* stack) {
    return stack->stack[stack->top-1];
}

int get_size(stack_256* stack) {
    return stack->top;
}
