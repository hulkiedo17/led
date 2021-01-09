#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/misc.h"

static char* program_version = "0.1";
char* filename_global = NULL;
char* prompt_pointer = NULL;
const char* default_prompt = ">";

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
    printf("c - clean the buffer(just free all allocated memory)\n");
    printf("q - quit from editor(you quit from editor if you saved all data to file or cleaned the buffer)\n");
    printf("p - print buffer to output\n");
    printf("w [filename] - write to file \"filename\"(\"filename\" is optional in arguments)\n");
    printf("wa[filename] - the same as the previous oe, but only the file is not overwritten before adding data\n");
    printf("pf - print the filename\n");
    printf("sf [filename] - set filename\n");
    printf("sp [prompt_str] - set prompt\n");
	printf("fb [filename] - fill buffer from file\n");
    putchar(10);
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

int is_valid_filename(char *fn)
{
    if(fn == NULL)
        fail("is_valid_filename(): null pointer");

    if(!strchr(fn, '/'))
        return 1;
    else
        return 0;
}

void set_filename(char *string)
{
    if(string == NULL)
        fail("set_filename(): null pointer");

    if(filename_global != NULL)
        free(filename_global);

    if(!is_valid_filename(string)) {
        printf("not valid filename, please type another name\n");
        return;
    }

    filename_global = duplicate_string(string);
    if(filename_global == NULL)
        fail("set_filename(): strdup() error");
}
