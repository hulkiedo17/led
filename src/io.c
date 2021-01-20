#include <stdio.h>
#include <stdlib.h>
#include "../include/io.h"
#include "../include/misc.h"

char* read_from_stream(FILE* stream, int with_new_line_flag)
{
    if(stream == NULL) {
        fail("read_from_stream(): null stream pointer");
    }

	int input_buffer_size = IBUF_SIZE;
	int position = 0, c;
	char *input_buffer = malloc(input_buffer_size * sizeof(char));

	if(!input_buffer)
		fail("read_input(): allocation error");

	while(1) {
		c = fgetc(stream);

	// if we set the new_line_flag, so we should to read file
	if(with_new_line_flag == SET_NEW_LINE_FLAG) {
            if(c == EOF) {
                input_buffer[position] = '\0';
                return input_buffer;
            } else {
                input_buffer[position] = c;
            }
        } else {
			// else, we read from stdin
            if(c == EOF || c == '\n') {
                input_buffer[position] = '\0';
                return input_buffer;
            } else {
                input_buffer[position] = c;
            }
        }

		position++;

		// typically reallocation for memory
		if(position >= input_buffer_size) {
			input_buffer_size += IBUF_SIZE;
			input_buffer = realloc(input_buffer, input_buffer_size);
			if(!input_buffer)
				fail("read_input(): reallocation error");
		}
	}
}

int write_to_file(char *buffer, size_t size, FILE* fp)
{
    return fwrite(buffer, size, 1, fp);
}

int read_from_file(char *buffer, size_t size, FILE* fp)
{
    return fread(buffer, size, 1, fp);
}

FILE* open_file(char *filename, char *mode)
{
    FILE* fp = fopen(filename, mode);
    if(fp == NULL)
        return NULL;

    return fp;
}
