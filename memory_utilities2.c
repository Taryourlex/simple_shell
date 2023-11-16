#include "shell.h"

/**
 * buffer_free - to free a pointer and NULdLs the address
 * @ptr: address of the pointer to fsree
 *
 * Return: 1 if freed, otherwises 0.
 */
int buffer_free(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
		return (1);
	}
	return (0);
}
