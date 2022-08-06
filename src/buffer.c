#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "misc.h"
#include "main.h"
#include "buffer.h"

char* alloc_buffer(size_t size)
{
	char* buffer = malloc(size);
	if(!buffer) {
		fail(stderr, "error: allocation error for new buffer\n");
	}
	return buffer;
}

char* realloc_buffer(char* buffer, size_t old_size, size_t new_data_size)
{
	buffer = realloc(buffer, old_size + new_data_size);
	if(!buffer) {
		fail(stderr, "error: reallocation error for buffer\n");
	}
	return buffer;
}

size_t get_buffer_size(const char* const buffer)
{
	if(buffer) {
		return strlen(buffer) + 1;
	}
	return 0;
}

size_t get_number_of_lines(const char* const buffer)
{
	size_t new_line_counter = 1;
	size_t count;
	size_t bufsize;

	if((bufsize = get_buffer_size(buffer)) == 0) {
		//warning(stderr, "buffer is empty\n");
		return bufsize;
	}

	for(count = 0; count < bufsize; count++) {
		if(buffer[count] == '\n') {
			new_line_counter++;
		}
	}

	return new_line_counter;
}

size_t get_position_at_line(const char* const buffer, size_t line_number, bool *is_err)
{
	size_t new_line_counter = 1;
	size_t count;
	size_t bufsize;

	if((bufsize = get_buffer_size(buffer)) == 0) {
		*is_err = true;
		return 0;
	}

	for(count = 0; count < bufsize; count++) {
		if(new_line_counter == line_number) {
			return count;
		}
		if(buffer[count] == '\n') {
			new_line_counter++;
		}
	}

	*is_err = true;
	return 0;
}

void clean_buffer(void)
{
	if(global_buffer != NULL) {
		printf("freed memory: %zd bytes\n", get_buffer_size(global_buffer));
		free(global_buffer);
		global_buffer = NULL;
	} else {
		printf("buffer is empty\n");
	}

	is_data_saved = true;
}

void print_buffer(const char* const buffer, char* line, uint8_t numbered_lines_flag)
{
	size_t count_lines = 1;
	size_t number_of_lines;
	size_t line_position;
	size_t line_position2;
	size_t line_number;
	bool is_err = false;

	number_of_lines = get_number_of_lines(buffer);

	if(!buffer) {
		printf("buffer is empty\n");
		return;
	} else {
		if(line != NULL) {
			line_number = get_number(line);

			if((line_number < 1) || (line_number > number_of_lines)) {
				printf("warninig: out of lines\n");
				return;
			}

			line_position = get_position_at_line(buffer, line_number, &is_err);
			if(is_err) {
				warning(stderr, "warning: get_position_at_line() failed\n");
				return;
			}

			if(line_number == number_of_lines) {
				line_position2 = get_buffer_size(buffer);
			} else {
				line_position2 = get_position_at_line(buffer, line_number + 1, &is_err);
				if(is_err) {
					warning(stderr, "warning: get_position_at_line() failed\n");
					return;
				}
			}

			for(size_t i = line_position; i < line_position2; i++) {
				if(buffer[i] != '\n') {
					putchar(buffer[i]);
				}
			}
			putchar('\n');
		} else if(numbered_lines_flag == OUTPUT_NUMBERED_LINES) {
			if(number_of_lines < 10) {
				printf("%ld ", count_lines);
			} else if(number_of_lines >= 10 && number_of_lines < 100) {
				printf("%2ld ", count_lines);
			} else if(number_of_lines >= 100 && number_of_lines < 1000) {
				printf("%3ld ", count_lines);
			} else if(number_of_lines >= 1000 && number_of_lines < 10000) {
				printf("%4ld ", count_lines);
			}

			count_lines++;

			for(size_t i = 0; i < strlen(buffer); i++) {
				putchar(buffer[i]);

				if(buffer[i] == '\n') {
					if(number_of_lines < 10) {
						printf("%1ld ", count_lines);
					} else if(number_of_lines >= 10 && number_of_lines < 100) {
						printf("%2ld ", count_lines);
					} else if(number_of_lines >= 100 && number_of_lines < 1000) {
						printf("%3ld ", count_lines);
					} else if(number_of_lines >= 100 && number_of_lines < 10000) {
						printf("%4ld ", count_lines);
					}
					count_lines++;
				}
			}
			putchar('\n');
		} else {
			printf("%s\n", buffer);
		}
	}
}

void print_buffer_by_char(const char* const buffer)
{
	if(!buffer) {
		printf("buffer is empty\n");
		return;
	}

	for(size_t i = 0; i < strlen(buffer) + 1; i++) {
		if(buffer[i] == '\0') {
			printf("\'\\0\'\n");
			break;
		} else if(buffer[i] == '\n') {
			printf("\'\\n\'\n");
		} else {
			printf("\'%c\' ", buffer[i]);
		}
	}
}

void free_buffer_pointers(void) {
	free(global_filename); global_filename = NULL;
	free(global_buffer); global_buffer = NULL;
}
