#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "prompt.h"
#include "parser.h"
#include "buffer.h"
#include "append.h"
#include "delete.h"
#include "insert.h"
#include "file.h"
#include "misc.h"
#include "io.h"

int execute_command(tokens_t token)
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
