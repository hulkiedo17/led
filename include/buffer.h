#ifndef BUFFER_H
#define BUFFER_H

int buflen(char *buffer);
bool is_buffer_empty(void);
int get_buffer_size(void);
char* allocate_mem_for_buffer(int size);
char* reallocate_mem_for_buffer(char *buffer, int old_size, int add_to_old_size);
int get_position_at_line(int line);
int get_number_of_lines_in_buffer(void);
int get_number_of_line_expr(char *pos);
int get_position_for_next_line(int line_num);

#endif

