#ifndef MISC_H
#define MISC_H

extern char* filename_global;
extern char* prompt_pointer;
extern const char* default_prompt;

void fail(const char *string);
void print_filename(void);
void help(void);
void basic_command_help(void);
void version(void);
void print_prompt(void);
void set_prompt(char* string);
char* duplicate_string(char *str);
int is_valid_filename(char *fn);
void set_filename(char *string);
void clean_screen(void);
int expand_pos_expr(char *expr);
int expand_line_expr(char *expr);
char* choose_filename(char *filename);

#endif

