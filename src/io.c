#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../include/misc.h"
#include "../include/io.h"

char* read_from_stream(FILE* stream, uint8_t new_line_flag)
{
	if(stream == NULL) {
		fail(stderr, "error: null stream pointer\n");
	}

	int input_buffer_size = IBUF_SIZE;
	int c;
	int position = 0;
	char* input_buffer = calloc(input_buffer_size, sizeof(char));
	if(input_buffer == NULL) {
		fail(stderr, "error: allocation error for input buffer\n");
	}

	while(1) {
		c = fgetc(stream);

		if(new_line_flag == SKIP_NEW_LINE) {
			if(c == EOF) {
				input_buffer[position] = '\0';
				return input_buffer;
			} else {
				input_buffer[position] = c;
			}
		} else if(new_line_flag == DONT_SKIP_NEW_LINE) {
			if(c == EOF || c == '\n') {
				input_buffer[position] = '\0';
				return input_buffer;
			} else {
				input_buffer[position] = c;
			}
		} else {
			fail(stderr, "error: invalid value for new_line_flag\n");
		}

		position++;

		if(position >= input_buffer_size) {
			input_buffer_size += IBUF_SIZE;
			input_buffer = realloc(input_buffer, input_buffer_size);
			if(input_buffer == NULL) {
				fail(stderr, "error: reallocation error for input_buffer\n");
			}
		}
	}
}
