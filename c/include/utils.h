#ifndef UTILS_H
#define UTILS_H

#define ESCAPE_CHAR '\033'

void update_screen(char ch);

void write_file(char *filename, char *contents);

void kim_log(const char *format, ...);

int file_has_extension(const char *file_name, const char *extension);

char *read_file_to_string(const char *filename);

void dump_logs();

void dump_file(char* filename);

void clear_logs();
#endif