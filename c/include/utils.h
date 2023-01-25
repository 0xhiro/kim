#ifndef UTILS_H
#define UTILS_H

void update_screen(char ch);

void write_file(char *filename, char *contents);

void kim_log(char *log);

int file_has_extension(const char *file_name, const char *extension);

char *read_file_to_string(const char *filename);

void dump_logs();

void clear_logs();
#endif