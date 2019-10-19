#ifndef _STACK_H_
#define _STACK_H_

typedef struct {
    char stack[256];
    int top;
} stack_256;

void    push(char value, stack_256* stack);
char    pop(stack_256* stack);
char    get_last(stack_256* stack);
int     get_size(stack_256* stack);

#endif