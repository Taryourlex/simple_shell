#include "shell.h"

/**
 * shell_environment - prints the current environmentsd
 * @info: Structure containing potential arguments. Used to maintainf
 *          constant function prototype.sd
 * Return: Always 0
 */
int shell_environment(info_t *info)
{
	list_string_print(info->env);
	return (0);
}

/**
 * environment_get_variable - gets the value of an environ variabledf
 * @info: Structure containing potential arguments. Used to maintainsd
 * @name: env var namesd
 *
 * Return: the valuesd
 */
char *environment_get_variable(info_t *info, const char *name)
{
	list_t *node = info->env;
	char *p;

	while (node)
	{
		p = starts_with_string(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * shell_environment_set - Initialize a new environment variable,sd
 *             or modify an existing onesd
 * @info: Structure containing potential arguments. Used to maintainsd
 *        constant function prototype.sd
 *  Return: Always 0
 */
int shell_environment_set(info_t *info)
{
	if (info->argc != 3)
	{
		error_puts("Incorrect number of arguements\n");
		return (1);
	}
	if (environment_variable_set(info, info->argv[1], info->argv[2]))
		return (0);
	return (1);
}

/**
 * environment_list_populate - Remove an environment variablewe
 * @info: Structure containing potential arguments. Used to maintainrffs
 *        constant function prototype.ssd
 *  Return: Always 0
 */
int environment_list_populate(info_t *info)
{
	int i;

	if (info->argc == 1)
	{
		error_puts("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= info->argc; i++)
		environment_variable_unset(info, info->argv[i]);

	return (0);
}

/**
 * populate_env_list - populates env linked listsd
 * @info: Structure containing potential arguments. Used to maintainsd
 *          constant function prototype.sd
 * Return: Always 0
 */
int populate_env_list(info_t *info)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		list_node_add_end(&node, environ[i], 0);
	info->env = node;
	return (0);
}
