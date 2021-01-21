#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../include/parser.h"
#include "../include/misc.h"

static const char* command_list[] = {
	"a", "c", "q", "p", "w", "wa", "pf", "sf", "sp", "wb", "pl"
};

char* arg_for_command = NULL;
static uint8_t command_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;

command_type_t command_parser(char* input_buffer)
{
	char *token1_command = NULL;
	char *token2_argument = NULL;
	command_type_t command_token = UNKNOWN;

	/*
		using the strtok function, the input_buffer is split into 
		two tokens (1 - the command itself, 2 - the command arguments, if any),
		and then using the auxiliary functions, the constant of this command is
		taken and returned to the calling function.

		the command argument is processed in this function, provided that the 
		command requires an argument and the token is not equal to zero (that is,
		it is checked whether the second token exists at all or not) and if the
		conditions are true, then memory is allocated for it into a global var.
	*/

	token1_command = strtok(input_buffer, " ");
	token2_argument = strtok(NULL, " ");

	if(token1_command != NULL)
		command_token = get_command_token(token1_command);		

	if((command_argument_flag != COMMAND_DONT_REQUIRES_AN_ARG) && (token2_argument != NULL))
	{
		if(arg_for_command != NULL)
			free(arg_for_command);

		arg_for_command = malloc((strlen(token2_argument) + 1) * sizeof(char));
		if(arg_for_command == NULL)
			fail("parse_command(): allocation error");

		strncpy(arg_for_command, token2_argument, strlen(token2_argument) + 1);
	}

	free(input_buffer);
	return command_token;
}

command_type_t get_command_token(char *token)
{
	if(token == NULL)
		fail("get_command_token(): null pointer");

	/* 
		the token is compared with the commands, the data is in the
		command_list[] array, and if the comparsion was successful, 
		then the command_argument_flag flag is marked depending on 
		the command (whether the command has additional arguments)
		and returns a constant with the command number.
	*/
	if(strcmp(token, command_list[0]) == 0) {
		command_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		return APPEND;
	} else if(strcmp(token, command_list[1]) == 0) {
		command_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		return CLEAN;
	} else if(strcmp(token, command_list[2]) == 0) {
		command_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		return QUIT;
	} else if(strcmp(token, command_list[3]) == 0) {
		command_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		return PRINT_BUF;
	} else if(strcmp(token, command_list[4]) == 0) {
		command_argument_flag = COMMAND_REQUIRES_AN_ARG;
		return WRITE;
	} else if(strcmp(token, command_list[5]) == 0) {
		command_argument_flag = COMMAND_REQUIRES_AN_ARG;
		return WRITE_A;
	} else if(strcmp(token, command_list[6]) == 0) {
		command_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		return PRINT_FN;
	} else if(strcmp(token, command_list[7]) == 0) {
		command_argument_flag = COMMAND_REQUIRES_AN_ARG;
		return SET_FILENAME;
	} else if(strcmp(token, command_list[8]) == 0) {
		command_argument_flag = COMMAND_REQUIRES_AN_ARG;
		return SET_PROMPT;
	} else if(strcmp(token, command_list[9]) == 0) {
		command_argument_flag = COMMAND_REQUIRES_AN_ARG;
		return FILL_BUF;
	} else if(strcmp(token, command_list[10]) == 0) {
		command_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		return PRINT_BUF_WITH_NUM;
	}

	return UNKNOWN;
}
