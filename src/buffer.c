#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/main.h"
#include "../include/misc.h"

/* 
	this function is a clone of the strlen function, but with one
	difference: it counts characters in the string until it 
	encounters a null character. While the strlen can stop counting
	at the end of line(\n) character, but there, when working with
	strings, this is not necessary.
 */
int buflen(char *buffer)
{
	long i;
	i = 0;
	while(buffer[i] != '\0')
		i++;

	return i;
}

bool is_buffer_empty(void)
{
	if(buffer == NULL)
		return true;
	else
		return false;
}

int get_buffer_size(void)
{
	if(!is_buffer_empty())
		return buflen(buffer) + 1;
	else
		return -1;
}

char* allocate_mem_for_buffer(int size)
{
	char *buffer = malloc(size * sizeof(char));
	if(buffer == NULL)
		fail("allocate_mem_for_buffer(): allocation error");
	return buffer;
}

char* reallocate_mem_for_buffer(char *buffer, int old_size, int add_to_old_size)
{
	int size = old_size + add_to_old_size;
	buffer = realloc(buffer, size);
	if(buffer == NULL)
		fail("reallocate_mem_for_buffer(): reallocation error");
	return buffer;
}

int get_position_at_line(int line)
{
	int nl = 1, i;
	int bufsize;

	if((bufsize = get_buffer_size()) == -1) {
		fprintf(stderr, "buffer is empty\n");
		return -1;
	}

	for(i = 0; i < bufsize; i++) {
		if(nl == line) {
			return i;
		}
		if(buffer[i] == '\n')
			nl++;
	}

	return -1;
}

int get_number_of_lines_in_buffer(void)
{
	int nl, i, bufsize;

	if((bufsize = get_buffer_size()) == -1) {
		fprintf(stderr, "buffer is empty\n");
		return -1;
	}

	nl = 1;
	for(i = 0; i < bufsize; i++) {
		if(buffer[i] == '\n')
			nl++;
	}
	return nl;
}

int get_number_of_line_expr(char *pos)
{
	int position;
	int bufsize;

	if((bufsize = get_buffer_size()) == -1) {
		fprintf(stderr, "buffer is empty\n");
	}

	if((position = expand_line_expr(pos)) == -1) {
		position = strtol(pos, NULL, 10);
	}

	if(position < 0 || position > bufsize) {
		printf("buffer is empty\n");
		return 0;
	}

	return position;
}
