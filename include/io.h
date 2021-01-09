#ifndef INPUT_H
#define INPUT_H

#define IBUF_SIZE 128

#define SET_NEW_LINE_FLAG 1
#define UNSET_NEW_LINE_FLAG 0

char* read_from_stream(FILE* stream, int with_new_line_flag);
int write_to_file(char *buffer, size_t size, FILE* fp);
int read_from_file(char *buffer, size_t size, FILE* fp);
FILE* open_file(char *filename, char *mode);

#endif