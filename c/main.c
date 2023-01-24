#include <ncurses.h>

void init_screen() {
  initscr();            // initialize ncurses
  noecho();             // turn off input echoing
  cbreak();             // disable buffering of typed characters
  keypad(stdscr, TRUE); // enable special keys (e.g. arrow keys)
}

void exit_screen() {
  endwin(); // end ncurses
}

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
  FILE *file = fopen("./kim_logs.txt", "a"); // open file for writing

  if (file == NULL) {
    printf("Error opening file!\n");
    return;
  }

  fprintf(file, "%s", log);   // write contents to file
  fprintf(file, "%s", "\n"); // write contents to file

  fclose(file); // close the file
}

void process_input() {
  char *filename = "test.txt";
  char text_buffer[100]; // string to store user input
  int i = 0;             // index for str

  while (1) {
    int ch = getch(); // get input character

    if (ch == KEY_BACKSPACE || ch == 127) { // handle backspace
      if (i > 0) {
        i--;
        mvdelch(0, i); // move cursor back and delete character
      }
    } else if (ch == 27) {   // handle newline
      text_buffer[i] = '\0'; // add null terminator
      break;                 // exit loop
    } else {
      text_buffer[i++] = ch; // add character to str
      update_screen(ch);     // print character
    }
  }

  write_file(filename, text_buffer);
}

int main() {
  kim_log("starting kim");
  init_screen();

  process_input();

  exit_screen();

  return 0;
}
