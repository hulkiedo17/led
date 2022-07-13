#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "misc.h"

static char* prompt_pointer = NULL;
static const char* const default_prompt = ">";

void print_prompt(void)
{
	if(prompt_pointer == NULL) {
		if(default_prompt == NULL) {
			fail(stderr, "error: null prompt pointers\n");
		}

		printf("%s ", default_prompt);
	} else {
		printf("%s ", prompt_pointer);
	}
}

void set_custom_prompt(const char* const string)
{
	if(string == NULL) {
		warning(stderr, "there is no prompt line\n");
		return;
	}

	if(prompt_pointer != NULL) {
		free(prompt_pointer);
		prompt_pointer = NULL;
	}

	size_t len = strlen(string) + 1;
	prompt_pointer = malloc(len * sizeof(char));
	if(prompt_pointer == NULL) {
		fail(stderr, "error: allocation error for prompt pointer\n");
	}

	strncpy(prompt_pointer, string, len);
}

void set_default_prompt(void)
{
	if(prompt_pointer != NULL) {
		free(prompt_pointer);
		prompt_pointer = NULL;
	}
}
