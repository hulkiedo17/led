#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/misc.h"
#include "../include/buffer.h"
#include "../include/main.h"

char* filename_global = NULL;
char* prompt_pointer = NULL;
const char* default_prompt = ">";
static char* program_version = "0.1";
static const char* line_addr_expr[] = {
	".", "$", "/"
};

void fail(const char *string)
{
    fprintf(stderr, "%s\n", string);
    exit(EXIT_FAILURE);
}

void print_filename(void)
{
	printf("filename = %s\n", filename_global);
}

void help(void)
{
    printf("usage: editor [filename] [--help|h]\n");
    printf("the \"filename\" option is optional, just like the rest of the parameters\n\n");
    
    printf("commands:\n");
    printf("a - add a text to buffer(to stop, type the '.' in first position in new line)\n");
    printf("an - same as the previous, but without \\n at end\n");
    printf("c - clean the buffer(just free all allocated memory)\n");
    printf("q - quit from editor(you quit from editor if you saved all data to file or cleaned the buffer)\n");
    printf("p - print buffer to output\n");
    printf("pl - print buffer with number of lines\n");
    printf("pf - print the current file name\n");
    printf("w  [filename] - write to file \"filename\"(\"filename\" is optional in arguments)\n");
    printf("wa [filename] - the same as the previous oe, but only the file is not overwritten before adding data\n");
    printf("wb [fn] - append data from the specified file to the buffer\n");
    printf("sf [filename] - set filename\n");
    printf("sp [prompt_str] - set prompt\n");
    printf("i [position] [data] - insert data to the buffer at the specified position\n");
    printf("in [position] [data] - same as the previous, but here added \\n at the end of the inserted data\n");
    printf("dl [line_number] - delete the specified line from the buffer\n");
    printf("h - basic help reference about commands\n");
    printf("cs - clean screen\n");
    putchar(10);
}

void basic_command_help(void)
{
    printf("commands:\n");
    printf("a - append data to the buffer\n");
    printf("an - append data to the buffer (without \\n)\n");
    printf("c - clean the buffer\n");
    printf("q - quit from the editor\n");
    printf("p - print buffer\n");
    printf("w [fn] - write the buffer data to the specified file(or current file)\n");
    printf("wa [fn] - same as the prev command, but only append data to the file\n");
    printf("wb [fn] - append data from the specified file to the buffer\n");
    printf("sf [str] - set current filename\n");
    printf("sp [str] - set prompt\n");
    printf("i [pos] [data] - insert data to the buffer at the specified position\n");
    printf("in [pos] [data] - same as the previous, but here added \\n at the end of the inserted data\n");
    printf("dl [line] - delete the specified line from the buffer\n");
    printf("h - basic help reference about commands\n");
    printf("cs - clean screen\n");
}

void version(void)
{
    printf("Editor version - v%s\n\n", program_version);
}

void print_prompt(void)
{
    if(prompt_pointer == NULL) {
        if(default_prompt == NULL)
            fail("error: null prompt pointer\n");
        printf("%s ", default_prompt);
    } else {
        printf("%s ", prompt_pointer);
    }
}

void set_prompt(char* string)
{
    if(string == NULL) {
        if(prompt_pointer != NULL) {
            free(prompt_pointer);
            prompt_pointer = NULL;
        }
        return;
    }

    char *prompt;
    size_t len;

    len = strlen(string) + 1;
    prompt = malloc(len * sizeof(char));
    if(prompt == NULL)
        fail("set_prompt(): allocation error\n");

    if(prompt_pointer != NULL)
        free(prompt_pointer);

    strncpy(prompt, string, len);
    prompt_pointer = prompt;
}

char* duplicate_string(char *str)
{
    if(str == NULL)
        fail("duplicate_string(): null pointer");
    
    char *dup;
    size_t len;

    len = strlen(str) + 1;
    dup = malloc(len * sizeof(char));
    if(dup == NULL)
        fail("duplicate_string(): allocation error");

    strncpy(dup, str, len);
    return dup;
}

void set_filename(char *string)
{
    if(string == NULL)
        fail("set_filename(): null pointer");

    if(filename_global != NULL)
        free(filename_global);

    filename_global = duplicate_string(string);
    if(filename_global == NULL)
        fail("set_filename(): strdup() error");
}

void clean_screen(void)
{
    system("clear");
}

int expand_pos_expr(char *expr)
{
	if(strcmp(expr, line_addr_expr[0]) == 0)
		return 0;
	else if(strcmp(expr, line_addr_expr[1]) == 0)
		return buflen(buffer);
	else if(strcmp(expr, line_addr_expr[2]) == 0)
		return buflen(buffer) / 2;

	return -1;
}

int expand_line_expr(char *expr)
{
	if(strcmp(expr, line_addr_expr[0]) == 0)
		return 1;
	else if(strcmp(expr, line_addr_expr[1]) == 0)
		return get_number_of_lines_in_buffer();
	else if(strcmp(expr, line_addr_expr[2]) == 0)
		return get_number_of_lines_in_buffer() / 2;

	return -1;
}

char* choose_filename(char *filename)
{
	if(filename == NULL) {
		if(filename_global == NULL) {
			return NULL;
		}

		return filename_global;
	} 
    
	return filename;
}