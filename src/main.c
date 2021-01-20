#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <getopt.h>
#include "../include/main.h"
#include "../include/io.h"
#include "../include/parser.h"
#include "../include/buffer.h"
#include "../include/misc.h"

char *buffer = NULL;

int main(int argc, char **argv)
{
	int result;
	char *input_buffer = NULL;
	uint8_t quit_flag = UNSET_QUIT_FLAG;
	command_type_t user_command = UNKNOWN;
	const char* short_opt = "hvf:";
	const struct option long_opt[] = {
		{"help", no_argument, NULL, 'h'},
		{"version", no_argument, NULL, 'v'},
		{"filename", required_argument, NULL, 'f'},
		{NULL, 0, NULL, 0}
	};

	// handling command line options
	while((result = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1)
	{
		switch(result) {
		case 'h':
			help();
			break;
		case 'v':
			version();
			break;
		case 'f':
			filename_global = duplicate_string(optarg);
			break;
		default:
			printf("unknown option\n");
			break;
		}
	}

	// main loop
	while(1) {
		print_prompt();
		input_buffer = read_from_stream(stdin, UNSET_NEW_LINE_FLAG);

		/* 
			convert the command to a token, and then to a constant that 
			is returned by this function (command_parser()), and also, if 
			the command requires an argument, the after the operation of 
			this function it must be in a global variable (arg_for_command),
			if there is one, otherwise null.
		*/
		user_command = command_parser(input_buffer);

		// find the required token and execute the command
		switch(user_command) {
		case APPEND:
			add_data_to_buffer();
			is_data_saved_flag = DATA_NO_HAS_BEEN_SAVED;
			break;
		case CLEAN:
			if(buffer_allocated_memory_flag == BUFFER_HAS_MEMORY) {
				buffer = clean_buffer();
				buffer_allocated_memory_flag = BUFFER_HAS_NO_MEMORY;
				is_data_saved_flag = DATA_HAS_BEEN_SAVED;
			}
			break;
		case PRINT_BUF:
			line_numbers_flag = WITHOUT_LINE_NUM_IN_OUTPUT;
			print_buffer();
			break;
		case WRITE:
			save_buffer_to_file(arg_for_command, "w+");
			break;
		case WRITE_A:
			save_buffer_to_file(arg_for_command, "a+");
			break;
		case PRINT_FN:
			print_filename();
			break;
		case SET_PROMPT:
			set_prompt(arg_for_command);
			break;
		case SET_FILENAME:
			set_filename(arg_for_command);
			break;
		case FILL_BUF:
			fill_buffer_from_file(arg_for_command);
			break;
		case PRINT_BUF_WITH_NUM:
			line_numbers_flag = WITH_LINE_NUM_IN_OUTPUT;
			print_buffer();
			break;
		case QUIT:
			if(is_data_saved_flag == DATA_NO_HAS_BEEN_SAVED && buffer_allocated_memory_flag == BUFFER_HAS_MEMORY) {
				printf("please clean buffer or save data in buffer to file\n");
				break;
			}
			quit_flag = SET_QUIT_FLAG;
			break;
		case UNKNOWN:
		default:
			printf("unknown command\n");
			break;
		}

		free(arg_for_command);
		arg_for_command = NULL;

		if(quit_flag == SET_QUIT_FLAG)
			break;
	}

	if(buffer != NULL)
		clean_buffer();
	if(arg_for_command != NULL)
		free(arg_for_command);
	if(prompt_pointer != NULL)
		free(prompt_pointer);
	if(filename_global != NULL)
		free(filename_global);
	return 0;
}

