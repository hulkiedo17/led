#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include "main.h"
#include "buffer.h"

static const char* const  program_version = "0.4";

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
	printf("usage: led [options...]\n\n");

	printf("[cli options]:\n");
	printf("\t-h        - prints this help message\n");
	printf("\t-v        - shows the program version\n");
	printf("\t-f [file] - set the filename by default\n\n");

	printf("[program commands]:\n");
	printf("\tq  - quit\n");
	printf("\tsp [str] - set custom prompt\n");
	printf("\tsd - set default prompt\n");
	printf("\tsf [file] - set filename\n");
	printf("\tc  - clean buffer\n");
	printf("\tp  - print buffer\n");
	printf("\tpl - print buffer with numbered lines\n");
	printf("\tpc - print buffer by characters\n");
	printf("\tpf - print filename\n");
	printf("\ta  - append data to buffer(wihtout \\n)\n");
	printf("\tan - append data to buffer(with \\n)\n");
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
}

void version(void)
{
	printf("led version - v%s\n", program_version);
}

void set_filename(const char* const string)
{
	if(string == NULL) {
		warning(stderr, "there is no filename\n");
		return;
	}

	if(global_filename != NULL) {
		free(global_filename);
	}

	global_filename = strdup(string);
	if(global_filename == NULL) {
		fail(stderr, "error: null global filename pointer\n");
	}
}

void print_filename(void)
{
	if(global_filename == NULL) {
		printf("filename not specified\n");
	} else {
		printf("filename = %s\n", global_filename);
	}
}
