#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/main.h"
#include "../include/misc.h"
#include "../include/buffer.h"
#include "../include/io.h"

static char* choose_filename(char* filename)
{
	if(filename == NULL) {
		if(global_filename == NULL) {
			return NULL;
		}

		return global_filename;
	}
	return filename;
}

void save_buffer(char* filename, const char* mode)
{
	FILE* fp = NULL;
	char* current_filename = NULL;
	size_t buffer_length;

	if((buffer_length = get_buffer_size(global_buffer)) == 0) {
		printf("buffer is empty\n");
		return;
	}
	buffer_length--;	// write without '\0' character

	current_filename = choose_filename(filename);
	if(current_filename == NULL) {
		printf("unknown filename, please type another filename\n");
		return;
	}

	fp = fopen(current_filename, mode);
	if(fp == NULL) {
		warning(stderr, "warning: cannot save buffer - file can't open or doesn't exists\n");
		return;
	}

	if(fwrite(global_buffer, buffer_length, 1, fp) != 1) {
		fail(stderr, "error: fwrite() failed\n");
	}

	printf("written %zd bytes\n", buffer_length);

	fclose(fp);
	is_data_saved = true;
}

void fill_buffer(char* filename, uint8_t new_line_flag)
{
	int buffer_length, temp_buffer_length;
	char* temp_buffer = NULL;
	char* current_filename = NULL;
	FILE* fp = NULL;

	current_filename = choose_filename(filename);
	if(current_filename == NULL) {
		printf("unknown filename, please type another filename\n");
		return;
	}

	fp = fopen(current_filename, "r+");
	if(fp == NULL) {
		warning(stderr, "warning: cannot open this %s file\n", current_filename);
		return;
	}

	temp_buffer = read_from_stream(fp, SKIP_NEW_LINE);
	fclose(fp);

	if((buffer_length = get_buffer_size(global_buffer)) == -1) {
		buffer_length = 0;
	}

	temp_buffer_length = buflen(temp_buffer) + 1;
	global_buffer = realloc_buffer(global_buffer, buffer_length, temp_buffer_length);

	strncpy(global_buffer + buffer_length, temp_buffer, temp_buffer_length);
	if(buffer_length != 0) {
		if(new_line_flag == DONT_SKIP_NEW_LINE) {
			global_buffer[buffer_length - 1] = '\n';
		} else {
			global_buffer[buffer_length - 1] = ' ';
		}
	}

	free(temp_buffer);
	is_data_saved = false;
}
