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

char* global_buffer = NULL;
char* global_filename = NULL;
bool is_data_saved = true;

static int execute_command(tokens_t token);
static void free_argument_pointers(void);
static void free_buffer_pointers(void);

int main(int argc, char **argv)
{
	int result;
	int help_flag = 0;
	int version_flag = 0;
	tokens_t token = { UNKNOWN_TYPE, UNKNOWN_TOKEN };
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

	free_buffer_pointers();
	return 0;
}

static int execute_command(tokens_t token)
{
	size_t temp_val;
	switch(token.token_type) {
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
		print_buffer(global_buffer, first_small_argument_pointer, DONT_OUTPUT_NUMBERED_LINES);
		break;
	case PRINT_NUMBERED_LINES:
		print_buffer(global_buffer, first_small_argument_pointer, OUTPUT_NUMBERED_LINES);
		break;
	case APPEND:
		append_data(SKIP_NEW_LINE);
		break;
	case APPEND_NL:
		append_data(DONT_SKIP_NEW_LINE);
		break;
	case PRINT_BY_CHAR:
		print_buffer_by_char(global_buffer);
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

	free_argument_pointers();

	return DONT_QUIT_FLAG;
}

static void free_argument_pointers(void) {
	free(first_small_argument_pointer); first_small_argument_pointer = NULL;
	free(second_small_argument_pointer); second_small_argument_pointer = NULL;
	free(one_big_argument_pointer); one_big_argument_pointer = NULL;
}

static void free_buffer_pointers(void) {
	free(global_filename); global_filename = NULL;
	free(global_buffer); global_buffer = NULL;
}

