#ifndef IO_H
#define IO_H

#define DONT_SKIP_NEW_LINE 1
#define SKIP_NEW_LINE 0
#define IBUF_SIZE 128

char* read_from_stream(FILE* stream, uint8_t new_line_flag);

#endif
