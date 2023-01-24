#include "include/utils.h"
#include <ncurses.h>

void exit_screen() {
  endwin(); // end ncurses
}

void init_screen() {
  initscr();            // initialize ncurses
  noecho();             // turn off input echoing
  cbreak();             // disable buffering of typed characters
  keypad(stdscr, TRUE); // enable special keys (e.g. arrow keys)
  nodelay(stdscr, TRUE);
}

WINDOW *create_footer() {
  // Create a new window for the footer
  WINDOW *footer = newwin(1, COLS, LINES - 1, 0);

  // Set the background color to red and the text color to white
  init_pair(1, COLOR_WHITE, COLOR_RED);
  wbkgd(footer, COLOR_PAIR(1));
  wattron(footer, COLOR_PAIR(1));

  // Draw a horizontal line to separate the text from the border
  mvwhline(footer, 0, 0, ACS_HLINE, COLS);
  // // Add text to the footer
  // mvwaddstr(footer, 0, 0, "Press 'q' to quit");

  // Refresh the screen
  wrefresh(footer);
  return footer;
}

void process_input() {
  char *filename = "./test/test.txt";
  char text_buffer[100]; // string to store user input
  int i = 0;             // index for str

  WINDOW *footer = create_footer();
  wrefresh(footer);

  for(;;) {
    // int ch = getch(); // get input character

    // if (ch == KEY_BACKSPACE || ch == 127) { // handle backspace
    //   if (i > 0) {
    //     i--;
    //     mvdelch(0, i); // move cursor back and delete character
    //   }
    // } else if (ch == 27) {   // handle newline
    //   text_buffer[i] = '\0'; // add null terminator
    //   break;                 // exit loop
    // } else {
    //   text_buffer[i++] = ch; // add character to str
    //   update_screen(ch);     // print character
    // }
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
