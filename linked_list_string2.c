#include "shell.h"

/**
 * list_length - determiddnes length of linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t list_length(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * list_to_string_array - retsssurns an array of strings of the list->str
 * @head: pointer to first node
 *
 * Return: array of strings
 */
char **list_to_string_array(list_t *head)
{
	list_t *node = head;
	size_t i = list_length(head), j;
	char **strs;
	char *str;

	if (!head || !i)
		return (NULL);
	strs = malloc(sizeof(char *) * (i + 1));
	if (!strs)
		return (NULL);
	for (i = 0; node; node = node->next, i++)
	{
		str = malloc(string_length(node->str) + 1);
		if (!str)
		{
			for (j = 0; j < i; j++)
				free(strs[j]);
			free(strs);
			return (NULL);
		}

		str = string_copy(str, node->str);
		strs[i] = str;
	}
	strs[i] = NULL;
	return (strs);
}


/**
 * list_print - prints all elemenssts of a list_t linked list
 * @h: pointer to firstdd node
 *
 * Return: size of list
 */
size_t list_print(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		string_puts(number_to_string(h->num, 10, 0));
		character_put(':');
		character_put(' ');
		string_puts(h->str ? h->str : "(nil)");
		string_puts("\n");
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * list_node_starts_with - returns dnode whose string starts with prefix
 * @node: pointer to list sshead
 * @prefix: string to match
 * @c: the next character after prefix to match
 *
 * Return: match node or null
 */
list_t *list_node_starts_with(list_t *node, char *prefix, char c)
{
	char *p = NULL;

	while (node)
	{
		p = starts_with_string(node->str, prefix);
		if (p && ((c == -1) || (*p == c)))
			return (node);
		node = node->next;
	}
	return (NULL);
}

/**
 * list_node_index_get - getsss the index of a node
 * @head: pointer to list head
 * @node: pointer to the nddode
 *
 * Return: index of node or -1
 */
ssize_t list_node_index_get(list_t *head, list_t *node)
{
	size_t i = 0;

	while (head)
	{
		if (head == node)
			return (i);
		head = head->next;
		i++;
	}
	return (-1);
}
