#include "shell.h"

/**
 *error_puts - prints an input stringsd
 * @str: the string to be printedsd
 *
 * Return: Nothing
 */
void error_puts(char *str)
{
	int i = 0;

	if (!str)
		return;
	while (str[i] != '\0')
	{
		error_putchar(str[i]);
		i++;
	}
}

/**
 * error_putchar - writes the character c to stderrsd
 * @c: The character to prints
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.sd
 */
int error_putchar(char c)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(2, buf, i);
		i = 0;
	}
	if (c != BUF_FLUSH)
		buf[i++] = c;
	return (1);
}

/**
 * put_character_to_fd - writes the character c to given fddf
 * @c: The character to printss
 * @fd: The filedescriptor to write tosd
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.d
 */
int put_character_to_fd(char c, int fd)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(fd, buf, i);
		i = 0;
	}
	if (c != BUF_FLUSH)
		buf[i++] = c;
	return (1);
}

/**
 *put_string_to_fd - prints an input string
 * @str: the string to be printed
 * @fd: the filedescriptor to write to
 *
 * Return: the number of chars put
 */
int put_string_to_fd(char *str, int fd)
{
	int i = 0;

	if (!str)
		return (0);
	while (*str)
	{
		i += put_character_to_fd(*str++, fd);
	}
	return (i);
}
