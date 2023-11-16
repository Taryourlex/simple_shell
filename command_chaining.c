#include "shell.h"

/**
 * is_command_chain - to test if current char in buffer is a chain delimeterse
 * @info: the parameter structd
 * @buf: the char bufferf
 * @p: address of current position in bufs
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */
int is_command_chain(info_t *info, char *buf, size_t *p)
{
	size_t j = *p;

	if (buf[j] == '|' && buf[j + 1] == '|')
	{
		buf[j] = 0;
		j++;
		info->cmd_buf_type = CMD_OR;
	}
	else if (buf[j] == '&' && buf[j + 1] == '&')
	{
		buf[j] = 0;
		j++;
		info->cmd_buf_type = CMD_AND;
	}
	else if (buf[j] == ';') /* found end of this commandsd */
	{
		buf[j] = 0; /* replace semicolon with null */
		info->cmd_buf_type = CMD_CHAIN;
	}
	else
		return (0);
	*p = j;
	return (1);
}

/**
 * command_chain_check - checks we should continueer chaining based on last status
 * @info: the parameter structsd
 * @buf: the char buffersd
 * @p: address of current position in bufsd
 * @i: starting position in bufsd
 * @len: length of bufde
 *
 * Return: Void
 */
void command_chain_check(info_t *info, char *buf, size_t *p, size_t i, size_t len)
{
	size_t j = *p;

	if (info->cmd_buf_type == CMD_AND)
	{
		if (info->status)
		{
			buf[i] = 0;
			j = len;
		}
	}
	if (info->cmd_buf_type == CMD_OR)
	{
		if (!info->status)
		{
			buf[i] = 0;
			j = len;
		}
	}

	*p = j;
}

/**
 * replace_alias - replaces an aliases in the tokenized stringrrtt
 * @info: the parameter structdf
 *
 * Return: 1 if replaced, 0 otherwisese
 */
int replace_alias(info_t *info)
{
	int i;
	list_t *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = list_node_starts_with(info->alias, info->argv[0], '=');
		if (!node)
			return (0);
		free(info->argv[0]);
		p = string_character_search(node->str, '=');
		if (!p)
			return (0);
		p = string_duplicate(p + 1);
		if (!p)
			return (0);
		info->argv[0] = p;
	}
	return (1);
}

/**
 * variables_replace - replaces vars in the tokenized stringdf
 * @info: the parameter structsd
 *
 * Return: 1 if replaced, 0 otherwisef
 */
int variables_replace(info_t *info)
{
	int i = 0;
	list_t *node;

	for (i = 0; info->argv[i]; i++)
	{
		if (info->argv[i][0] != '$' || !info->argv[i][1])
			continue;

		if (!string_compare(info->argv[i], "$?"))
		{
			string_replace(&(info->argv[i]),
				string_duplicate(number_to_string(info->status, 10, 0)));
			continue;
		}
		if (!string_compare(info->argv[i], "$$"))
		{
			string_replace(&(info->argv[i]),
				string_duplicate(number_to_string(getpid(), 10, 0)));
			continue;
		}
		node = list_node_starts_with(info->env, &info->argv[i][1], '=');
		if (node)
		{
			string_replace(&(info->argv[i]),
				string_duplicate(string_character_search(node->str, '=') + 1));
			continue;
		}
		string_replace(&info->argv[i], string_duplicate(""));

	}
	return (0);
}

/**
 * string_replace - replaces stringrf
 * @old: address of old stringsd
 * @new: new stringsd
 *
 * Return: 1 if replaced, 0 otherwisesf
 */
int string_replace(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}
