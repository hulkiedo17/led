#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>
#include "../include/main.h"
#include "../include/io.h"
#include "../include/parser.h"
#include "../include/commands.h"
#include "../include/misc.h"
#include "../include/buffer.h"

char *buffer = NULL;

int main(int argc, char **argv)
{
	int result;
	long temp_val;
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
		// 'a' group of commands
		case APPEND_NL:
			add_data_to_buffer(SET_NEW_LINE_FLAG);
			break;
		case APPEND:
			add_data_to_buffer(UNSET_NEW_LINE_FLAG);
			break;
		// 'w' group of commands
		case WRITE:
			save_buffer_to_file(first_arg_for_command, "w+");
			break;
		case WRITE_A:
			save_buffer_to_file(first_arg_for_command, "a+");
			break;
		case FILL_BUF:
			fill_buffer_from_file(first_arg_for_command);
			break;
		// 'p' group of commands
		case PRINT_BUF:
			print_buffer(WITHOUT_LINE_NUM_IN_OUTPUT);
			break;
		case PRINT_BUF_WITH_NUM:
			print_buffer(WITH_LINE_NUM_IN_OUTPUT);
			break;
		case PRINT_FN:
			print_filename();
			break;
		// 's' group of commands
		case SET_PROMPT:
			set_prompt(first_arg_for_command);
			break;
		case SET_FILENAME:
			set_filename(first_arg_for_command);
			break;
		// 'i' group of commands
		case INSERT:
			temp_val = expand_pos_expr(first_arg_for_command);
			insert_to_buffer(temp_val, second_arg_for_command, UNSET_NEW_LINE_FLAG);
			break;
		case INSERT_NL:
			temp_val = expand_pos_expr(first_arg_for_command);
			insert_to_buffer(temp_val, second_arg_for_command, SET_NEW_LINE_FLAG);
			break;
		case INSERT_AFTER_LINE:
			insert_after_line(first_arg_for_command, second_arg_for_command, UNSET_NEW_LINE_FLAG);
			break;
		case INSERT_AFTER_LINE_NL:
			insert_after_line(first_arg_for_command, second_arg_for_command, SET_NEW_LINE_FLAG);
			break;
		// 'd' group of commands
		case DELETE_LINE:
			temp_val = strtol(first_arg_for_command, NULL, 10);
			buffer = delete_line(temp_val);
			break;
		case DELETE_IN_RANGE:
			buffer = delete_range(first_arg_for_command, second_arg_for_command);
			break;
		// other commands
		case CLEAN:
			if(buffer_allocated_memory_flag == BUFFER_HAS_MEMORY)
				buffer = clean_buffer();
			break;
		case BASIC_HELP:
			basic_command_help();
			break;
		case CLEAN_SCREEN:
			clean_screen();
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

		if(first_arg_for_command != NULL)
			free(first_arg_for_command);
		if(second_arg_for_command != NULL)
			free(second_arg_for_command);
		
		first_arg_for_command = NULL;
		second_arg_for_command = NULL;

		if(quit_flag == SET_QUIT_FLAG)
			break;
	}

	if(buffer != NULL)
		clean_buffer();

	if(prompt_pointer != NULL)
		free(prompt_pointer);
	if(filename_global != NULL)
		free(filename_global);
	return 0;
}

