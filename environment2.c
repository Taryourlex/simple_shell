#include "shell.h"

/**
 * environment_get - returns the string array copy of our environsd
 * @info: Structure containing potential arguments. Used to maintainss
 *          constant function prototype.dd
 * Return: Always 0
 */
char **environment_get(info_t *info)
{
	if (!info->environ || info->env_changed)
	{
		info->environ = list_to_string_array(info->env);
		info->env_changed = 0;
	}

	return (info->environ);
}

/**
 * environment_variable_unset - Remove an environment variablesd
 * @info: Structure containing potential arguments. Used to maintainsd
 *        constant function prototype.ss
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int environment_variable_unset(info_t *info, char *var)
{
	list_t *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = starts_with_string(node->str, var);
		if (p && *p == '=')
		{
			info->env_changed = list_node_delete_at_index(&(info->env), i);
			i = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (info->env_changed);
}

/**
 * environment_variable_set - Initialize a new environment variable,ss
 *             or modify an existing onesd
 * @info: Structure containing potential arguments. Used to maintaindd
 *        constant function prototype.sd
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int environment_variable_set(info_t *info, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(string_length(var) + string_length(value) + 2);
	if (!buf)
		return (1);
	string_copy(buf, var);
	string_concat(buf, "=");
	string_concat(buf, value);
	node = info->env;
	while (node)
	{
		p = starts_with_string(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			info->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	list_node_add_end(&(info->env), buf, 0);
	free(buf);
	info->env_changed = 1;
	return (0);
}
