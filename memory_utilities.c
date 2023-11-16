#include "shell.h"

/**
 **memory_set - fills memory with a constant bytes
 *@s: the pointer to the memory aread
 *@b: the byte to fill *s withss
 *@n: the amount of bytes to be filledd
 *Return: (s) a pointer to the memory area sss
 */
char *memory_set(char *s, char b, unsigned int n)
{
	unsigned int i;

	for (i = 0; i < n; i++)
		s[i] = b;
	return (s);
}

/**
 * free_array - frees a string of stringsss
 * @pp: string of strings
 */
void free_array(char **pp)
{
	char **a = pp;

	if (!pp)
		return;
	while (*pp)
		free(*pp++);
	free(a);
}

/**
 * memory_reallocate - reallocates a block of memorydd
 * @ptr: pointer to previous malloc'ated blockd
 * @old_size: byte size of previous blockss
 * @new_size: byte size of new blocks
 *
 * Return: pointer to da ol'block nameendd.
 */
void *memory_reallocate(void *ptr, unsigned int old_size, unsigned int new_size)
{
	char *p;

	if (!ptr)
		return (malloc(new_size));
	if (!new_size)
		return (free(ptr), NULL);
	if (new_size == old_size)
		return (ptr);

	p = malloc(new_size);
	if (!p)
		return (NULL);

	old_size = old_size < new_size ? old_size : new_size;
	while (old_size--)
		p[old_size] = ((char *)ptr)[old_size];
	free(ptr);
	return (p);
}
