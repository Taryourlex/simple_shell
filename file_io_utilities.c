#include "shell.h"

/**
 * history_file_get - gets the history filesd
 * @info: parameter structsd
 *
 * Return: allocated string containg history filesd
 */

char *history_file_get(info_t *info)
{
	char *buf, *dir;

	dir = environment_get_variable(info, "HOME=");
	if (!dir)
		return (NULL);
	buf = malloc(sizeof(char) * (string_length(dir) + string_length(HIST_FILE) + 2));
	if (!buf)
		return (NULL);
	buf[0] = 0;
	string_copy(buf, dir);
	string_concat(buf, "/");
	string_concat(buf, HIST_FILE);
	return (buf);
}

/**
 * history_write - creates a file, or appends to an existing filesd
 * @info: the parameter structsd
 *
 * Return: 1 on success, else -1
 */
int history_write(info_t *info)
{
	ssize_t fd;
	char *filename = history_file_get(info);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = info->history; node; node = node->next)
	{
		put_string_to_fd(node->str, fd);
		put_character_to_fd('\n', fd);
	}
	put_character_to_fd(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * history_read - reads history from filesd
 * @info: the parameter structss
 *
 * Return: histcount on success, 0 otherwisedd
 */
int history_read(info_t *info)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = history_file_get(info);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			history_list_build(info, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		history_list_build(info, buf + last, linecount++);
	free(buf);
	info->histcount = linecount;
	while (info->histcount-- >= HIST_MAX)
		list_node_delete_at_index(&(info->history), 0);
	history_renumber(info);
	return (info->histcount);
}

/**
 * history_list_build - adds entry to a history linked list sd
 * @info: Structure containing potential arguments. Used to maintainsd
 * @buf: buffer
 * @linecount: the history linecount, histcounts
 *
 * Return: Always 0
 */
int history_list_build(info_t *info, char *buf, int linecount)
{
	list_t *node = NULL;

	if (info->history)
		node = info->history;
	list_node_add_end(&node, buf, linecount);

	if (!info->history)
		info->history = node;
	return (0);
}

/**
 * history_renumber - renumbers the history linked list after changessd
 * @info: Structure containing potential arguments. Used to maintainss
 *
 * Return: the new histcountdd
 */
int history_renumber(info_t *info)
{
	list_t *node = info->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (info->histcount = i);
}
