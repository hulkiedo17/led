#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>
#include <stdbool.h>
#include "../include/misc.h"
#include "../include/prompt.h"
#include "../include/io.h"
#include "../include/parser.h"
#include "../include/main.h"
#include "../include/buffer.h"
#include "../include/append.h"
#include "../include/file.h"
#include "../include/insert.h"
#include "../include/delete.h"

char* buffer = NULL;
char* global_filename = NULL;
bool is_data_saved = true;

static int execute_command(command_token_t token);
static void free_all_pointers_and_buffer(void);

int main(int argc, char **argv)
{
	int result, help_flag = 0, version_flag = 0;
	command_token_t token = UNKNOWN_TOKEN;
	char* input_buffer = NULL;
	const char* short_opt = "hvf:";
	const struct option long_opt[] = {
		{"help", no_argument, NULL, 'h'},
		{"version", no_argument, NULL, 'v'},
		{"filename", required_argument, NULL, 'f'},
		{NULL, 0, NULL, 0}
	};

	while((result = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1) {
		switch(result) {
		case 'h':
			help();
			help_flag = 1;
			break;
		case 'v':
			version();
			version_flag = 1;
			break;
		case 'f':
			set_filename(optarg);
			break;
		default:
			printf("-%c - unknown option\n", optopt);
			break;
		}
	}

	if(help_flag || version_flag) {
		exit(EXIT_SUCCESS);
	}

	while(1) {
		print_prompt();

		input_buffer = read_from_stream(stdin, DONT_SKIP_NEW_LINE);
		token = parse_command(input_buffer);
		free(input_buffer);

		if(execute_command(token) == QUIT_FLAG) {
			break;
		}
	}

	free_all_pointers_and_buffer();
	return 0;
}

static int execute_command(command_token_t token)
{
	size_t temp_val;
	switch(token) {
	case QUIT:
		if(is_data_saved == false) {
			printf("before you quit, please write data in buffer to file or clear buffer\n");
			break;
		}
		return QUIT_FLAG;
		break;
	case SET_CUSTOM_PROMPT:
		set_custom_prompt(first_small_argument_pointer);
		break;
	case SET_DEFAULT_PROMPT:
		set_default_prompt();
		break;
	case CLEAN_SCREEN:
		clean_screen();
		break;
	case CLEAN_BUFFER:
		clean_buffer();
		break;
	case PRINT_DEFAULT:
		print_buffer(first_small_argument_pointer, DONT_OUTPUT_NUMBERED_LINES);
		break;
	case PRINT_NUMBERED_LINES:
		print_buffer(first_small_argument_pointer, OUTPUT_NUMBERED_LINES);
		break;
	case APPEND:
		append_data(SKIP_NEW_LINE);
		break;
	case APPEND_WITH_NEW_LINE:
		append_data(DONT_SKIP_NEW_LINE);
		break;
	case PRINT_BY_CHAR:
		print_buffer_by_char();
		break;
	case SET_FILENAME:
		set_filename(first_small_argument_pointer);
		break;
	case PRINT_FILENAME:
		print_filename();
		break;
	case WRITE_BUF:
		save_buffer(first_small_argument_pointer, "w+");
		break;
	case WRITE_BUF_APPEND:
		save_buffer(first_small_argument_pointer, "a+");
		break;
	case FILL_BUF:
		fill_buffer(first_small_argument_pointer, SKIP_NEW_LINE);
		break;
	case FILL_BUF_NL:
		fill_buffer(first_small_argument_pointer, DONT_SKIP_NEW_LINE);
		break;
	case INSERT_AFTER:
		insert_after(one_big_argument_pointer, SKIP_NEW_LINE);
		break;
	case INSERT_AFTER_NL:
		insert_after(one_big_argument_pointer, DONT_SKIP_NEW_LINE);
		break;
	case INSERT_BEFORE:
		insert_before(one_big_argument_pointer, SKIP_NEW_LINE);
		break;
	case INSERT_BEFORE_NL:
		insert_before(one_big_argument_pointer, DONT_SKIP_NEW_LINE);
		break;
	case DELETE_LINE:
		temp_val = (size_t)strtol(first_small_argument_pointer, NULL, 10);
		delete_line(temp_val);
		break;
	case DELETE_LINES_IN_RANGE:
		delete_range_of_lines(first_small_argument_pointer, second_small_argument_pointer);
		break;
	case UNKNOWN_TOKEN:
	default:
		printf("unknown command\n");
		break;
	}

	return DONT_QUIT_FLAG;
}

static void free_all_pointers_and_buffer(void)
{
	if(one_big_argument_pointer != NULL) {
		free(one_big_argument_pointer);
		one_big_argument_pointer = NULL;
	}
	if(first_small_argument_pointer != NULL) {
		free(first_small_argument_pointer);
		first_small_argument_pointer = NULL;
	}
	if(second_small_argument_pointer != NULL) {
		free(second_small_argument_pointer);
		second_small_argument_pointer = NULL;
	}
	if(global_filename != NULL) {
		free(global_filename);
		global_filename = NULL;
	}
	if(buffer != NULL) {
		free(buffer);
		buffer = NULL;
	}
}