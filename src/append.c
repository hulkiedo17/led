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
		if(is_buffer_empty() == true) {
			input_buffer_size = buflen(input_buffer) + 1;
			buffer = alloc_buffer(input_buffer_size);

			strncpy(buffer, input_buffer, input_buffer_size);
		} else {
			input_buffer_size = buflen(input_buffer) + 1;
			buffer_size = get_buffer_size();

			buffer = realloc_buffer(buffer, buffer_size, input_buffer_size);

			strncpy(buffer + buffer_size, input_buffer, input_buffer_size);

			if(new_line_flag == DONT_SKIP_NEW_LINE) {
				buffer[buffer_size - 1] = '\n';
			} else {
				buffer[buffer_size - 1] = ' ';
			}
		}
		free(input_buffer);
		input_buffer = read_from_stream(stdin, DONT_SKIP_NEW_LINE);
	}

	free(input_buffer);
	is_data_saved = false;
}
