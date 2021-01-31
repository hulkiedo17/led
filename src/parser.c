#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../include/parser.h"
#include "../include/misc.h"

static const char* command_list[] = {
	"a", "an", "c", "q", "p", "w", 
	"wa", "pf", "sf", "sp", "wb", "pl", 
	"i", "in", "dl", "h", "cs", "dr",
	"il", "iln"
};

char* first_arg_for_command = NULL;
char* second_arg_for_command = NULL;
static uint8_t command_first_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
static uint8_t command_second_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;

command_type_t command_parser(char* input_buffer)
{
	char *token1_command = NULL;
	char *token2_argument_1 = NULL;
	char *token3_argument_2 = NULL;
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
	token2_argument_1 = strtok(NULL, " ");
	token3_argument_2 = strtok(NULL, " ");

	if(token1_command != NULL)
		command_token = get_command_token(token1_command);		

	if((command_first_argument_flag != COMMAND_DONT_REQUIRES_AN_ARG) && (token2_argument_1 != NULL))
	{
		if(first_arg_for_command != NULL)
			free(first_arg_for_command);

		first_arg_for_command = malloc((strlen(token2_argument_1) + 1) * sizeof(char));
		if(first_arg_for_command == NULL)
			fail("command_parser(): allocation error");

		strncpy(first_arg_for_command, token2_argument_1, strlen(token2_argument_1) + 1);
	}

	if((command_second_argument_flag != COMMAND_DONT_REQUIRES_AN_ARG) && (token3_argument_2 != NULL))
	{
		if(second_arg_for_command != NULL)
			free(second_arg_for_command);

		second_arg_for_command = malloc((strlen(token3_argument_2) + 1) * sizeof(char));
		if(second_arg_for_command == NULL)
			fail("command_parser(): allocation error");

		strncpy(second_arg_for_command, token3_argument_2, strlen(token3_argument_2) + 1);
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
		command_first_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		command_second_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		return APPEND_NL;
	} else if(strcmp(token, command_list[1]) == 0) {
		command_first_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		command_second_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		return APPEND;
	} else if(strcmp(token, command_list[2]) == 0) {
		command_first_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		command_second_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		return CLEAN;
	} else if(strcmp(token, command_list[3]) == 0) {
		command_first_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		command_second_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		return QUIT;
	} else if(strcmp(token, command_list[4]) == 0) {
		command_first_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		command_second_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		return PRINT_BUF;
	} else if(strcmp(token, command_list[5]) == 0) {
		command_first_argument_flag = COMMAND_REQUIRES_AN_ARG;
		command_second_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		return WRITE;
	} else if(strcmp(token, command_list[6]) == 0) {
		command_first_argument_flag = COMMAND_REQUIRES_AN_ARG;
		command_second_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		return WRITE_A;
	} else if(strcmp(token, command_list[7]) == 0) {
		command_first_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		command_second_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		return PRINT_FN;
	} else if(strcmp(token, command_list[8]) == 0) {
		command_first_argument_flag = COMMAND_REQUIRES_AN_ARG;
		command_second_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		return SET_FILENAME;
	} else if(strcmp(token, command_list[9]) == 0) {
		command_first_argument_flag = COMMAND_REQUIRES_AN_ARG;
		command_second_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		return SET_PROMPT;
	} else if(strcmp(token, command_list[10]) == 0) {
		command_first_argument_flag = COMMAND_REQUIRES_AN_ARG;
		command_second_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		return FILL_BUF;
	} else if(strcmp(token, command_list[11]) == 0) {
		command_first_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		command_second_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		return PRINT_BUF_WITH_NUM;
	} else if(strcmp(token, command_list[12]) == 0) {
		command_first_argument_flag = COMMAND_REQUIRES_AN_ARG;
		command_second_argument_flag = COMMAND_REQUIRES_AN_ARG;
		return INSERT;
	} else if(strcmp(token, command_list[13]) == 0) {
		command_first_argument_flag = COMMAND_REQUIRES_AN_ARG;
		command_second_argument_flag = COMMAND_REQUIRES_AN_ARG;
		return INSERT_NL;
	} else if(strcmp(token, command_list[14]) == 0) {
		command_first_argument_flag = COMMAND_REQUIRES_AN_ARG;
		command_second_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		return DELETE_LINE;
	} else if(strcmp(token, command_list[15]) == 0) {
		command_first_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		command_second_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		return BASIC_HELP;
	} else if(strcmp(token, command_list[16]) == 0) {
		command_first_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		command_second_argument_flag = COMMAND_DONT_REQUIRES_AN_ARG;
		return CLEAN_SCREEN;
	} else if(strcmp(token, command_list[17]) == 0) {
		command_first_argument_flag = COMMAND_REQUIRES_AN_ARG;
		command_second_argument_flag = COMMAND_REQUIRES_AN_ARG;
		return DELETE_IN_RANGE;
	} else if(strcmp(token, command_list[18]) == 0) {
		command_first_argument_flag = COMMAND_REQUIRES_AN_ARG;
		command_second_argument_flag = COMMAND_REQUIRES_AN_ARG;
		return INSERT_AFTER_LINE;
	} else if(strcmp(token, command_list[19]) == 0) {
		command_first_argument_flag = COMMAND_REQUIRES_AN_ARG;
		command_second_argument_flag = COMMAND_REQUIRES_AN_ARG;
		return INSERT_AFTER_LINE_NL;
	}

	return UNKNOWN;
}
