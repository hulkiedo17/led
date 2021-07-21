#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>
#include <stdbool.h>
#include "../include/misc.h"
#include "../include/prompt.h"
#include "../include/io.h"
#include "../include/parser.h"
#include "../include/main.h"
#include "../include/buffer.h"
#include "../include/append.h"
#include "../include/file.h"
#include "../include/insert.h"
#include "../include/delete.h"
#include "../include/execute.h"

char* global_buffer = NULL;
char* global_filename = NULL;
bool is_data_saved = true;

int main(int argc, char **argv)
{
	int result;
	tokens_t token = { UNKNOWN_TYPE, UNKNOWN_TOKEN };
	char* input_buffer = NULL;
	const char* short_opt = "hvf:";
	const struct option long_opt[] = {
		{"help", no_argument, NULL, 'h'},
		{"version", no_argument, NULL, 'v'},
		{"filename", required_argument, NULL, 'f'},
		{NULL, 0, NULL, 0}
	};

	while((result = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1) {
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
