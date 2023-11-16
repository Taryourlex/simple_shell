#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for number_to_string() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly lssinked list
 * @num: the number fielddd
 * @str: a string
 * @next: points to the nexsst node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseddudo-arguements to pass into a function,
 *					allowing unifsorm prototype for function pointer struct
 *@arg: a string generated from getsdline containing arguements
 *@argv: an array of strings generdated from arg
 *@path: a string path for the cursrent command
 *@argc: the argumendt count
 *@line_count: the errosdr count
 *@err_num: the error cosde for exit()s
 *@linecount_flag: if on cdount this line of input
 *@fname: the program filesdname
 *@env: linked list local csopy of environ
 *@environ: custom modified dcopy of environ from LL env
 *@history: the history nosdde
 *@alias: the alias nosde
 *@env_changed: on if environs was changed
 *@status: the return statuds of the last exec'd command
 *@cmd_buf: address of pointerd to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &d&, ;
 *@readfd: the fd from which to sread line input
 *@histcount: the history line nsdumber count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmds ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtdin string and related function
 *@type: the builtin cosmmand flag
 *@func: the functllion
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;


/* mainshellloop.c */
int mainshellloop(info_t *, char **);
int builtin_command_search(info_t *);
void command_search(info_t *);
void command_execute(info_t *);

/* path.c */
int is_command_executable(info_t *, char *);
char *duplicate_characters(char *, int, int);
char *search_command_path(info_t *, char *, char *);

/* mainshellloop.c */
int loopmainshellloop(char **);

/* error_string_functions.c */
void error_puts(char *);
int error_putchar(char);
int put_character_to_fd(char c, int fd);
int put_string_to_fd(char *str, int fd);

/* string_utilities.c */
int string_length(char *);
int string_compare(char *, char *);
char *starts_with_string(const char *, const char *);
char *string_concat(char *, char *);

/* string_utilities2.c */
char *string_copy(char *, char *);
char *string_duplicate(const char *);
void string_puts(char *);
int character_put(char);

/* string_utilities3.c */
char *string_n_copy(char *, char *, int);
char *string_n_concat(char *, char *, int);
char *string_character_search(char *, char);

/* sstring_utilities4.c */
char **string_split_to_words(char *, char *);
char **string_split_to_words2(char *, char);

/* memory_utilities.c */
char *memory_set(char *, char, unsigned int);
void free_array(char **);
void *memory_reallocate(void *, unsigned int, unsigned int);

/* memory_utilities2.c */
int buffer_free(void **);

/* additional_functions.c */
int is_shell_interactive(info_t *);
int is_character_delimiter(char, char *);
int is_alpha(int);
int ascii_to_integer(char *);

/* additional_functions2.c */
int error_string_to_int(char *);
void error_print(info_t *, char *);
int print_decimal(int, int);
char *number_to_string(long int, int, int);
void command_comments_remove(char *);

/* builtin_commands.c */
int shell_exit(info_t *);
int shell_change_directory(info_t *);
int shell_display_help(info_t *);

/* builtin_commands2.c */
int shell_show_history(info_t *);
int shell_alias_management(info_t *);

/* getline.c module */
ssize_t input_get(info_t *);
int get_line(info_t *, char **, size_t *);
void signal_interrupt_handler(int);

/* info_management.c module */
void info_clear(info_t *);
void info_setup(info_t *, char **);
void info_free(info_t *, int);

/* environment.c module */
char *environment_get_variable(info_t *, const char *);
int shell_environment(info_t *);
int shell_environment_set(info_t *);
int environment_list_populate(info_t *);
int populate_env_list(info_t *);

/* environment2.c module */
char **environment_get(info_t *);
int environment_variable_unset(info_t *, char *);
int environment_variable_set(info_t *, char *, char *);

/* file_io_utilities.c */
char *history_file_get(info_t *info);
int history_write(info_t *info);
int history_read(info_t *info);
int history_list_build(info_t *info, char *buf, int linecount);
int history_renumber(info_t *info);

/* llinked_list_string.c */
list_t *add_node(list_t **, const char *, int);
list_t *list_node_add_end(list_t **, const char *, int);
size_t list_string_print(const list_t *);
int list_node_delete_at_index(list_t **, unsigned int);
void list_free(list_t **);

/* llinked_list_string2.c */
size_t list_length(const list_t *);
char **list_to_string_array(list_t *);
size_t list_print(const list_t *);
list_t *list_node_starts_with(list_t *, char *, char);
ssize_t list_node_index_get(list_t *, list_t *);

/* command_chaining.c */
int is_command_chain(info_t *, char *, size_t *);
void command_chain_check(info_t *, char *, size_t *, size_t, size_t);
int replace_alias(info_t *);
int variables_replace(info_t *);
int string_replace(char **, char *);

#endif
