#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>
#include <stdbool.h>
#include "misc.h"
#include "prompt.h"
#include "parser.h"
#include "main.h"
#include "buffer.h"
#include "execute.h"

char* global_buffer = NULL;
char* global_filename = NULL;
bool is_data_saved = true;

static void parse_arguments(int argc, char **argv)
{
	int result;

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
}

static void editor_loop(void)
{
	tokens_t token = { UNKNOWN_TYPE, UNKNOWN_TOKEN };
	size_t size = 0;
	char* line = NULL;

	print_prompt();
	while(getline(&line, &size, stdin) != -1) {
		token = parse_command(line);

		if(execute_command(token) == QUIT_FLAG)
			break;

		print_prompt();
	}

	free(line);
	free_buffer_pointers();
}

int main(int argc, char **argv)
{
	parse_arguments(argc, argv);

	editor_loop();

	return 0;
}
