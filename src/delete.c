#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/main.h"
#include "../include/misc.h"
#include "../include/buffer.h"

static char* delete(size_t position1, size_t position2)
{
	char* temp_buffer = NULL;
	size_t start_position, end_position, bufsize, temp_bufsize;

	if((bufsize = get_buffer_size()) == 0) {
		return NULL;
	}

	if(position1 < position2 && position1 < (bufsize - 1)) {
		start_position = position1;
	} else {
		//warning(stderr, "warning: invalid position1\n");
		return NULL;
	}

	if(position2 > 0 && position2 > position1 && position2 < bufsize) {
		end_position = position2;
	} else {
		//warning(stderr, "warning: invalid position2\n");
		return NULL;
	}

	temp_bufsize = bufsize - (end_position - start_position);
	if(temp_bufsize > 0) {
		temp_buffer = alloc_buffer(temp_bufsize);
	} else {
		free(buffer);
		free(temp_buffer);
		buffer = NULL;
		is_data_saved = true;
		return NULL;
	}

	strncpy(temp_buffer, buffer, start_position);
	if(end_position == bufsize - 1) {
		temp_buffer[start_position - 1] = '\0';
	} else {
		strncpy(temp_buffer + start_position, buffer + end_position, bufsize - end_position);
	}

	free(buffer);
	buffer = alloc_buffer(temp_bufsize);
	strncpy(buffer, temp_buffer, temp_bufsize);

	free(temp_buffer);
	is_data_saved = false;
	return buffer;
}

void delete_line(size_t line_number)
{
	size_t number_of_lines = get_number_of_lines();
	int position1, position2;

	if(line_number < (size_t)1 || line_number > number_of_lines) {
		printf("warning: out of lines\n");
		return;
	}

	// handling the last line in the buffer
	if(line_number == (size_t)1 && number_of_lines == (size_t)1) {
		free(buffer);
		buffer = NULL;
		is_data_saved = true;
		return;
	}

	if((position1 = get_position_at_line(line_number)) == -1) {
		warning(stderr, "warning: get_number_of_lines() failed\n");
		return;
	}

	if(line_number == number_of_lines) {
		position2 = get_buffer_size() - 1;
	} else {
		if((position2 = get_position_at_line(line_number + 1)) == -1) {
			warning(stderr, "warning: get_number_of_lines() failed\n");
			return;
		}
	}

	if(delete((size_t)position1, (size_t)position2) == NULL) {
		warning(stderr, "warning: delete() failed\n");
	}
}

void delete_range_of_lines(const char* const line1_start, const char* const line2_end)
{
	if(line1_start == NULL || line2_end == NULL) {
		warning(stderr, "warning: null lines pointers\n");
		return;
	}

	if(is_buffer_empty()) {
		printf("buffer is empty\n");
		return;
	}

	size_t number_of_lines = get_number_of_lines();
	size_t start_line, end_line;

	start_line = (size_t)strtol(line1_start, NULL, 10);
	end_line = (size_t)strtol(line2_end, NULL, 10);

	if(start_line > 0 && end_line <= number_of_lines) {
		if(start_line > end_line) {
			printf("invalid range(%zd-%zd)\n", start_line, end_line);
			return;
		}
	} else {
		printf("out of lines\n");
		return;
	}

	if(start_line == end_line) {
		delete_line(start_line);
	} else {
		for(size_t i = 0; i <= (end_line - start_line); i++) {
			delete_line(start_line);
		}
	}
}
