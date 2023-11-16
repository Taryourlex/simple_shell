#include "shell.h"

/**
 * shell_exit - exits the shelld
 * @info: Structure containing potential arguments. Used to maintainf
 *          constant function prototype.s
 *  Return: exits with a given exit statusf
 *         (0) if info.argv[0] != "exit"s
 */
int shell_exit(info_t *info)
{
	int checkexite;

	if (info->argv[1])  /* If there is an exit arguementdf */
	{
		checkexite = error_string_to_int(info->argv[1]);
		if (checkexite == -1)
		{
			info->status = 2;
			error_print(info, "Illegal number: ");
			error_puts(info->argv[1]);
			error_putchar('\n');
			return (1);
		}
		info->err_num = error_string_to_int(info->argv[1]);
		return (-2);
	}
	info->err_num = -1;
	return (-2);
}

/**
 * shell_change_directory - changes the current directory of the processsd
 * @info: Structure containing potential arguments. Used to maintainsdf
 *          constant function prototypeer.
 *  Return: Always 0
 */
int shell_change_directory(info_t *info)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		string_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!info->argv[1])
	{
		dir = environment_get_variable(info, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this bree? */
				chdir((dir = environment_get_variable(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (string_compare(info->argv[1], "-") == 0)
	{
		if (!environment_get_variable(info, "OLDPWD="))
		{
			string_puts(s);
			character_put('\n');
			return (1);
		}
		string_puts(environment_get_variable(info, "OLDPWD=")), character_put('\n');
		chdir_ret = /* TODO: what should this besd? */
			chdir((dir = environment_get_variable(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->argv[1]);
	if (chdir_ret == -1)
	{
		error_print(info, "can't cd to ");
		error_puts(info->argv[1]), error_putchar('\n');
	}
	else
	{
		environment_variable_set(info, "OLDPWD", environment_get_variable(info, "PWD="));
		environment_variable_set(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * shell_display_help - changes the current directory of the processwe
 * @info: Structure containing potential arguments. Used to maintainsd
 *          constant function prototypesd.
 *  Return: Always 0
 */
int shell_display_help(info_t *info)
{
	char **arg_array;

	arg_array = info->argv;
	string_puts("help call works. Function not yet implemented \n");
	if (0)
		string_puts(*arg_array); /* temp att_unused workarounder */
	return (0);
}
