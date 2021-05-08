#ifndef BUFFER_H
#define BUFFER_H

#define OUTPUT_NUMBERED_LINES 1
#define DONT_OUTPUT_NUMBERED_LINES 0

bool is_buffer_empty(const char* const buffer);
size_t buflen(const char* const buffer);
char* alloc_buffer(size_t size);
char* realloc_buffer(char* buffer, size_t old_size, size_t size_of_added_data);
size_t get_buffer_size(const char* const buffer);
size_t get_number_of_lines(const char* const buffer);
int get_position_at_line(const char* const buffer, size_t line_number);
void clean_buffer(void);
void print_buffer(const char* const buffer, char* line, uint8_t numbered_lines_flag);
void print_buffer_by_char(const char* const buffer);

#endif
