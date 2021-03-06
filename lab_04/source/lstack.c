#include "lstack.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "error_handle.h"


int lstack_init(lstack_t *stack, long elem)
{
	int err = OK;
	if (*stack)
		free(*stack);
	*stack = malloc(sizeof(node_t));
	if (*stack)
	{
		(*stack)->elem = elem;
		(*stack)->next = NULL;
	}
	else
		err = MALLOC_ERROR;
	return err;
}

int lstack_push(lstack_t *stack, long elem)
{
	int err = OK;
	lstack_t new_node = malloc(sizeof(node_t));
	if (new_node)
	{
		new_node->next = *stack;
		new_node->elem = elem;
		*stack = new_node;
	}
	else
		err = MALLOC_ERROR;
	return err;
}

int lstack_pop(lstack_t *stack, long *elem, lstack_t *freed)
{
	int err = OK;
	lstack_t buf = NULL;
	if (*stack)
	{
		*elem = (*stack)->elem;

		buf = *stack;
		if ((*stack)->next)
			*stack = (*stack)->next;
		else
			*stack = NULL;
		*freed = buf;
		free(buf);
	}
	else
		err = EMPTY_STACK;
	return err;
}

int lstack_print(lstack_t *stack)
{
	/*
	long elem;
	int err = OK;
	lstack_t new_stack = NULL;
	lstack_t free_buf;
	
	// read and output elements from stack and push them to buffer stack
	// (because of stack it gets reverse)
	if (!*stack)
		puts("Empty stack");
		
	while (!err && *stack)
	{
		err = lstack_pop(stack, &elem, &free_buf);
		if (!err)
		{
			printf("%ld ", elem);
			err = lstack_push(&new_stack, elem);
		}
	}
	printf("\n");

	// push elements to source stack to make order like source
	while (!err && new_stack)
	{
		err = lstack_pop(&new_stack, &elem, &free_buf);
		if (!err)
			err = lstack_push(stack, elem);
	}

	return err;
	*/
	lstack_t new_stack = *stack;
	if (!(*stack))
		return EMPTY_STACK;
	while (new_stack)
	{
		printf("%p: %ld\n", (void *)new_stack, new_stack->elem);
		new_stack = new_stack->next;
	}
	return OK;
}

int lstack_print_decreasing(lstack_t *stack)
{
	long elem, prev = LONG_MAX;
	int err = OK, start = 1, empty = 1;
	lstack_t new_stack = NULL;
	lstack_t free_buf;

	while (!err && *stack)
	{
		err = lstack_pop(stack, &elem, &free_buf);
		if (!err)
		{
			if (prev < elem)
			{
				if (start)
				{
					printf("\n%ld ", prev);
					start = 0;
				}
				printf("%ld ", elem);
			}
			else
				start = 1;
			prev = elem;
			err = lstack_push(&new_stack, elem);
		}
	}
	printf("\n");

	if (!empty && err == EMPTY_STACK)
		err = OK;
	
	while (!err && new_stack)
	{
		err = lstack_pop(&new_stack, &elem, &free_buf);
		if (!err)
		{
			err = lstack_push(stack, elem);
			empty = 0;
		}
	}

	if (!empty && err == EMPTY_STACK)
		err = OK;

	return err;
}

int lstack_delete(lstack_t *stack)
{
	int err;
	long buf = 0;
	lstack_t lbuf = NULL;
	err = lstack_pop(stack, &buf, &lbuf);
	if (!err)
	{
		while (!err)
			err = lstack_pop(stack, &buf, &lbuf);
		if (err == EMPTY_STACK)
			err = OK;
	}
	return err;
}
