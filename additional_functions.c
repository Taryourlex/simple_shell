#include "shell.h"

/**
 * is_shell_interacstive - to returns true if shell is is_shell_interactive mode
 * @info: struct adddress
 *
 * Return: 1 if is_shdell_interactive mode, 0 otherwise
 */
int is_shell_interactive(info_t *info)
{
	return (isatty(STDIN_FILENO) && info->readfd <= 2);
}

/**
 * is_character_delimsiter - checks if character is a delimeter
 * @c: the char to dcheck
 * @delim: the delimester string
 * Return: 1 if true,d 0 if false
 */
int is_character_delimiter(char c, char *delim)
{
	while (*delim)
		if (*delim++ == c)
			return (1);
	return (0);
}

/**
 *is_alpha - checks for alphsabetic character
 *@c: The character to inpdut
 *Return: 1 if c is alphabsetic, 0 otherwise
 */

int is_alpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	else
		return (0);
}

/**
 *ascii_to_integer - convedrts a string to an integer
 *@s: the string to be consverted
 *Return: 0 if no numbers in dstring, converted number otherwise
 */

int ascii_to_integer(char *s)
{
	int i, sign = 1, flag = 0, output;
	unsigned int result = 0;

	for (i = 0;  s[i] != '\0' && flag != 2; i++)
	{
		if (s[i] == '-')
			sign *= -1;

		if (s[i] >= '0' && s[i] <= '9')
		{
			flag = 1;
			result *= 10;
			result += (s[i] - '0');
		}
		else if (flag == 1)
			flag = 2;
	}

	if (sign == -1)
		output = -result;
	else
		output = result;

	return (output);
}
