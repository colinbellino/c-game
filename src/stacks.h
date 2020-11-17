#include <stdlib.h>
#include <stdio.h>

struct Stack
{
    uint32_t *data;
    size_t capacity;
    size_t top;
    bool empty;
};

Stack *createStack(size_t capacity)
{
    Stack *stack = (Stack *)malloc(sizeof(*stack));
    stack->data = (uint32_t *)malloc(sizeof(*stack->data) * capacity);
    stack->capacity = capacity;
    stack->top = 0;
    stack->empty = true;

    return stack;
}

bool isStackFull(Stack *stack)
{
    return stack->top == stack->capacity;
}

void pushStack(Stack *stack, uint32_t value)
{
    if (isStackFull(stack))
    {
        printf("Failed to push %u, stack is full\n", value);
        return;
    }

    stack->empty = false;
    stack->data[stack->top++] = value;
}

uint32_t popStack(Stack *stack)
{
    if (stack->empty)
    {
        printf("Failed to pop, stack is empty.\n");
        return 0;
    }

    if (stack->top == 1)
    {
        stack->empty = true;
    }
    return stack->data[--stack->top];
}