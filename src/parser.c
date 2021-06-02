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

	if(strcmp(string_token, command_list[0]) == 0) {			// q
		token.command_arg_type = DONT_USES_ARG;
		token.token_type = QUIT;
	} else if(strcmp(string_token, command_list[1]) == 0) {		// sp
		token.command_arg_type = USES_A_FEW_SMALL_ARGS;
		token.token_type = SET_CUSTOM_PROMPT;
	} else if(strcmp(string_token, command_list[2]) == 0) {		// sd
		token.command_arg_type = DONT_USES_ARG;
		token.token_type = SET_DEFAULT_PROMPT;
	} else if(strcmp(string_token, command_list[3]) == 0) {		// cs
		token.command_arg_type = DONT_USES_ARG;
		token.token_type = CLEAN_SCREEN;
	} else if(strcmp(string_token, command_list[4]) == 0) {		// c
		token.command_arg_type = DONT_USES_ARG;
		token.token_type = CLEAN_BUFFER;
	} else if(strcmp(string_token, command_list[5]) == 0) {		// p
		token.command_arg_type = DONT_USES_ARG;
		token.token_type = PRINT_DEFAULT;
	} else if(strcmp(string_token, command_list[6]) == 0) {		// pl
		token.command_arg_type = USES_A_FEW_SMALL_ARGS;
		token.token_type = PRINT_NUMBERED_LINES;
	} else if(strcmp(string_token, command_list[7]) == 0) {		// a
		token.command_arg_type = DONT_USES_ARG;
		token.token_type = APPEND;
	} else if(strcmp(string_token, command_list[8]) == 0) {		// ap
		token.command_arg_type = DONT_USES_ARG;
		token.token_type = APPEND_WITH_NEW_LINE;
	} else if(strcmp(string_token, command_list[9]) == 0) {		// pc
		token.command_arg_type = DONT_USES_ARG;
		token.token_type = PRINT_BY_CHAR;
	} else if(strcmp(string_token, command_list[10]) == 0) {	// sf
		token.command_arg_type = USES_A_FEW_SMALL_ARGS;
		token.token_type = SET_FILENAME;
	} else if(strcmp(string_token, command_list[11]) == 0) {	// pf
		token.command_arg_type = DONT_USES_ARG;
		token.token_type = PRINT_FILENAME;
	} else if(strcmp(string_token, command_list[12]) == 0) {	// w
		token.command_arg_type = USES_A_FEW_SMALL_ARGS;
		token.token_type = WRITE_BUF;
	} else if(strcmp(string_token, command_list[13]) == 0) {	// wa
		token.command_arg_type = USES_A_FEW_SMALL_ARGS;
		token.token_type = WRITE_BUF_APPEND;
	} else if(strcmp(string_token, command_list[14]) == 0) {	// f
		token.command_arg_type = USES_A_FEW_SMALL_ARGS;
		token.token_type = FILL_BUF;
	} else if(strcmp(string_token, command_list[15]) == 0) {	// fn
		token.command_arg_type = USES_A_FEW_SMALL_ARGS;
		token.token_type = FILL_BUF_NL;
	} else if(strcmp(string_token, command_list[16]) == 0) {	// ia
		token.command_arg_type = USES_ONE_BIG_ARG;
		token.token_type = INSERT_AFTER;
	} else if(strcmp(string_token, command_list[17]) == 0) {	// ian
		token.command_arg_type = USES_ONE_BIG_ARG;
		token.token_type = INSERT_AFTER_NL;
	} else if(strcmp(string_token, command_list[18]) == 0) {	// ib
		token.command_arg_type = USES_ONE_BIG_ARG;
		token.token_type = INSERT_BEFORE;
	} else if(strcmp(string_token, command_list[19]) == 0) {	// ibn
		token.command_arg_type = USES_ONE_BIG_ARG;
		token.token_type = INSERT_BEFORE_NL;
	} else if(strcmp(string_token, command_list[20]) == 0) {	// dl
		token.command_arg_type = USES_A_FEW_SMALL_ARGS;
		token.token_type = DELETE_LINE;
	} else if(strcmp(string_token, command_list[21]) == 0) {	// dr
		token.command_arg_type = USES_A_FEW_SMALL_ARGS;
		token.token_type = DELETE_LINES_IN_RANGE;
	} else {
		token.command_arg_type = UNKNOWN_TYPE;
		token.token_type = UNKNOWN_TOKEN;
	}

	return token;
}

tokens_t parse_command(const char* const input_buffer)
{
	tokens_t command_token = { UNKNOWN_TYPE, UNKNOWN_TOKEN };
	char* command_without_args = NULL;
	char* local_input_buffer;
	char* first_arg_ptr = NULL, *second_arg_ptr = NULL;
	const char* big_arg_ptr = NULL;
	size_t first_arg_size, second_arg_size;
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
