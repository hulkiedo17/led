#ifndef PARSER_H
#define PARSER_H

typedef enum argument_type {
	UNKNOWN_TYPE = 0,
	DONT_USES_ARG,
	USES_ONE_BIG_ARG,
	USES_A_FEW_SMALL_ARGS
} argument_type_t;

typedef enum command_token {
	UNKNOWN_TOKEN = 0,
	QUIT,
	SET_CUSTOM_PROMPT,
	SET_DEFAULT_PROMPT,
	CLEAN_SCREEN,
	CLEAN_BUFFER,
	PRINT_DEFAULT,
	PRINT_NUMBERED_LINES,
	APPEND,
	APPEND_NL,
	PRINT_BY_CHAR,
	SET_FILENAME,
	PRINT_FILENAME,
	WRITE_BUF,
	WRITE_BUF_APPEND,
	FILL_BUF,
	FILL_BUF_NL,
	INSERT_AFTER,
	INSERT_AFTER_NL,
	INSERT_BEFORE,
	INSERT_BEFORE_NL,
	DELETE_LINE,
	DELETE_LINES_IN_RANGE
} command_token_t;

typedef struct tokens {
	argument_type_t command_arg_type;
	command_token_t token_type;
} tokens_t;

extern char* one_big_argument_pointer;
extern char* first_small_argument_pointer;
extern char* second_small_argument_pointer;

tokens_t parse_command(const char* const input_buffer);
void free_argument_pointers(void);

#endif
