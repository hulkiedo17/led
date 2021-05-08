#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/main.h"
#include "../include/io.h"
#include "../include/buffer.h"

void append_data(uint8_t new_line_flag)
{
	char* input_buffer = NULL;
	size_t input_buffer_size = 0;
	size_t buffer_size = 0;

	input_buffer = read_from_stream(stdin, DONT_SKIP_NEW_LINE);
	while(strcmp(input_buffer, ".") != 0) {
		if(is_buffer_empty(global_buffer) == true) {
			input_buffer_size = buflen(input_buffer) + 1;
			global_buffer = alloc_buffer(input_buffer_size);

			strncpy(global_buffer, input_buffer, input_buffer_size);
		} else {
			input_buffer_size = buflen(input_buffer) + 1;
			buffer_size = get_buffer_size(global_buffer);

			global_buffer = realloc_buffer(global_buffer, buffer_size, input_buffer_size);

			strncpy(global_buffer + buffer_size, input_buffer, input_buffer_size);

			if(new_line_flag == DONT_SKIP_NEW_LINE) {
				global_buffer[buffer_size - 1] = '\n';
			} else {
				global_buffer[buffer_size - 1] = ' ';
			}
		}
		free(input_buffer);
		input_buffer = read_from_stream(stdin, DONT_SKIP_NEW_LINE);
	}

	free(input_buffer);
	is_data_saved = false;
}
