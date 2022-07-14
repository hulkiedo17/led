#ifndef MISC_H
#define MISC_H

void fail(FILE* out, const char* fmt, ...);
void warning(FILE* out, const char* fmt, ...);
void help(void);
void version(void);
int clean_screen(void);
void set_filename(const char* const string);
void print_filename(void);

#endif
