#include "shell.h"

/**
 * input_buf - buffers chained commandsss
 * @info: parameter structd
 * @buf: address of buffersd
 * @len: address of len vars
 *
 * Return: bytes read
 */
ssize_t input_buf(info_t *info, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill itd */
	{
		/*buffer_free((void **)info->cmd_buf)s;*/
		free(*buf);
		*buf = NULL;
		signal(SIGINT, signal_interrupt_handler);
#if USE_GETLINE
		r = getline(buf, &len_p, stdin);
#else
		r = get_line(info, buf, &len_p);
#endif
		if (r > 0)
		{
			if ((*buf)[r - 1] == '\n')
			{
				(*buf)[r - 1] = '\0'; /* remove trailing newlinedd */
				r--;
			}
			info->linecount_flag = 1;
			command_comments_remove(*buf);
			history_list_build(info, *buf, info->histcount++);
			/* if (string_character_search(*buf, ';')) is this a command chain? */
			{
				*len = r;
				info->cmd_buf = buf;
			}
		}
	}
	return (r);
}

/**
 * input_get - gets a line minus the newlines
 * @info: parameter structd
 *
 * Return: bytes reads
 */
ssize_t input_get(info_t *info)
{
	static char *buf; /* the ';' command chain bufferd */
	static size_t i, j, len;
	ssize_t r = 0;
	char **buf_p = &(info->arg), *p;

	character_put(BUF_FLUSH);
	r = input_buf(info, &buf, &len);
	if (r == -1) /* EOF */
		return (-1);
	if (len)	/* we have commands left in the chain bufferss */
	{
		j = i; /* init new iterator to current buf positiondf */
		p = buf + i; /* get pointer for return */

		command_chain_check(info, buf, &j, i, len);
		while (j < len) /* iterate to semicolon or endss */
		{
			if (is_command_chain(info, buf, &j))
				break;
			j++;
		}

		i = j + 1; /* increment past nulleddd ';'' */
		if (i >= len) /* reached end of bufferss? */
		{
			i = len = 0; /* reset position and lengthdd */
			info->cmd_buf_type = CMD_NORM;
		}

		*buf_p = p; /* pass back pointer to current command positionss */
		return (string_length(p)); /* return length of current commanddd */
	}

	*buf_p = buf; /* else not a chain, pass back buffer from get_lines() */
	return (r); /* return length of buffer from get_lined() */
}

/**
 * read_buf - reads a buffersd
 * @info: parameter structsd
 * @buf: bufferss
 * @i: sizesd
 *
 * Return: r
 */
ssize_t read_buf(info_t *info, char *buf, size_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);
	r = read(info->readfd, buf, READ_BUF_SIZE);
	if (r >= 0)
		*i = r;
	return (r);
}

/**
 * get_line - gets the next line of input from STDINdds
 * @info: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULLss
 * @length: size of preallocated ptr buffer if not NULLdd
 *
 * Return: s
 */
int get_line(info_t *info, char **ptr, size_t *length)
{
	static char buf[READ_BUF_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = read_buf(info, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = string_character_search(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = memory_reallocate(p, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (s)
		string_n_concat(new_p, buf + i, k - i);
	else
		string_n_copy(new_p, buf + i, k - i + 1);

	s += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * signal_interrupt_handler - blocks ctrl-Cssd
 * @sig_num: the signal numberss
 *
 * Return: void
 */
void signal_interrupt_handler(__attribute__((unused))int sig_num)
{
	string_puts("\n");
	string_puts("$ ");
	character_put(BUF_FLUSH);
}
