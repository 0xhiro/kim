#include <curses.h>
#include <stdio.h>

void update_screen(char ch) { addch(ch); }

void write_file(char *filename, char *contents) {
  FILE *file = fopen(filename, "w"); // open file for writing

  if (file == NULL) {
    printf("Error opening file!\n");
    return;
  }

  fprintf(file, "%s", contents); // write contents to file
  fclose(file);                  // close the file
}

void kim_log(char *log) {
  FILE *file = fopen("./target/kim_logs.txt", "a"); // open file for writing

  if (file == NULL) {
    printf("Error opening file!\n");
    return;
  }

  fprintf(file, "%s", log);  // write contents to file
  fprintf(file, "%s", "\n"); // write contents to file

  fclose(file); // close the file
}