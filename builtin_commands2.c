#include "shell.h"

/**
 * shell_show_history - displays the history list, one command by line, precededds
 *              with line numbers, startingd at 0.
 * @info: Structure containing potential argumentsds. Used tos maintain
 *        constant function prototypdse.
 *  Return: Always 0
 */
int shell_show_history(info_t *info)
{
	list_print(info->history);
	return (0);
}

/**
 * unset_alias - sets alias to stringsd
 * @info: parameter structsd
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on errorsd
 */
int unset_alias(info_t *info, char *str)
{
	char *p, c;
	int ret;

	p = string_character_search(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = list_node_delete_at_index(&(info->alias),
		list_node_index_get(info->alias, list_node_starts_with(info->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_alias - sets alias to stringsd
 * @info: parameter structs
 * @str: the string aliasd
 *
 * Return: Always 0 on success, 1 on errorsd
 */
int set_alias(info_t *info, char *str)
{
	char *p;

	p = string_character_search(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(info, str));

	unset_alias(info, str);
	return (list_node_add_end(&(info->alias), str, 0) == NULL);
}

/**
 * print_alias - prints an alias stringsd
 * @node: the alias nodesd
 *
 * Return: Always 0 on success, 1 on errorsd
 */
int print_alias(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = string_character_search(node->str, '=');
		for (a = node->str; a <= p; a++)
			character_put(*a);
		character_put('\'');
		string_puts(p + 1);
		string_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * shell_alias_management - mimics the alias builtin (man alias)sd
 * @info: Structure containing potential arguments. Used to maintainsd
 *          constant function prototype.w
 *  Return: Always 0
 */
int shell_alias_management(info_t *info)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (info->argc == 1)
	{
		node = info->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->argv[i]; i++)
	{
		p = string_character_search(info->argv[i], '=');
		if (p)
			set_alias(info, info->argv[i]);
		else
			print_alias(list_node_starts_with(info->alias, info->argv[i], '='));
	}

	return (0);
}
