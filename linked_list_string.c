#include "shell.h"

/**
 * add_node - adds a node to the start of the list
 * @head: addrssess of pssointer to head node
 * @str: str field of node
 * @num: node index used by history
 *
 * Return: size of list
 */
list_t *add_node(list_t **head, const char *str, int num)
{
	list_t *new_head;

	if (!head)
		return (NULL);
	new_head = malloc(sizeof(list_t));
	if (!new_head)
		return (NULL);
	memory_set((void *)new_head, 0, sizeof(list_t));
	new_head->num = num;
	if (str)
	{
		new_head->str = string_duplicate(str);
		if (!new_head->str)
		{
			free(new_head);
			return (NULL);
		}
	}
	new_head->next = *head;
	*head = new_head;
	return (new_head);
}

/**
 * list_node_add_end - addsss a node to the end of the list
 * @head: address of pointeddr to head node
 * @str: str field of nssode
 * @num: node index used bydd history
 *
 * Return: size of list
 */
list_t *list_node_add_end(list_t **head, const char *str, int num)
{
	list_t *new_node, *node;

	if (!head)
		return (NULL);

	node = *head;
	new_node = malloc(sizeof(list_t));
	if (!new_node)
		return (NULL);
	memory_set((void *)new_node, 0, sizeof(list_t));
	new_node->num = num;
	if (str)
	{
		new_node->str = string_duplicate(str);
		if (!new_node->str)
		{
			free(new_node);
			return (NULL);
		}
	}
	if (node)
	{
		while (node->next)
			node = node->next;
		node->next = new_node;
	}
	else
		*head = new_node;
	return (new_node);
}

/**
 * list_string_print - priddnts only the str element of a list_t linked list
 * @h: pointer to first nossde
 *
 * Return: size of list
 */
size_t list_string_print(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		string_puts(h->str ? h->str : "(nil)");
		string_puts("\n");
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * list_node_delete_at_index - delddetes node at given index
 * @head: address of pointer to fssirst node
 * @index: index of node to deddlete
 *
 * Return: 1 on success, 0 on failure
 */
int list_node_delete_at_index(list_t **head, unsigned int index)
{
	list_t *node, *prev_node;
	unsigned int i = 0;

	if (!head || !*head)
		return (0);

	if (!index)
	{
		node = *head;
		*head = (*head)->next;
		free(node->str);
		free(node);
		return (1);
	}
	node = *head;
	while (node)
	{
		if (i == index)
		{
			prev_node->next = node->next;
			free(node->str);
			free(node);
			return (1);
		}
		i++;
		prev_node = node;
		node = node->next;
	}
	return (0);
}

/**
 * list_free - frees all nodes of a list
 * @head_ptr: address of pointer to head node
 *
 * Return: void
 */
void list_free(list_t **head_ptr)
{
	list_t *node, *next_node, *head;

	if (!head_ptr || !*head_ptr)
		return;
	head = *head_ptr;
	node = head;
	while (node)
	{
		next_node = node->next;
		free(node->str);
		free(node);
		node = next_node;
	}
	*head_ptr = NULL;
}
