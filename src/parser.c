#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/misc.h"
#include "../include/parser.h"
#include "../include/buffer.h"

static const char* const command_list[] = {
	"q", "sp", "sd", "cs", "c", "p", "pl",
	"a", "an", "pc", "sf", "pf", "w", "wa",
	"f", "fn", "ia", "ian", "ib", "ibn", 
	"dl", "dr"
};

static const command_token_t token_list[] = {
	QUIT, SET_CUSTOM_PROMPT, SET_DEFAULT_PROMPT, CLEAN_SCREEN,
	CLEAN_BUFFER, PRINT_DEFAULT, PRINT_NUMBERED_LINES, APPEND,
	APPEND_NL, PRINT_BY_CHAR, SET_FILENAME, PRINT_FILENAME,
	WRITE_BUF, WRITE_BUF_APPEND, FILL_BUF, FILL_BUF_NL,
	INSERT_AFTER, INSERT_AFTER_NL, INSERT_BEFORE, INSERT_BEFORE_NL,
	DELETE_LINE, DELETE_LINES_IN_RANGE
};

static const size_t command_list_size = sizeof(command_list) / sizeof(command_list[0]);
char* one_big_argument_pointer = NULL;
char* first_small_argument_pointer = NULL;
char* second_small_argument_pointer = NULL;

static tokens_t get_command_token(const char* const string_token)
{
	tokens_t token = { UNKNOWN_TYPE, UNKNOWN_TOKEN };
	if(string_token == NULL) {
		//warning(stderr, "error: null token pointer\n");
		return token;
	}

	for(size_t i = 0; i < command_list_size; i++) {
		if(strcmp(string_token, command_list[i]) == 0) {
			token.token_type = token_list[i];
			break;
		}
	}

	switch(token.token_type) {
		case QUIT:
		case SET_DEFAULT_PROMPT:
		case CLEAN_SCREEN:
		case PRINT_DEFAULT:
		case APPEND:
		case APPEND_NL:
		case PRINT_BY_CHAR:
		case PRINT_FILENAME:
			token.command_arg_type = DONT_USES_ARG;
			break;
		case SET_CUSTOM_PROMPT:
		case PRINT_NUMBERED_LINES:
		case SET_FILENAME:
		case WRITE_BUF:
		case WRITE_BUF_APPEND:
		case FILL_BUF:
		case FILL_BUF_NL:
		case DELETE_LINE:
		case DELETE_LINES_IN_RANGE:
			token.command_arg_type = USES_A_FEW_SMALL_ARGS;
			break;
		case INSERT_AFTER:
		case INSERT_AFTER_NL:
		case INSERT_BEFORE:
		case INSERT_BEFORE_NL:
			token.command_arg_type = USES_ONE_BIG_ARG;
			break;
		case UNKNOWN_TYPE:
		default:
			//warning(stderr, "err: unknown type");
			break;
	}

	return token;
}

tokens_t parse_command(const char* const input_buffer)
{
	tokens_t command_token = { UNKNOWN_TYPE, UNKNOWN_TOKEN };
	char* command_without_args = NULL;
	char* local_input_buffer = NULL;
	char* first_arg_ptr = NULL;
	char* second_arg_ptr = NULL;
	const char* big_arg_ptr = NULL;
	size_t first_arg_size;
	size_t second_arg_size;
	size_t big_arg_size;

	local_input_buffer = alloc_buffer(strlen(input_buffer) + 1);
	strncpy(local_input_buffer, input_buffer, strlen(input_buffer) + 1);

	command_without_args = strtok(local_input_buffer, " ");
	command_token = get_command_token(command_without_args);

	if(command_token.command_arg_type == USES_A_FEW_SMALL_ARGS) {
		first_arg_ptr = strtok(NULL, " ");
		second_arg_ptr = strtok(NULL, " ");

		if(first_arg_ptr != NULL) {
			first_arg_size = strlen(first_arg_ptr) + 1;
			first_small_argument_pointer = alloc_buffer(first_arg_size);

			strncpy(first_small_argument_pointer, first_arg_ptr, first_arg_size);

			if(second_arg_ptr != NULL) {
				second_arg_size = strlen(second_arg_ptr) + 1;
				second_small_argument_pointer = alloc_buffer(second_arg_size);

				strncpy(second_small_argument_pointer, second_arg_ptr, second_arg_size);
			}
		}
	} else if(command_token.command_arg_type == USES_ONE_BIG_ARG) {
		big_arg_ptr = input_buffer + strlen(command_without_args) + 1;
		big_arg_size = strlen(big_arg_ptr) + 1;

		one_big_argument_pointer = alloc_buffer(big_arg_size);
		strncpy(one_big_argument_pointer, big_arg_ptr, big_arg_size);
	} else {
		if(command_token.command_arg_type != DONT_USES_ARG) {
			//warning(stderr, "error: unknown type of argument for command\n");
			free(local_input_buffer);
			return command_token;
		}
	}

	free(local_input_buffer);
	return command_token;
}
