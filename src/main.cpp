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
        void draw() {
            //code
            setcolour(colour);
            mvprintw(19,7*column+5,"1");
        }
};

class player {

    public:
        int column;
        int x;
        int row;
        int button;
        int arm;
        string status; // standby, grabbing, placeing, lowering, raising


        void move() {
            if (button != x) {
                if (arm != 0) {
                    arm--;
                } else if (button < x) {
                    x--;
                } else if (button > x) {
                    x++;
                }
                
            }

            if (button == x) {
                column = button;
                if (arm < 15) {
                    arm++;
                }
                if (status == "standby") {
                    arm--;
                }
            }
        }

        void draw () {
            init_pair(7, COLOR_WHITE, -1);
            attrset(COLOR_PAIR(7));
            mvprintw(0,7*x+5,"┓");
            if (arm) {
                for (int i = 1; i <= arm; i++) {
                    mvprintw(i,7*x+5,"┃");
                }
            }
            mvprintw(arm+1,7*x+3,"┏━┗━┓");
        }

};

class inventory {
    public:
        //7 column, 6 rows 
        class crate coordinates[7][6];
        void spawn() {
            
            int randomcrate = 1+ (rand() % 100);
            int randomcoliumn = 0+ (rand() % 6);

            if (randomcrate == 1) {
                //white crate
                crate newcrate;
                newcrate.colour = 1;
                newcrate.column = randomcoliumn;
                coordinates[randomcoliumn][1] = newcrate;
            }
            if (2 >= randomcrate && randomcrate >= 3) {
                //pink crate
                crate newcrate;
                newcrate.colour = "pink";
            }
            if (4 >= randomcrate && randomcrate >= 8) {
                //red crate
                crate newcrate;
                newcrate.colour = "red";
            }
            if (9 >= randomcrate && randomcrate >= 15) {
                //yellow crate
                crate newcrate;
                newcrate.colour = "yellow";
            }
            if (16 >= randomcrate && randomcrate >= 24) {
                //green crate
                crate newcrate;
                newcrate.colour = "green";
            }
            if (25 >= randomcrate && randomcrate >= 35) {
                //cyan crate
                crate newcrate;
                newcrate.colour = "cyan";
            }
            if (36 >= randomcrate && randomcrate >= 48) {
                //blue crate
                crate newcrate;
                newcrate.colour = "blue";
            }
            if (49 >= randomcrate && randomcrate >= 59) {
                //tnt crate
                crate newcrate;
                newcrate.colour = "tnt";
            }
            
            if (60 >= randomcrate && randomcrate >= 100) {
                //nothing
            }
        }

        void draw () {
            for (int c = 1; c <= 7; c++) {
                for (int r = 1; r <= 6; r++) {
                    coordinates[c][r].draw();
                }
            }
        }
};

int ch;
int currentscore = 0;
bool gameover = false;
//string craterarity[100] = {'white', 'pink', 'pink', 'red', 'red', 'red', 'yellow', 'yellow', 'yellow', 'yellow', 'green', 'green', 'green', 'green', 'green', 'green', 'green', 'green', 'cyan', 'cyan', 'cyan', 'cyan', 'cyan', 'cyan', 'cyan', 'cyan', 'cyan', 'cyan', 'blue', 'blue', 'blue', 'blue', 'blue', 'blue', 'blue', 'blue', 'blue', 'blue', 'blue', 'blue', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'nothing', 'tnt', 'tnt', 'tnt', 'tnt', 'tnt', 'tnt', 'tnt', 'tnt', 'tnt', 'tnt'};

player player;
inventory inventory;

// different colours
//COLOR_YELLOW, COLOR_CYAN, COLOR_BLUE, COLOR_WHITE, COLOR_RED, COLOR_GREEN, COLOR_MAGENTA

//functions

void setcolour (string colour) {
    if (colour == "white") {

    }
    if (colour == "pink") {

    }
    if (colour == "red") {

    }
    if (colour == "yellow") {

    }
    if (colour == "green") {

    }
    if (colour == "cyan") {

    }
    if (colour == "blue") {

    }
    if (colour == "black") {

    }
    if (colour == "tnt") {

    }
}

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

void draw() {
    // print the first of the screen line to cover up shape

    const string screenstring = "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓    ┏━━━━━score━━━━━┓\n┃                                                  ┃    ┃ current: 0    ┃\n┃                                                  ┃    ┃ high: 0       ┃\n┃                                                  ┃    ┗━━━━━━━━━━━━━━━┛\n┃                                                  ┃    ┏━━━━━boost━━━━━┓\n┃                                                  ┃    ┃               ┃\n┃                                                  ┃    ┃     T * |     ┃\n┃                                                  ┃    ┗━━━━━━━━━━━━━━━┛\n┃                                                  ┃\n┃                                                  ┃\n┃                                                  ┃\n┃                                                  ┃\n┃                                                  ┃\n┃                                                  ┃\n┃                                                  ┃\n┃                                                  ┃\n┃                                                  ┃\n┗━━┗━1━┛━━┗━2━┛━━┗━3━┛━━┗━4━┛━━┗━5━┛━━┗━6━┛━━┗━7━┛━┛\n";
    for (int i = 0; i < 18; i++) {
        mvprintw(i,0,readLine(screenstring, i).c_str());
    }
    player.draw();
    inventory.draw();
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


    init_pair(7, COLOR_WHITE, -1);
    attrset(COLOR_PAIR(7));
}

void game() {
	switch(ch) {
        case '1':     
            mvprintw(12,60,"1");
            player.column = 0;
            player.button = 0;
            break;
        case '2': 
            mvprintw(12,60,"2");
            player.column = 1;
            player.button = 1;
            break;
        case '3': 
            mvprintw(12,60,"3"); 
            player.column = 2;
            player.button = 2;
            break;
        case '4':
            mvprintw(12,60,"4");
            player.column = 3;
            player.button = 3;
            break;
        case '5':
            mvprintw(12,60,"5");
            player.column = 4;
            player.button = 4;
            break;
        case '6':
            mvprintw(12,60,"6");
            player.column = 5;
            player.button = 5;
            break;
        case '7':
            mvprintw(12,60,"7");
            player.column = 6;
            player.button = 6;
            break;
        default: 
            break;
    }
    player.move();
    inventory.spawn();
}

//main game loop

int main()
{
    //state the defult settings
    setlocale(LC_CTYPE, "");
    initscr();
    noecho();
    keypad(stdscr, true);
    nodelay(stdscr, true);
    curs_set(0);
	start_color();
    use_default_colors();
    player.status = "standby";
    // player.status = "grabbing";
    
    //clear the screen and refreash it
    clear();
    refresh();
    // the main game stuff starts here

	while ((ch = getch()) != 'q' && ch != 'Q') {
        draw();
        game();
        refresh();
        usleep(100000);
    }
    //and ends here
    refresh();

    // while(1);
    //exit the game
    endwin();
    return 0;
}


