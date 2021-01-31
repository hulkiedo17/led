#ifndef PARSER_H
#define PARSER_H

#define SET_QUIT_FLAG 1
#define UNSET_QUIT_FLAG 0

#define COMMAND_REQUIRES_AN_ARG 1
#define COMMAND_DONT_REQUIRES_AN_ARG 0

typedef enum command_type {
    UNKNOWN = 0,
    QUIT,
    APPEND,
    APPEND_NL,
    CLEAN,
    PRINT_BUF,
    WRITE,
    WRITE_A,
    PRINT_FN,
    SET_PROMPT,
    SET_FILENAME,
    FILL_BUF,
    PRINT_BUF_WITH_NUM,
    INSERT,
    INSERT_NL,
    INSERT_AFTER_LINE,
    INSERT_AFTER_LINE_NL,
    DELETE_LINE,
    BASIC_HELP,
    CLEAN_SCREEN,
    DELETE_IN_RANGE
} command_type_t;

extern char* first_arg_for_command;
extern char* second_arg_for_command;

command_type_t command_parser(char* input_buffer);
command_type_t get_command_token(char *token);

#endif

