#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>
#include "../include/buffer.h"
#include "../include/main.h"
#include "../include/io.h"
#include "../include/misc.h"

static bool check_for_correct_arguments(const char* const argument)
{
	if(argument == NULL) {
		return -1;
	}

	size_t i = 0;
	size_t size = buflen(argument) + 1;

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

static char* get_tokens_from_argument(const char* const argument, long *line_num_pointer)
{
	char* line, *data;
	const char* temp_data;
	char* temp_string;
	long line_number;
	size_t temp_data_len;

	temp_string = dup_string(argument);
	if(check_for_correct_arguments(argument) == false) {
		free(temp_string);
		return NULL;
	}

	line = strtok(temp_string, " ");
	line_number = strtol(line, NULL, 10);
	*line_num_pointer = line_number;

	temp_data = argument + strlen(line) + 1;
	temp_data_len = strlen(temp_data) + 1;

	data = alloc_buffer(temp_data_len);

	strncpy(data, temp_data, temp_data_len);
	free(temp_string);
	return data;
}

static char* insert(int position, char* data, int new_line_flag)
{
	if(data == NULL) {
		warning(stderr, "warning: null data pointer\n");
		return NULL;
	}

	char* temp_buffer = NULL;
	size_t buffer_length, data_length = buflen(data) + 1;

	if(is_buffer_empty() == false) {
		buffer_length = get_buffer_size();

		if(position < 0 || position >= (int)buffer_length) {
			warning(stderr, "warning: out of buffer\n");
			return NULL;
		}

		temp_buffer = alloc_buffer(buffer_length + data_length);

		strncpy(temp_buffer, buffer, position);
		strncpy(temp_buffer + position, data, data_length);
		strncpy(temp_buffer + position + data_length, buffer + position, buffer_length - position);

		if(new_line_flag == DONT_SKIP_NEW_LINE) {
			temp_buffer[position + data_length - 1] = '\n';
		} else {
			temp_buffer[position + data_length - 1] = ' ';
		}

		free(buffer);
		buffer = alloc_buffer(data_length + buffer_length);
		strncpy(buffer, temp_buffer, data_length + buffer_length);
		free(temp_buffer);
	} else {
		buffer = alloc_buffer(data_length);
		strncpy(buffer, data, data_length);
	}

	free(data);
	is_data_saved = false;
	return buffer;
}

void insert_after(const char* const data, int new_line_flag)
{
	long line_number = -1;
	char* data_pointer;
	size_t number_of_lines, position;

	number_of_lines = get_number_of_lines();
	data_pointer = get_tokens_from_argument(data, &line_number);
	if(data_pointer == NULL) {
		warning(stderr, "warning: invaid arguments\n");
		return;
	}

	if(line_number < (long)0 || line_number > (long)number_of_lines) {
		printf("out of lines\n");
		free(data_pointer);
		return;
	}

	if((size_t)line_number == number_of_lines) {
		if(line_number == 0) {
			position = 0;
		} else {
			position = get_buffer_size() - 1;
		}
	} else {
		position = get_position_at_line(line_number + 1);
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
	long line_number = -1;
	char* data_pointer;
	size_t number_of_lines, position;

	number_of_lines = get_number_of_lines();
	data_pointer = get_tokens_from_argument(data, &line_number);
	if(data_pointer == NULL) {
		warning(stderr, "warning: invalid arguments\n");
		return;
	}

	if(line_number < (long)0 || line_number > (long)number_of_lines) {
		printf("out of lines\n");
		free(data_pointer);
		return;
	}

	if((size_t)line_number <= number_of_lines) {
		if(line_number == 0) {
			position = 0;
		} else {
			position = get_position_at_line(line_number);
		}
	}

	if(insert(position, data_pointer, new_line_flag) == NULL) {
		warning(stderr, "warning: insert() failed\n");
		return;
	}

	is_data_saved = false;
	return;
}