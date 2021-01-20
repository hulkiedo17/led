#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../include/main.h"
#include "../include/io.h"
#include "../include/buffer.h"
#include "../include/misc.h"

uint8_t buffer_allocated_memory_flag = BUFFER_HAS_NO_MEMORY;
uint8_t is_data_saved_flag = DATA_HAS_BEEN_SAVED;
uint8_t line_numbers_flag = WITHOUT_LINE_NUM_IN_OUTPUT;

/* STATIC FUNCTIONS: */

/* 
	this function is a clone of the strlen function, but with one
	difference: it counts characters in the string until it 
	encounters a null character. While the strlen can stop counting
	at the end of line(\n) character, but there, when working with
	strings, this is not necessary.
 */
static int buflen(char *buffer)
{
	long i;
	i = 0;
	while(buffer[i] != '\0')
		i++;

	return i;
}

static char* allocate_mem_for_buffer(int size)
{
	char *buffer = malloc(size * sizeof(char));
	if(buffer == NULL)
		fail("allocate_mem_for_buffer(): allocation error");
	return buffer;
}

static char* reallocate_mem_for_buffer(char *buffer, int old_size, int add_to_old_size)
{
	int size = old_size + add_to_old_size;
	buffer = realloc(buffer, size);
	if(buffer == NULL)
		fail("reallocate_mem_for_buffer(): reallocation error");
	return buffer;
}

static char* choose_filename(char *filename)
{
    if(filename == NULL) {
		if(filename_global == NULL) {
			return NULL;
		}

		return filename_global;
	} 
    
    return filename;    
}

/* NON-STATIC FUNCTIONS: */

char* clean_buffer(void)
{
	if(buffer != NULL) {
		printf("freed memory: %zd bytes\n", strlen(buffer) + 1);
		free(buffer);
	}
	
	return NULL;
}

void print_buffer(void)
{
	long count_lines = 1;

	if(buffer == NULL)
		printf("buffer is empty\n");
	else {
		if(line_numbers_flag == WITH_LINE_NUM_IN_OUTPUT) {
			/* how works the output with line numbers:

				before the loop, number of the first line is
				displayed, then the loop goes through the buffer
				and each character in the array is checked for 
				equality to a newline character, fi the equality 
				is true, then the number of the next line is 
				displayed after it, then it is incremented.
			*/
			printf("%ld ", count_lines++);
			for(long i = 0; i < buflen(buffer); i++) {
				putchar(buffer[i]);
				if(buffer[i] == '\n')
					printf("%ld ", count_lines++);
			}
			putchar('\n');
		} else {
			printf("%s\n", buffer);
		}
	}
}

void add_data_to_buffer(void)
{
	char *input_buffer = NULL;
	int buffer_size = 0;
	int input_buffer_size = 0;

	// read line from standard input
	input_buffer = read_from_stream(stdin, UNSET_NEW_LINE_FLAG);
	while(input_buffer[0] != '.') {
		input_buffer_size = strlen(input_buffer) + 1;

		// if buffer doesn't have some data, allocate the memory
		// and copy data from input_buffer to main buffer
		if(buffer_allocated_memory_flag == BUFFER_HAS_NO_MEMORY) {
			buffer = allocate_mem_for_buffer(input_buffer_size);
			buffer_allocated_memory_flag = BUFFER_HAS_MEMORY;

			strncpy(buffer, input_buffer, input_buffer_size);
		} else {
			// else, if buffer have some data, check the main buffer size,
			// and then reallocate memory in main buffer, and copy the data
			// from the input_buffer to the new allocated memory in the main 
			// buffer.
			buffer_size = strlen(buffer) + 1;
			buffer = reallocate_mem_for_buffer(buffer, buffer_size, input_buffer_size);

			strncpy(buffer + buffer_size, input_buffer, input_buffer_size);

			// and don't forget to replace the \0 to \n character,
			// because \0 character in file is not a good practice
			buffer[buffer_size - 1] = '\n';
		}

		// free used memory in input_buffer and read new line from standard input
		free(input_buffer);
		input_buffer = read_from_stream(stdin, UNSET_NEW_LINE_FLAG);
	}

	free(input_buffer);
}

void save_buffer_to_file(char *filename, char *mode)
{
	if(buffer == NULL) {
		printf("buffer is empty\n");
		return;
	}

	FILE *fp = NULL;
	char *current_filename = NULL;
	int len = buflen(buffer);

    current_filename = choose_filename(filename);
    if(current_filename == NULL) {
		printf("unknown filename, please type filename\n");
        return;
    }

	fp = open_file(current_filename, mode);
	if(fp == NULL) {
		fprintf(stderr, "cannot save buffer - file cannot open or doesn't exists\n");
		return;
	}

	if(write_to_file(buffer, len, fp) < 0)
		fail("save_buffer_to_file(): write_to_file() failed");

	fclose(fp);

	printf("written %d bytes\n", len);
	// we saved data to file, so you need to set the flag 
	is_data_saved_flag = DATA_HAS_BEEN_SAVED;
}

void fill_buffer_from_file(char *filename)
{
	int len_buffer, len_temp_buffer;
	char* temporary_buffer = NULL;
	char *current_filename = NULL;
	FILE* fp = NULL;

	current_filename = choose_filename(filename);
    if(current_filename == NULL) {
		printf("unknown filename, please type filename\n");
        return;
    }

	fp = open_file(current_filename, "r+");
	if(fp == NULL) {
		fprintf(stderr, "cannot open %s file\n", current_filename);
		return;
	}

	temporary_buffer = read_from_stream(fp, SET_NEW_LINE_FLAG);
	fclose(fp);

	if(buffer == NULL) {
		len_buffer = 0;
	} else {
		len_buffer = buflen(buffer) + 1;
	}

	// reallocate memory in main buffer for data in 
	// temporary_buffer, and then copy to main buffer
	len_temp_buffer = strlen(temporary_buffer) + 1;
	buffer = realloc(buffer, len_buffer + len_temp_buffer);
	if(buffer == NULL)
		fail("fill_buffer_from_file(): reallocation error");

	strncpy(buffer + len_buffer, temporary_buffer, len_temp_buffer);

	// don't forget to replace the \0 to \n character
	if(len_buffer == 0) {
		buffer_allocated_memory_flag = BUFFER_HAS_MEMORY;
	} else {
		buffer[len_buffer - 1] = '\n';
	}

	// because we add data from file, so you need to set the flag
	is_data_saved_flag = DATA_NO_HAS_BEEN_SAVED;
}
