#ifndef MISC_H
#define MISC_H

void fail(FILE* out, const char* fmt, ...);
void warning(FILE* out, const char* fmt, ...);
void help(void);
void version(void);
void clean_screen(void);
char* dup_string(const char* const str);
void set_filename(const char* const string);
void print_filename(void);

#endif
