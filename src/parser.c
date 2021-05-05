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

static argument_type_t command_arg_type = UNKNOWN_TYPE;
char* one_big_argument_pointer = NULL;
char* first_small_argument_pointer = NULL;
char* second_small_argument_pointer = NULL;

static command_token_t get_command_token(const char* const token)
{
	if(token == NULL) {
		//warning(stderr, "error: null token pointer\n");
		return UNKNOWN_TOKEN;
	}

	if(strcmp(token, command_list[0]) == 0) {				// q
		command_arg_type = DONT_USES_ARG;
		return QUIT;
	} else if(strcmp(token, command_list[1]) == 0) {		// sp
		command_arg_type = USES_A_FEW_SMALL_ARGS;
		return SET_CUSTOM_PROMPT;
	} else if(strcmp(token, command_list[2]) == 0) {		// sd
		command_arg_type = DONT_USES_ARG;
		return SET_DEFAULT_PROMPT;
	} else if(strcmp(token, command_list[3]) == 0) {		// cs
		command_arg_type = DONT_USES_ARG;
		return CLEAN_SCREEN;
	} else if(strcmp(token, command_list[4]) == 0) {		// c
		command_arg_type = DONT_USES_ARG;
		return CLEAN_BUFFER;
	} else if(strcmp(token, command_list[5]) == 0) {		// p
		command_arg_type = DONT_USES_ARG;
		return PRINT_DEFAULT;
	} else if(strcmp(token, command_list[6]) == 0) {		// pl
		command_arg_type = USES_A_FEW_SMALL_ARGS;
		return PRINT_NUMBERED_LINES;
	} else if(strcmp(token, command_list[7]) == 0) {		// a
		command_arg_type = DONT_USES_ARG;
		return APPEND;
	} else if(strcmp(token, command_list[8]) == 0) {		// ap
		command_arg_type = DONT_USES_ARG;
		return APPEND_WITH_NEW_LINE;
	} else if(strcmp(token, command_list[9]) == 0) {		// pc
		command_arg_type = DONT_USES_ARG;
		return PRINT_BY_CHAR;
	} else if(strcmp(token, command_list[10]) == 0) {		// sf
		command_arg_type = USES_A_FEW_SMALL_ARGS;
		return SET_FILENAME;
	} else if(strcmp(token, command_list[11]) == 0) {		// pf
		command_arg_type = DONT_USES_ARG;
		return PRINT_FILENAME;
	} else if(strcmp(token, command_list[12]) == 0) {		// w
		command_arg_type = USES_A_FEW_SMALL_ARGS;
		return WRITE_BUF;
	} else if(strcmp(token, command_list[13]) == 0) {		// wa
		command_arg_type = USES_A_FEW_SMALL_ARGS;
		return WRITE_BUF_APPEND;
	} else if(strcmp(token, command_list[14]) == 0) {		// f
		command_arg_type = USES_A_FEW_SMALL_ARGS;
		return FILL_BUF;
	} else if(strcmp(token, command_list[15]) == 0) {		// fn
		command_arg_type = USES_A_FEW_SMALL_ARGS;
		return FILL_BUF_NL;
	} else if(strcmp(token, command_list[16]) == 0) {		// ia
		command_arg_type = USES_ONE_BIG_ARG;
		return INSERT_AFTER;
	} else if(strcmp(token, command_list[17]) == 0) {		// ian
		command_arg_type = USES_ONE_BIG_ARG;
		return INSERT_AFTER_NL;
	} else if(strcmp(token, command_list[18]) == 0) {		// ib
		command_arg_type = USES_ONE_BIG_ARG;
		return INSERT_BEFORE;
	} else if(strcmp(token, command_list[19]) == 0) {		// ibn
		command_arg_type = USES_ONE_BIG_ARG;
		return INSERT_BEFORE_NL;
	} else if(strcmp(token, command_list[20]) == 0) {		// dl
		command_arg_type = USES_A_FEW_SMALL_ARGS;
		return DELETE_LINE;
	} else if(strcmp(token, command_list[21]) == 0) {		// dr
		command_arg_type = USES_A_FEW_SMALL_ARGS;
		return DELETE_LINES_IN_RANGE;
	}

	return UNKNOWN_TOKEN;
}

command_token_t parse_command(const char* const input_buffer)
{
	command_token_t token = UNKNOWN_TOKEN;
	char* command_without_args = NULL;
	char* local_input_buffer;
	char* first_arg_ptr = NULL, *second_arg_ptr = NULL;
	const char* big_arg_ptr = NULL;
	size_t first_arg_size, second_arg_size;
	size_t big_arg_size;

	local_input_buffer = alloc_buffer(strlen(input_buffer) + 1);
	strncpy(local_input_buffer, input_buffer, strlen(input_buffer) + 1);

	command_without_args = strtok(local_input_buffer, " ");
	token = get_command_token(command_without_args);

	if(command_arg_type == USES_A_FEW_SMALL_ARGS) {
		first_arg_ptr = strtok(NULL, " ");
		second_arg_ptr = strtok(NULL, " ");

		if(first_small_argument_pointer != NULL) {
			free(first_small_argument_pointer);
			first_small_argument_pointer = NULL;
		}
		if(second_small_argument_pointer != NULL) {
			free(second_small_argument_pointer);
			second_small_argument_pointer = NULL;
		}

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
	} else if(command_arg_type == USES_ONE_BIG_ARG) {
		big_arg_ptr = input_buffer + strlen(command_without_args) + 1;
		big_arg_size = strlen(big_arg_ptr) + 1;

		if(one_big_argument_pointer != NULL) {
			free(one_big_argument_pointer);
			one_big_argument_pointer = NULL;
		}

		one_big_argument_pointer = alloc_buffer(big_arg_size);
		strncpy(one_big_argument_pointer, big_arg_ptr, big_arg_size);
	} else {
		if(command_arg_type != DONT_USES_ARG) {
			//warning(stderr, "error: unknown type of argument for command\n");
			return token;
		}
	}

	free(local_input_buffer);
	return token;
}
