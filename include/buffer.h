#ifndef BUFFER_H
#define BUFFER_H

#define OUTPUT_NUMBERED_LINES 1
#define DONT_OUTPUT_NUMBERED_LINES 0

bool is_buffer_empty(void);
size_t buflen(const char* const buffer);
char* alloc_buffer(size_t size);
char* realloc_buffer(char* buffer, size_t old_size, size_t size_of_added_data);
size_t get_buffer_size(void);
size_t get_number_of_lines(void);
int get_position_at_line(size_t line_number);
void clean_buffer(void);
void print_buffer(char* line, uint8_t numbered_lines_flag);
void print_buffer_by_char(void);

#endif
