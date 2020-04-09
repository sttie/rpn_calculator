#include "../include/stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXINTCHAR 11 


void parse(char* source, char** exit_array, stack_256* operation_stack);
int eval(char** exit_array);
int is_forbidden(char c);
int is_operator(char c);
int is_number(char c);



int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Введите числовое выражение!\n");
        return 0;
    }
	
    // массив выхода
    char* exit_array[256];

    // для всех 256 строк выделяем память под 11 символов
    for (int i = 0; i < 256; i++)
    exit_array[i] = malloc(MAXINTCHAR*sizeof(char));

    // стек операций
    stack_256 operation_stack;
    operation_stack.top = 0;

    parse(argv[1], exit_array, &operation_stack);
    int result = eval(exit_array);
    printf("%d\n", result);

    return 0;
}

void parse(char* source, char** exit_array, stack_256* operation_stack)
{
    // int = 4 байта => максимально возможное число - десятизначное + '\0'
    char buffer[MAXINTCHAR];
memset(buffer, 0, MAXINTCHAR);

    // индекс для buffer
    int buffer_index = 0;
    // индекс для exit_array
    int exit_index = 0;

    // массив приоритетов
    int priors[48];
    priors['+'] = 1;
    priors['-'] = 1;
    priors['*'] = 2;
    priors['/'] = 2;

    // главный цикл
    while (*source)
    {
        // если символ - не буква и не операция, прерываем программу
        if (is_forbidden(*source))
        {
            printf("Invalid character: %c\n", *source);
            exit(1);
        }

        /*
            если символ - пробел или операция, закрываем буфер и изменяем
            exit_array и operation_stack
        */
        if (is_operator(*source))
        {
            // закрываем buffer и обнуляем его индекс
            buffer[buffer_index] = '\0';

            strncpy(exit_array[exit_index], buffer, MAXINTCHAR);
            exit_index++;

            //clear_buffer(buffer, &buffer_index);
			memset(buffer, 0, buffer_index);
			buffer_index = 0;

            // действия с операторами
            // ============================================
            // если стек операций пуст, то просто кладем оператор на стек
            if (get_size(operation_stack) == 0)
                push(*source, operation_stack);

            // если не пуст, то
            else
            {
                if (priors[get_last(operation_stack)] >= priors[*source])
                {
                    char buffer_op[2];
                    buffer_op[0] = pop(operation_stack);
                    buffer_op[1] = '\0';

                    strncpy(exit_array[exit_index], buffer_op, 1);
                    exit_index++;
                }

                push(*source, operation_stack);
            }
		}

        // если символ - число, то обновляем buffer
        else if (is_number(*source))
        {
            buffer[buffer_index] = *source;
            buffer_index++;
        }

        // выполняется всегда, в том числе для пробелов
        source++;
    }

    buffer[buffer_index] = '\0';
    strncpy(exit_array[exit_index], buffer, MAXINTCHAR);
    exit_index++;

    // выталкивание оставшихся операторов в массив выхода
    while (operation_stack->top != 0)
    {
        char buffer_op[2];
        buffer_op[0] = pop(operation_stack);
        buffer_op[1] = '\0';

        strncpy(exit_array[exit_index], buffer_op, 1);
        exit_index++;
    }

    exit_array[exit_index] = "\0";
}


int eval(char** exit_array)
{
    int current_numbers[20]; // можно увеличить
    memset(current_numbers, 0, 20);
    int cn_index = 0;

    int op1, op2 = 0;

    while (**exit_array)
    {
        if (!strcmp(*exit_array, "+") || !strcmp(*exit_array, "-") ||
            !strcmp(*exit_array, "*") || !strcmp(*exit_array, "/"))
        {
            op1 = current_numbers[cn_index-2];
            op2 = current_numbers[cn_index-1];
            cn_index -= 2;

            if (!strcmp(*exit_array, "+"))
				current_numbers[cn_index] = op1 + op2;
            else if (!strcmp(*exit_array, "-"))
				current_numbers[cn_index] = op1 - op2;
            else if (!strcmp(*exit_array, "*"))
				current_numbers[cn_index] = op1 * op2;
            else if (!strcmp(*exit_array, "/"))
				current_numbers[cn_index] = op1 / op2;
        }

        else 
        	current_numbers[cn_index] = atoi(*exit_array);

        cn_index++;
        exit_array++;
    }

    return current_numbers[0];
}

int is_forbidden(char c)
{
    return (c >= '0' && c <= '9') || (c >= '(' && c <= '/') ? 0 : 1;
}

int is_operator(char c)
{
    return c >= '(' && c <= '/' ? 1 : 0;
}

int is_number(char c)
{
	return c >= '0' && c <= '9' ? 1 : 0;
}
