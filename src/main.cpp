//library
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <ncurses.h>// colour library
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <random>
#include <cstring>
#include <algorithm>
#include <unistd.h>
#include <errno.h>
#include <cstring>

using namespace std;

//variables

class crate {
    public:
        string colour;
        int column;
        int row;
        bool infectived;
};

//7 column, 6 rows 
crate coordinates[7][6];

int currentscore = 0;
bool gameover = false;
// different colours
//COLOR_YELLOW, COLOR_CYAN, COLOR_BLUE, COLOR_WHITE, COLOR_RED, COLOR_GREEN, COLOR_MAGENTA

//functions

string readLine(string str, int n) {
    // returns the nth line of a string
    stringstream f(str);
    string s;
    
    for (int i = 0; i < n; i++) {
        getline(f, s);
    }

    getline(f,s);
    return s; 
}

void screen() {
    // print the first of the screen line to cover up shape

    const string screenstring = "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓    ┏━━━━━score━━━━━┓\n┃                                                  ┃    ┃ current: 0    ┃\n┃                                                  ┃    ┃ high: 0       ┃\n┃                                                  ┃    ┗━━━━━━━━━━━━━━━┛\n┃                                                  ┃    ┏━━━━━boost━━━━━┓\n┃                                                  ┃    ┃               ┃\n┃                                                  ┃    ┃               ┃\n┃                                                  ┃    ┗━━━━━━━━━━━━━━━┛\n┃                                                  ┃\n┃                                                  ┃\n┃                                                  ┃\n┃                                                  ┃\n┃                                                  ┃\n┃                                                  ┃\n┗━━┗━1━┛━━┗━2━┛━━┗━3━┛━━┗━4━┛━━┗━5━┛━━┗━6━┛━━┗━7━┛━┛\n";

    for (int i = 0; i < 15; i++) {
        mvprintw(i,0,readLine(screenstring, i).c_str());
    }
}


//main game loop

int main()
{
    //state the defult settings
    setlocale(LC_CTYPE, "");
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
	start_color();
    use_default_colors();
    //clear the screen and refreash it
    clear();
    refresh();
    // the main game stuff starts here

    screen();

    init_pair(1, COLOR_BLUE, -1);
    attrset(COLOR_PAIR(1));

    mvprintw(10,55,"┏━┓");
    mvprintw(11,55,"┗━┛");

    init_pair(2, COLOR_CYAN, -1);
    attrset(COLOR_PAIR(2));

    mvprintw(10,60,"┏━┓");
    mvprintw(11,60,"┗━┛");

    init_pair(3, COLOR_GREEN, -1);
    attrset(COLOR_PAIR(3));

    mvprintw(10,65,"┏━┓");
    mvprintw(11,65,"┗━┛");

    init_pair(4, COLOR_YELLOW, -1);
    attrset(COLOR_PAIR(4));

    mvprintw(10,70,"┏━┓");
    mvprintw(11,70,"┗━┛");

    init_pair(5, COLOR_RED, -1);
    attrset(COLOR_PAIR(5));

    mvprintw(10,75,"┏━┓");
    mvprintw(11,75,"┗━┛");

    init_pair(6, COLOR_MAGENTA, -1);
    attrset(COLOR_PAIR(6));

    mvprintw(10,80,"┏━┓");
    mvprintw(11,80,"┗━┛");

    init_pair(7, COLOR_WHITE, -1);
    attrset(COLOR_PAIR(7));

    mvprintw(10,85,"┏━┓");
    mvprintw(11,85,"┗━┛");

    init_pair(8, COLOR_BLACK, -1);
    attrset(COLOR_PAIR(8));

    mvprintw(10,90,"┏━┓");
    mvprintw(11,90,"┗━┛");

    // init_pair(1, COLOR_WHITE, -1);
    // attrset(COLOR_PAIR(1));

    //and ends here
    refresh();

    while(1);

    //exit the game
    endwin();
    return 0;
}


