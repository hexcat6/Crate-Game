#include <ncurses.h>
#include <iostream>

int ch;

int main() {
    setlocale(LC_CTYPE, "");
    initscr();
    noecho();
    keypad(stdscr, true);
    nodelay(stdscr, true);
    curs_set(0);
    start_color();
    use_default_colors();

    init_pair(1, COLOR_BLUE, -1);
    clear();
    refresh();
    
    attrset(COLOR_PAIR(1));
    // attron(A_UNDERLINE);
    
    while ((ch = getch()) != 'q' && ch != 'Q') {
        move(0,0); addch(ACS_ULCORNER); addch(ACS_HLINE); addch(ACS_URCORNER);
        move(1,0); addch(ACS_LLCORNER); addch(ACS_HLINE); addch(ACS_LRCORNER);
        mvprintw(2,0,"┌─┐");
        mvprintw(3,0,"└─┘");
    }
    endwin();
    exit(0);
}