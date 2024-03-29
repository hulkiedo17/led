#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "misc.h"
#include "buffer.h"

static char* delete(size_t position1, size_t position2)
{
	char* temp_buffer = NULL;
	size_t start_position;
	size_t end_position;
	size_t bufsize;
	size_t temp_bufsize;

	if((bufsize = get_buffer_size(global_buffer)) == 0) {
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
		free(global_buffer);
		free(temp_buffer);
		global_buffer = NULL;
		is_data_saved = true;
		return NULL;
	}

	strncpy(temp_buffer, global_buffer, start_position);
	if(end_position == bufsize - 1) {
		if(start_position != 0) {
			temp_buffer[start_position - 1] = '\0';
		} else {
			strncpy(temp_buffer, global_buffer + end_position, bufsize - end_position);
		}
	} else {
		strncpy(temp_buffer + start_position, global_buffer + end_position, bufsize - end_position);
	}

	free(global_buffer);
	global_buffer = alloc_buffer(temp_bufsize);
	strncpy(global_buffer, temp_buffer, temp_bufsize);

	free(temp_buffer);
	is_data_saved = false;
	return global_buffer;
}

void delete_line(size_t line_number)
{
	size_t number_of_lines;
	size_t position1;
	size_t position2;
	bool is_err = false;

	number_of_lines = get_number_of_lines(global_buffer);

	if(line_number < 1 || line_number > number_of_lines) {
		printf("out of lines\n");
		return;
	}

	// handling the last line in the buffer
	if(line_number == 1 && number_of_lines == 1) {
		free(global_buffer);
		global_buffer = NULL;
		is_data_saved = true;
		return;
	}

	position1 = get_position_at_line(global_buffer, line_number, &is_err);
	if(is_err) {
		warning(stderr, "warning: get_number_of_lines() failed\n");
		return;
	}

	if(line_number == number_of_lines) {
		position2 = get_buffer_size(global_buffer) - 1;
	} else {
		position2 = get_position_at_line(global_buffer, line_number + 1, &is_err);
		if(is_err) {
			warning(stderr, "warning: get_number_of_lines() failed\n");
			return;
		}
	}

	if(delete(position1, position2) == NULL) {
		warning(stderr, "warning: delete() failed\n");
	}
}

void delete_range_of_lines(const char* const line1_start, const char* const line2_end)
{
	if(line1_start == NULL || line2_end == NULL) {
		warning(stderr, "warning: null lines pointers\n");
		return;
	}

	if(!global_buffer) {
		printf("buffer is empty\n");
		return;
	}

	size_t number_of_lines;
	size_t start_line;
	size_t end_line;
	size_t start_line_position;
	size_t end_line_position;
	bool is_err = false;

	number_of_lines = get_number_of_lines(global_buffer);

	start_line = get_number(line1_start);
	end_line = get_number(line2_end);

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
		if(start_line == 0) {
			start_line_position = 0;
		} else {
			start_line_position = get_position_at_line(global_buffer, start_line, &is_err);
			if(is_err) {
				warning(stderr, "warning: get_number_of_lines() failed\n");
				return;
			}
		}

		if(end_line == number_of_lines) {
			end_line_position = get_buffer_size(global_buffer);
		} else {
			end_line_position = get_position_at_line(global_buffer, end_line + 1, &is_err);
			if(is_err) {
				warning(stderr, "warning: get_number_of_lines() failed\n");
				return;
			}
		}

		if(start_line_position == 0 && end_line_position == get_buffer_size(global_buffer)) {
			clean_buffer();
		} else {
			if(end_line == number_of_lines - 1) {
				end_line_position++;
			}
			if(delete(start_line_position, end_line_position - 1) == NULL) { // -1 because '\0'
				warning(stderr, "warning: delete() failed\n");
				return;
			}
		}
	}
}
