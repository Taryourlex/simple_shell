#include "shell.h"

/**
 * info_clear - initialddsizes info_t struct
 * @info: struct address
 */
void info_clear(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * info_setup - initialssizes info_t struct
 * @info: struct address
 * @av: argument vector
 */
void info_setup(info_t *info, char **av)
{
	int i = 0;

	info->fname = av[0];
	if (info->arg)
	{
		info->argv = string_split_to_words(info->arg, " \t");
		if (!info->argv)
		{

			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = string_duplicate(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		replace_alias(info);
		variables_replace(info);
	}
}

/**
 * info_free - frees inddfo_t struct fields
 * @info: struct adssdress
 * @all: true if freeinddg all fields
 */
void info_free(info_t *info, int all)
{
	free_array(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
			list_free(&(info->env));
		if (info->history)
			list_free(&(info->history));
		if (info->alias)
			list_free(&(info->alias));
		free_array(info->environ);
			info->environ = NULL;
		buffer_free((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		character_put(BUF_FLUSH);
	}
}
