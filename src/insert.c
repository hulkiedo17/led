#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>
#include "buffer.h"
#include "main.h"
#include "io.h"
#include "misc.h"

static bool check_for_correct_arguments(const char* const argument)
{
	if(argument == NULL) {
		return false;
	}

	size_t i = 0;
	size_t size;

	size = strlen(argument) + 1;

	if(!isdigit(argument[i])) {
		return false;
	}

	for(; i < size; i++) {
		if(!isdigit(argument[i])) {
			break;
		}
	}

	if(!isspace(argument[i])) {
		return false;
	}

	return true;
}

static char* get_tokens_from_argument(const char* const argument, size_t *line_num_pointer)
{
	size_t temp_data_len;
	size_t line_number;
	char* line = NULL;
	char* data = NULL;
	char* temp_string = NULL;
	const char* temp_data = NULL;

	if(check_for_correct_arguments(argument) == false)
		return NULL;

	temp_string = strdup(argument);

	line = strtok(temp_string, " ");
	line_number = get_number(line);
	*line_num_pointer = line_number;

	temp_data = argument + strlen(line) + 1;
	temp_data_len = strlen(temp_data) + 1;

	data = alloc_buffer(temp_data_len);

	strncpy(data, temp_data, temp_data_len);
	free(temp_string);
	return data;
}

static char* insert(size_t position, char* data, int new_line_flag)
{
	if(data == NULL) {
		warning(stderr, "warning: null data pointer\n");
		return NULL;
	}

	size_t data_length;
	size_t buffer_length;
	char* temp_buffer = NULL;

	data_length = strlen(data) + 1;

	if(!global_buffer) {
		buffer_length = get_buffer_size(global_buffer);

		if(position >= buffer_length) {
			warning(stderr, "warning: out of buffer\n");
			return NULL;
		}

		if(new_line_flag != DONT_SKIP_NEW_LINE) {
			data_length--;
		}

		temp_buffer = alloc_buffer(buffer_length + data_length);

		strncpy(temp_buffer, global_buffer, position);
		strncpy(temp_buffer + position, data, data_length);
		strncpy(temp_buffer + position + data_length, global_buffer + position, buffer_length - position);

		if(new_line_flag == DONT_SKIP_NEW_LINE) {
			temp_buffer[position + data_length - 1] = '\n';
		}

		free(global_buffer);
		global_buffer = alloc_buffer(data_length + buffer_length);
		strncpy(global_buffer, temp_buffer, data_length + buffer_length);
		free(temp_buffer);
	} else {
		global_buffer = alloc_buffer(data_length);
		strncpy(global_buffer, data, data_length);
	}

	free(data);
	is_data_saved = false;
	return global_buffer;
}

void insert_after(const char* const data, int new_line_flag)
{
	size_t number_of_lines;
	size_t position;
	size_t line_number = 0;
	char* data_pointer = NULL;
	bool is_err = false;

	number_of_lines = get_number_of_lines(global_buffer);
	data_pointer = get_tokens_from_argument(data, &line_number);
	if(data_pointer == NULL) {
		warning(stderr, "warning: invaid arguments\n");
		return;
	}

	if(line_number > number_of_lines) {
		printf("out of lines\n");
		free(data_pointer);
		return;
	}

	if(line_number == number_of_lines) {
		if(line_number == 0) {
			position = 0;
		} else {
			position = get_buffer_size(global_buffer) - 1;
		}
	} else {
		position = get_position_at_line(global_buffer, line_number + 1, &is_err);
		if(is_err)
		{
			warning(stderr, "warning: get_position_at_line() failed\n");
			free(data_pointer);
			return;
		}
	}

	if(insert(position, data_pointer, new_line_flag) == NULL) {
		warning(stderr, "warning: insert() failed\n");
		return;
	}

	is_data_saved = false;
	return;
}

void insert_before(const char* const data, int new_line_flag)
{
	size_t number_of_lines;
	size_t position = 0;
	size_t line_number = 0;
	char* data_pointer = NULL;
	bool is_err = false;

	number_of_lines = get_number_of_lines(global_buffer);
	data_pointer = get_tokens_from_argument(data, &line_number);
	if(data_pointer == NULL) {
		warning(stderr, "warning: invalid arguments\n");
		return;
	}

	if(line_number < 1 || line_number > number_of_lines) {
		printf("out of lines\n");
		free(data_pointer);
		return;
	}

	if(line_number <= number_of_lines) {
		if(line_number == 0) {
			position = 0;
		} else {
			position = get_position_at_line(global_buffer, line_number, &is_err);
			if(is_err)
			{
				free(data_pointer);
				warning(stderr, "warning: get_position_at_line() failed\n");
				return;
			}
		}
	}

	if(insert(position, data_pointer, new_line_flag) == NULL) {
		warning(stderr, "warning: insert() failed\n");
		return;
	}

	is_data_saved = false;
	return;
}
