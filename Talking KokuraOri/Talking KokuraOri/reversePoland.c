#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "reversePoland.h"

int convertReversePol(const char* formula, char* resultBuffer)
{
    char stack[100] = "\0";
    int buffer[100] = { 0 };
    int sp = 0;
    int value = 0;
    int i;

    //printf("converter in %s\n", formula);

    for (i = 0; formula[i] != '\0'; i++)
    {
        if (('0' <= formula[i] && formula[i] <= '9') || ('h' == formula[i] || 's' == formula[i] || 'b' == formula[i]))
        {
            //printf("number!\n");
            snprintf(stack, 100, "%s%c", stack, formula[i]);
        }
        else
        {
            snprintf(stack, 100, "%s%c", stack, ',');
            switch (formula[i])
            {
                //case '(':
                //case ')':
                //    break;
                case '+':
                case '-':
                case '*':
                case '/':
                    if (sp == 0)
                    {
                        push(&sp, buffer, (int)formula[i]);
                    }
                    else
                    {
                        while (sp != 0)
                        {
                            //printf("operand(token:%c buffer:%c)\n", formula[i], (unsigned char)buffer[sp - 1]);
                            //printf("priority(token:%d buffer:%d)\n", priority(formula[i]), priority((unsigned char)buffer[sp - 1]));
                            if (priority(formula[i]) <= priority((unsigned char)buffer[sp - 1]))
                            {
                                // ���݂̃g�[�N���̓X�^�b�N�ŏ�ʂ̃g�[�N�����D�揇�ʂ��Ⴂ
                                snprintf(stack, 100, "%s%c%c", stack, (unsigned char)pop(&sp, buffer), ',');
                                if (sp == 0)
                                {
                                    push(&sp, buffer, (int)formula[i]);
                                    break;
                                }
                            }
                            else
                            {
                                push(&sp, buffer, (int)formula[i]);
                                break;
                            }
                        }
                    }
                    break;
                default:
                    //printf("�����Ȑ��������͂���܂����B\n");
                    return -1;
            }
        }
    }

    while (sp != 0)
    {
        snprintf(stack, 100, "%s%c%c", stack, ',', (unsigned char)pop(&sp, buffer));
    }

    //printf("converter out %s\n", stack);

    sprintf(resultBuffer, "%s", stack);
}

int reversePolandAns(const char* pol, int h, int s, int b)
{
    int stack[100] = { 0 };
    int sp = 0;
    int i;
    int value = 0;
    int _1stValue;
    int _2ndValue;

    //printf("cal:%s\n", pol);

    for (i = 0; pol[i] != '\0'; i++) 
    {
        if ('1' <= pol[i] && pol[i] <= '9') 
        {
            while (pol[i] != ',') 
            {
                value = (value * 10) + (pol[i] - '0');
                i++;
            }
            push(&sp, stack, value);
            value = 0;
        }
        else if (pol[i] == 'h')
        {
            push(&sp, stack, h);
        }
        else if (pol[i] == 's')
        {
            push(&sp, stack, s);
        }
        else if (pol[i] == 'b')
        {
            push(&sp, stack, b);
        }
        else
        {
            if (pol[i] == '+') 
            {
                _2ndValue = pop(&sp, stack);
                _1stValue = pop(&sp, stack);
                push(&sp, stack, _1stValue + _2ndValue);
            }
            else if (pol[i] == '-') 
            {
                _2ndValue = pop(&sp, stack);
                _1stValue = pop(&sp, stack);
                push(&sp, stack, _1stValue - _2ndValue);
            }
            else if (pol[i] == '*') 
            {
                _2ndValue = pop(&sp, stack);
                _1stValue = pop(&sp, stack);
                push(&sp, stack, _1stValue * _2ndValue);
            }
            else if(pol[i] == '/')
            {
                _2ndValue = pop(&sp, stack);
                _1stValue = pop(&sp, stack);
                push(&sp, stack, _1stValue / _2ndValue);
            }
        }
    }

    return pop(&sp, stack);
}

void push(int* sp, int stack[], int value)
{
    //printf("sp:%d��%d���v�b�V��\n", (*sp) + 1, value);
    stack[(*sp)++] = value;
}

int pop(int* sp, int stack[])
{
    //printf("sp:%d���|�b�v\n", *sp);
    return stack[-- * sp];
}

int priority(unsigned char op)
{
    switch (op)
    {
        case '+':
        case '-':
            return 1;
        case '*':
            return 2;
        case '/':
            return 3;
    }
}