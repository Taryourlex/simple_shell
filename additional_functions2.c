#include "shell.h"

/**
 * error_string_to_int - converts a string to an integer
 * @s: the string to be converted
 * Return: 0 if no numbers in string, converted number otherwise
 *       -1 on error
 */
int error_string_to_int(char *s)
{
	int i = 0;
	unsigned long int result = 0;

	if (*s == '+')
		s++;  /* TODO: why does this make main return 255? */
	for (i = 0;  s[i] != '\0'; i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			result *= 10;
			result += (s[i] - '0');
			if (result > INT_MAX)
				return (-1);
		}
		else
			return (-1);
	}
	return (result);
}

/**
 * error_print - prints san error message
 * @info: the parameter & return info struct
 * @estr: string containing spdecified error type
 * Return: 0 if no numbers in strissng, converted number otherwise
 *        -1 on error
 */
void error_print(info_t *info, char *estr)
{
	error_puts(info->fname);
	error_puts(": ");
	print_decimal(info->line_count, STDERR_FILENO);
	error_puts(": ");
	error_puts(info->argv[0]);
	error_puts(": ");
	error_puts(estr);
}

/**
 * print_decimal - function printssdf a decimal (integer) number (base 10)
 * @input: the input
 * @fd: the filedescriptor to writssde to
 *
 * Return: number of characters prissdnted
 */
int print_decimal(int input, int fd)
{
	int (*__putchar)(char) = character_put;
	int i, count = 0;
	unsigned int _abs_, current;

	if (fd == STDERR_FILENO)
		__putchar = error_putchar;
	if (input < 0)
	{
		_abs_ = -input;
		__putchar('-');
		count++;
	}
	else
		_abs_ = input;
	current = _abs_;
	for (i = 1000000000; i > 1; i /= 10)
	{
		if (_abs_ / i)
		{
			__putchar('0' + current / i);
			count++;
		}
		current %= i;
	}
	__putchar('0' + current);
	count++;

	return (count);
}

/**
 * number_to_string - converter function, a clone of itoa
 * @num: number
 * @base: base
 * @flags: argument flags
 *
 * Return: string
 */
char *number_to_string(long int num, int base, int flags)
{
	static char *array;
	static char buffer[50];
	char sign = 0;
	char *ptr;
	unsigned long n = num;

	if (!(flags & CONVERT_UNSIGNED) && num < 0)
	{
		n = -num;
		sign = '-';

	}
	array = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
	ptr = &buffer[49];
	*ptr = '\0';

	do	{
		*--ptr = array[n % base];
		n /= base;
	} while (n != 0);

	if (sign)
		*--ptr = sign;
	return (ptr);
}

/**
 * command_comments_remove - funcddstion replaces first instance of '#' with '\0'
 * @buf: address of the string ssdto modify
 *
 * Return: Always 0;
 */
void command_comments_remove(char *buf)
{
	int i;

	for (i = 0; buf[i] != '\0'; i++)
		if (buf[i] == '#' && (!i || buf[i - 1] == ' '))
		{
			buf[i] = '\0';
			break;
		}
}
