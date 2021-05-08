#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/misc.h"
#include "../include/main.h"
#include "../include/buffer.h"

bool is_buffer_empty(const char * const buffer)
{
	if(buffer == NULL) {
		return true;
	} else {
		return false;
	}
}

size_t buflen(const char* const buffer)
{
	size_t i = 0;
	if(buffer == NULL) {
		return i;
	}

	while(buffer[i] != '\0') {
		i++;
	}

	return i;
}

char* alloc_buffer(size_t size)
{
	char* buffer = malloc(size * sizeof(char));
	if(buffer == NULL) {
		fail(stderr, "error: allocation error for new buffer\n");
	}
	return buffer;
}

char* realloc_buffer(char* buffer, size_t old_size, size_t new_data_size)
{
	buffer = realloc(buffer, old_size + new_data_size);
	if(buffer == NULL) {
		fail(stderr, "error: reallocation error for buffer\n");
	}
	return buffer;
}

size_t get_buffer_size(const char* const buffer)
{
	if(is_buffer_empty(buffer) == false) {
		return buflen(buffer) + 1;
	}
	return 0;
}

size_t get_number_of_lines(const char* const buffer)
{
	size_t new_line_counter, count, bufsize = get_buffer_size(buffer);

	if(bufsize == 0) {
		//warning(stderr, "buffer is empty\n");
		return 0;
	}

	new_line_counter = 1;
	for(count = 0; count < bufsize; count++) {
		if(buffer[count] == '\n') {
			new_line_counter++;
		}
	}

	return new_line_counter;
}

int get_position_at_line(const char* const buffer, size_t line_number)
{
	size_t new_line_counter = 1, count, bufsize = get_buffer_size(buffer);

	if(bufsize == 0) {
		//warning(stderr, "buffer is empty\n");
		return -1;
	}

	for(count = 0; count < bufsize; count++) {
		if(new_line_counter == line_number) {
			return count;
		}
		if(buffer[count] == '\n') {
			new_line_counter++;
		}
	}

	return -1;
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
	size_t count_lines = 1, number_of_lines = get_number_of_lines(buffer);
	int line_position, line_position2;
	long line_number;

	if(is_buffer_empty(buffer) == true) {
		printf("buffer is empty\n");
		return;
	} else {
		if(line != NULL) {
			line_number = strtol(line, NULL, 10);
			
			if((line_number < (long)1) || (line_number > (long)number_of_lines)) {
				printf("warninig: out of lines\n");
				return;
			}

			line_position = get_position_at_line(buffer, line_number);
			if(line_position == -1) {
				warning(stderr, "warning: get_position_at_line() failed\n");
				return;
			}

			if(line_number == (long)number_of_lines) {
				line_position2 = get_buffer_size(buffer);
			} else {
				line_position2 = get_position_at_line(buffer, line_number + 1);
				if(line_position2 == -1) {
					warning(stderr, "warning: get_position_at_line() failed\n");
					return;
				}
			}

			for(size_t i = line_position; i < (size_t)line_position2; i++) {
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

			for(size_t i = 0; i < buflen(buffer); i++) {
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
	if(is_buffer_empty(buffer) == true) {
		printf("buffer is empty\n");
		return;
	}

	for(size_t i = 0; i < buflen(buffer) + 1; i++) {
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
