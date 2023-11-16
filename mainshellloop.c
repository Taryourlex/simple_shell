#include "shell.h"

/**
 * mainshellloop - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int mainshellloop(info_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		info_clear(info);
		if (is_shell_interactive(info))
			string_puts("$ ");
		error_putchar(BUF_FLUSH);
		r = input_get(info);
		if (r != -1)
		{
			info_setup(info, av);
			builtin_ret = builtin_command_search(info);
			if (builtin_ret == -1)
				command_search(info);
		}
		else if (is_shell_interactive(info))
			character_put('\n');
		info_free(info, 0);
	}
	history_write(info);
	info_free(info, 1);
	if (!is_shell_interactive(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (builtin_ret);
}

/**
 * builtin_command_search - finds a builtin commandss
 * @info: the parameter & return info structdd
 *
 * Return: -1 if builtin not found,ss
 *			0 if builtin executed successfullyd,
 *			1 if builtin found but not successfuld,
 *			-2 if builtin signals exit()
 */
int builtin_command_search(info_t *info)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", shell_exit},
		{"env", shell_environment},
		{"help", shell_display_help},
		{"history", shell_show_history},
		{"setenv", shell_environment_set},
		{"unsetenv", environment_list_populate},
		{"cd", shell_change_directory},
		{"alias", shell_alias_management},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (string_compare(info->argv[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * command_search - finds a commdand in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void command_search(info_t *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!is_character_delimiter(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = search_command_path(info, environment_get_variable(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		command_execute(info);
	}
	else
	{
		if ((is_shell_interactive(info) || environment_get_variable(info, "PATH=")
			|| info->argv[0][0] == '/') && is_command_executable(info, info->argv[0]))
			command_execute(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			error_print(info, "not found\n");
		}
	}
}

/**
 * command_execute - forks a an exddec thread to run cmd
 * @info: the parameter & return infsso struct
 *
 * Return: void
 */
void command_execute(info_t *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTddION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, environment_get(info)) == -1)
		{
			info_free(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				error_print(info, "Permission denied\n");
		}
	}
}
