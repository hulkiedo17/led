#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>
#include <stdbool.h>
#include "misc.h"
#include "prompt.h"
#include "io.h"
#include "parser.h"
#include "main.h"
#include "buffer.h"
#include "append.h"
#include "file.h"
#include "insert.h"
#include "delete.h"
#include "execute.h"

char* global_buffer = NULL;
char* global_filename = NULL;
bool is_data_saved = true;

int main(int argc, char **argv)
{
	int result;
	tokens_t token = { UNKNOWN_TYPE, UNKNOWN_TOKEN };
	char* input_buffer = NULL;

	while((result = getopt(argc, argv, "hvf:")) != -1) {
		switch(result) {
		case 'h':
			help();
			exit(EXIT_SUCCESS);
		case 'v':
			version();
			exit(EXIT_SUCCESS);
		case 'f':
			set_filename(optarg);
			break;
		default:
			printf("-%c - unknown option\n", optopt);
			break;
		}
	}

	while(1) {
		print_prompt();

		input_buffer = read_from_stream(stdin, DONT_SKIP_NEW_LINE);
		token = parse_command(input_buffer);
		free(input_buffer);

		if(execute_command(token) == QUIT_FLAG) {
			break;
		}
	}

	free_buffer_pointers();
	return 0;
}
