#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include "../include/main.h"

static const char* const  program_version = "0.1";

void fail(FILE* out, const char* fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(out, fmt, ap);
	va_end(ap);

	exit(EXIT_FAILURE);
}

void warning(FILE* out, const char* fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(out, fmt, ap);
	va_end(ap);
}

void help(void)
{
	puts("usage: led [options...]");
	putchar('\n');

	puts("[cli options]:");
	printf("\t-h (--help)    - prints this help message\n");
	printf("\t-v (--version) - shows the program version\n");
	printf("\t-f [file] (--filename) - set the filename by default\n");
	putchar('\n');

	puts("[program commands]:");
	printf("\tq  - quit\n");
	printf("\tsp [str] - set custom prompt\n");
	printf("\tsd - set default prompt\n");
	printf("\tcs - clear screen\n");
	printf("\tc  - clean buffer\n");
	printf("\tp  - print buffer\n");
	printf("\tpl - print buffer with numbered lines\n");
	printf("\ta  - append data to buffer(wihtout \\n)\n");
	printf("\tap - append data to buffer(with \\n)\n");
	printf("\tpc - print buffer by characters\n");
	printf("\tsf [file] - set filename\n");
	printf("\tpf - print filename\n");
	printf("\tw [file] - safe buffer to file\n");
	printf("\twa [file] - safe buffer to file(append, not rewrite)\n");
	printf("\tf [file] - fill buffer from file\n");
	printf("\tfn [file] - fill buffer from file(with \\n)\n");
	printf("\tia [line] [str] - insert data after specified line\n");
	printf("\tian [line] [str] - insert data after specified line(with \\n)\n");
	printf("\tib [line] [str] - insert data before specified line\n");
	printf("\tibn [line] [str] - insert data before specified line(with \\n)\n");
	printf("\tdl [line] - delete the specified line\n");
	printf("\tdr [line1] [line2] - delete the lines in the specified range\n");
	putchar('\n');
}

void version(void)
{
	printf("led version - v%s\n\n", program_version);
}

void clean_screen(void)
{
	system("clear");
}

char* dup_string(const char* const string)
{
	if(string == NULL) {
		warning(stderr, "warning: null string pointer\n");
		return NULL;
	}

	char* dup_string = NULL;
	size_t length;

	length = strlen(string) + 1;
	dup_string = malloc(length * sizeof(char));
	if(dup_string == NULL) {
		fail(stderr, "error: allocation eror for duplicated string\n");
	}
	
	strncpy(dup_string, string, length);
	return dup_string;
}

void set_filename(const char* const string)
{
	if(string == NULL) {
		warning(stderr, "warning: null string pointer\n");
		return;
	}

	if(global_filename != NULL) {
		free(global_filename);
	}

	global_filename = dup_string(string);
	if(global_filename == NULL) {
		fail(stderr, "error: null global filename pointer\n");
	}
}

void print_filename(void)
{
	printf("filename = %s\n", global_filename);
}
