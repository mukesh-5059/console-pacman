#include <ncurses.h>

int main() {
    int ch;

    initscr();           // Start curses mode
    cbreak();            // Line buffering disabled
    noecho();            // Don't echo() while we do getch
    nodelay(stdscr, TRUE); // THE MAGIC LINE: Make getch non-blocking
    keypad(stdscr, TRUE); // Enable arrow keys

    while (1) {
        mvprintw(0, 0, "Press 'q' to quit. I am running constantly!");
        
        // Try to get input
        ch = getch();

        if (ch == 'q') {
            break; 
        } else if (ch == ERR) {
            // No input was received this cycle
            mvprintw(1, 0, "Waiting for input...      ");
        } else {
            // User pressed something!
            mvprintw(1, 0, "You pressed: %c (code: %d)", ch, ch);
        }

        refresh();
        napms(100); // Sleep for 100ms to keep CPU usage sane
    }

    endwin(); // End curses mode
    return 0;
}