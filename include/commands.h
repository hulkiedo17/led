#ifndef COMMANDS_H
#define COMMANDS_H

#define BUFFER_HAS_MEMORY 1
#define BUFFER_HAS_NO_MEMORY 0

#define DATA_HAS_BEEN_SAVED 1
#define DATA_NO_HAS_BEEN_SAVED 0

#define WITH_LINE_NUM_IN_OUTPUT 1
#define WITHOUT_LINE_NUM_IN_OUTPUT 0

extern uint8_t buffer_allocated_memory_flag;
extern uint8_t is_data_saved_flag;

void add_data_to_buffer(uint8_t nl_flag);
char* clean_buffer(void);
void print_buffer(uint8_t line_num_flag);
void save_buffer_to_file(char *filename, char *mode);
void fill_buffer_from_file(char *filename);
char* insert_to_buffer(int position, char *data, int nl_flag);
char* delete_line(int line_num);
char* delete_range(char* start_pos, char* end_pos);
char* insert_after_line(char* line, char* data, int nl_flag, uint8_t expand_type);
char* delete_substring(char *substring);
char* append_after_line(char* line, uint8_t nl_flag);
#ifdef DEBUG
void print_characters(void);
void print_buffer_size(void);
void print_number_of_lines(void);
void print_position_at_line(char *line);
#endif

#endif

