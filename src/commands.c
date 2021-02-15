#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../include/io.h"
#include "../include/commands.h"
#include "../include/buffer.h"
#include "../include/main.h"
#include "../include/misc.h"

uint8_t buffer_allocated_memory_flag = BUFFER_HAS_NO_MEMORY;
uint8_t is_data_saved_flag = DATA_HAS_BEEN_SAVED;

#ifdef DEBUG
void print_characters(void) 
{
	if(buffer == NULL) {
		printf("buffer is empty\n");
		return;
	}

	for(long i = 0; i < buflen(buffer) + 1; i++) {
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

void print_buffer_size(void)
{
	if(buffer == NULL) {
		printf("buffer is empty\n");
		return;
	}

	printf("bufsize = %d\n", buflen(buffer) + 1);
}

void print_number_of_lines(void)
{
	if(buffer == NULL) {
		printf("buffer is empty\n");
		return;
	}

	printf("number of lines = %d\n", get_number_of_lines_in_buffer());
}

void print_position_at_line(char *line)
{
	int line_num, position;

	if(line == NULL) {
		printf("please type argument\n");
		return;
	}

	if((line_num = expand_line_expr(line)) == -1) {
		line_num = strtol(line, NULL, 10);
	}

	if((position = get_position_at_line(line_num)) == -1) {
		printf("out of lines\n");
		return;
	}

	printf("position at %d line = %d\n", line_num, position);
}
#endif

char* clean_buffer(void)
{
	if(buffer != NULL) {
		printf("freed memory: %d bytes\n", buflen(buffer) + 1);
		free(buffer);
		buffer = NULL;
	}

	buffer_allocated_memory_flag = BUFFER_HAS_NO_MEMORY;
	is_data_saved_flag = DATA_HAS_BEEN_SAVED;
	
	return NULL;
}

void print_buffer(uint8_t line_num_flag)
{
	long count_lines = 1;
	long number_of_lines = get_number_of_lines_in_buffer();

	if(is_buffer_empty()) {
		//printf("buffer is empty\n");
		return;
	} else {
		if(line_num_flag == WITH_LINE_NUM_IN_OUTPUT) {
			/* how works the output with line numbers:

				before the loop, number of the first line is
				displayed(the number of lines is taken into 
				account for a more beautiful output), then 
				the loop goes through the buffer and each 
				character in the array is checked for equality 
				to a newline character, if the equality is true, 
				then the number of the next line is displayed 
				after it, then it is incremented.
			*/

			if(number_of_lines < 10) {
				printf("%ld ", count_lines);
			} else if(number_of_lines >= 10 && number_of_lines < 100) {
				printf("%2ld ", count_lines);
			} else if(number_of_lines >= 100 && number_of_lines < 1000) {
				printf("%3ld ", count_lines);
			}
			count_lines++;

			for(long i = 0; i < buflen(buffer); i++) {
				putchar(buffer[i]);

				if(buffer[i] == '\n') {
					if(number_of_lines < 10) {
						printf("%ld ", count_lines);
					} else if(number_of_lines >= 10 && number_of_lines < 100) {
						printf("%2ld ", count_lines);
					} else if(number_of_lines >= 100 && number_of_lines < 1000) {
						printf("%3ld ", count_lines);
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

void add_data_to_buffer(uint8_t nl_flag)
{
	char *input_buffer = NULL;
	int buffer_size = 0;
	int input_buffer_size = 0;

	// read line from standard input
	input_buffer = read_from_stream(stdin, UNSET_NEW_LINE_FLAG);
	while(strcmp(input_buffer, ".") != 0) {
		// if buffer doesn't have some data, allocate the memory
		// and copy data from input_buffer to main buffer
		if(buffer_allocated_memory_flag == BUFFER_HAS_NO_MEMORY) {
			input_buffer_size = buflen(input_buffer) + 1;
			buffer = allocate_mem_for_buffer(input_buffer_size);
			buffer_allocated_memory_flag = BUFFER_HAS_MEMORY;

			strncpy(buffer, input_buffer, input_buffer_size);
		} else {
			// else, if buffer have some data, check the main buffer size,
			// and then reallocate memory in main buffer, and copy the data
			// from the input_buffer to the new allocated memory in the main 
			// buffer.
			if(nl_flag == SET_NEW_LINE_FLAG)
				input_buffer_size = buflen(input_buffer) + 1;
			else
				input_buffer_size = buflen(input_buffer);

			buffer_size = get_buffer_size();
			buffer = reallocate_mem_for_buffer(buffer, buffer_size, input_buffer_size);

			if(nl_flag == SET_NEW_LINE_FLAG)
				strncpy(buffer + buffer_size, input_buffer, input_buffer_size);
			else
				strncpy(buffer + buffer_size - 1, input_buffer, input_buffer_size);

			// and don't forget to replace the \0 to \n or without them,
			// because \0 character in file is not a good practice
			if(nl_flag == SET_NEW_LINE_FLAG)
				buffer[buffer_size - 1] = '\n';
		}

		// free used memory in input_buffer and read new line from standard input
		free(input_buffer);
		input_buffer = read_from_stream(stdin, UNSET_NEW_LINE_FLAG);
	}

	free(input_buffer);
	is_data_saved_flag = DATA_NO_HAS_BEEN_SAVED;
}

void save_buffer_to_file(char *filename, char *mode)
{
	FILE *fp = NULL;
	char *current_filename = NULL;
	int len;

	if((len = get_buffer_size()) == -1) {
		printf("buffer is empty\n");
		return;
	}

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

	/* 
		one is substracted from the length because we cannot
		pu a \0 character in the file
	*/
	if(write_to_file(buffer, len - 1, fp) < 0)
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

	if((len_buffer = get_buffer_size()) == -1) {
		len_buffer = 0;
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

	free(temporary_buffer);

	// because we add data from file, so you need to set the flag
	is_data_saved_flag = DATA_NO_HAS_BEEN_SAVED;
}

char* insert_to_buffer(int position, char *data, int nl_flag)
{
	if(data == NULL) {
		printf("insert(): null options pointers\n");
		return buffer;
	}

	char *temp_buf = NULL;
	int buf_len;
	size_t data_len;

	if(nl_flag == UNSET_NEW_LINE_FLAG)
		data_len = buflen(data);
	else
		data_len = buflen(data) + 1;	

	if(buffer_allocated_memory_flag == BUFFER_HAS_MEMORY) {
		if((buf_len = get_buffer_size()) == -1) {
			fprintf(stderr, "insert_to_buffer(): get_buffer_size() failed\n");
			return buffer;
		}
	}

	if(buffer_allocated_memory_flag == BUFFER_HAS_MEMORY) {
		/* 
			checking for the possibility to insert data at the
			specified position
		*/
		if(position < 0 || position >= buf_len) {
			printf("insert(): out of buffer\n");
			return buffer;
		}

		temp_buf = allocate_mem_for_buffer(data_len + buf_len);

		/* 
			1 strncpy - inserting data from the buffer into a temporary buffer
			up to the position after which other data should be inserted
			2 strncpy - inserting other data into a temporary buffer
			3 strncpy - inserting the remainder of the buffer data into a 
			temporary buffer
		*/
		strncpy(temp_buf, buffer, position);
		strncpy(temp_buf + position, data, data_len);
		strncpy(temp_buf + position + data_len, buffer + position, buf_len - position);

		/* 
			replacing the \0 char with a \n character or insert without them
			so that there are no conflicts when working with the buffer.
		*/
		if(nl_flag == SET_NEW_LINE_FLAG) {
			temp_buf[position + data_len - 1] = '\n';
		}

		/* 
			freeing data in the buffer and allocating new memory for copying
			to the data buffer from the temporary buffer
		*/
		free(buffer);
		buffer = allocate_mem_for_buffer(data_len + buf_len);
		strncpy(buffer, temp_buf, data_len + buf_len);
		free(temp_buf);
	} else {
		/* 
			if there is no data in the buffer, then just add this data to the buffer,
			regardless of the specified position when calling the command
		*/
		buffer = allocate_mem_for_buffer(data_len);
		strncpy(buffer, data, data_len);
		buffer_allocated_memory_flag = BUFFER_HAS_MEMORY;
	}

	is_data_saved_flag = DATA_NO_HAS_BEEN_SAVED;
	return buffer;
}

char* insert_after_line(char* line, char* data, int nl_flag)
{
	int number_of_line, position, all_lines;

	if((number_of_line = expand_line_expr(line)) == -1) {
		number_of_line = strtol(line, NULL, 10);
	}

	all_lines = get_number_of_lines_in_buffer();
	if(number_of_line <= 0 && number_of_line > all_lines) {
		printf("out of lines\n");
		return buffer;
	}

	if(number_of_line == all_lines) {
		position = get_buffer_size() - 1;
	} else {
		position = get_position_at_line(number_of_line + 1);
	}

	buffer = insert_to_buffer(position, data, nl_flag);
	return buffer;
}

char* delete_line(int line_num)
{
	int start_position, end_position, bufsize, temp_buffer_len, number_of_lines;
	char* temp_buffer = NULL;

	if((bufsize = get_buffer_size()) == -1) {
		fprintf(stderr, "buffer is empty\n");
		return buffer;
	}

	number_of_lines = get_number_of_lines_in_buffer();

	/* 
		1) if the reqired line is in the range from 1 to number_of_lines - 1, 
		then you can easily find the position of the beginning and end of the
		string in the buffer using the function
		2) if the desired line is the last one in the buffer, then the starting
		position is as before, but the last position will be found simply by
		finding the size of the buffer
		3) if the required line is not in the buffer (outside the range of lines),
		then a message is displayed and the functuin is exited
	*/
	if(line_num < number_of_lines && line_num > 0) {
		if((start_position = get_position_at_line(line_num)) == -1) {
			return buffer;
		}

		if((end_position = get_position_at_line(line_num + 1)) == -1) {
			return buffer;
		}
	} else if(line_num == number_of_lines) {
		if((start_position = get_position_at_line(line_num)) == -1) {
			return buffer;
		}

		end_position = get_buffer_size();
	} else {
		printf("out of lines\n");
		return buffer;
	}

	temp_buffer_len = bufsize - (end_position - start_position);
	if(temp_buffer_len > 0) {
		temp_buffer = allocate_mem_for_buffer(temp_buffer_len);
	} else {
		// if temp_buffer_len == 0, then we need to clean the entire buffer
		free(buffer);
		buffer = NULL;
		buffer_allocated_memory_flag = BUFFER_HAS_NO_MEMORY;
		is_data_saved_flag = DATA_HAS_BEEN_SAVED;
		return buffer;
	}

	strncpy(temp_buffer, buffer, start_position);

	if(line_num < number_of_lines) {
		// if we delete not the last line, then you need to copy the rest of the buffer
		strncpy(temp_buffer + start_position, buffer + end_position, bufsize - end_position);
	} else {
		// if we delete the last line, just put \0
		temp_buffer[start_position - 1] = '\0';
	}

	free(buffer);
	buffer = allocate_mem_for_buffer(temp_buffer_len);
	strncpy(buffer, temp_buffer, temp_buffer_len);

	free(temp_buffer);

	is_data_saved_flag = DATA_NO_HAS_BEEN_SAVED;
	return buffer;
}

char* delete_range(char* start_pos, char* end_pos)
{
	if(start_pos == NULL || end_pos == NULL) {
		printf("delete_range(): null option pointers\n");
		return buffer;
	}

	if(buffer == NULL) {
		printf("buffer is empty\n");
		return buffer;
	}

	int number_of_lines, start_line, end_line;

	number_of_lines = get_number_of_lines_in_buffer();
	start_line = get_number_of_line_expr(start_pos);
	end_line = get_number_of_line_expr(end_pos);

	if(start_line > 0 && end_line <= number_of_lines) {
		if(start_line > end_line) {
			printf("error in the specified range\n");
			return buffer;
		}
	} else {
		printf("out of lines\n");
		return buffer;
	}

	/*
		if only one line is specified in the range (that is, from 
		1 to 1 for example), then only it will be removed from 
		the buffer

		otherwise, lines in the specified range will be deleted
		using a loop
	*/
	if(start_line == end_line) {
		buffer = delete_line(start_line);
	} else {
		for(int i = 0; i <= (end_line - start_line); i++) {
			buffer = delete_line(start_line);
		}
	}

	return buffer;
}

char* delete_substring(char *substring)
{
	int str_len = buflen(substring) + 1;
	int buf_len = buflen(buffer) + 1;
	int temp_buf_len, diff_ps, temp_copy_size;
	char* ps = NULL;
	char *temp_buf = NULL;

	if((ps = strstr(buffer, substring)) != NULL) {
		temp_buf_len = buf_len - str_len;
		temp_buf = allocate_mem_for_buffer(temp_buf_len);
		diff_ps = ps - buffer;

		if(diff_ps == 0 && (buf_len == str_len)) {
			free(buffer);
			buffer = NULL;
			free(temp_buf);
			is_data_saved_flag = DATA_HAS_BEEN_SAVED;
			buffer_allocated_memory_flag = BUFFER_HAS_NO_MEMORY;
			return buffer;
		}

		strncpy(temp_buf, buffer, diff_ps);

		temp_copy_size = buf_len - diff_ps - str_len;
		if(temp_copy_size == 0) {
			temp_buf[diff_ps - 1] = '\0';
		} else {
			strncpy(temp_buf + diff_ps, buffer + diff_ps + str_len, temp_copy_size);
		}

		free(buffer);
		buffer = malloc(temp_buf_len);
		if(buffer == NULL) {
			fprintf(stderr, "E: allocation error\n");
			exit(1);
		}

		strncpy(buffer, temp_buf, temp_buf_len);
		free(temp_buf);

		is_data_saved_flag = DATA_NO_HAS_BEEN_SAVED;
	} else {
		printf("string does not have the specified string\n");
	}

	return buffer;
}