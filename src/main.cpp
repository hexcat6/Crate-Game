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
#include <time.h> 

using namespace std;

//variables

class crate {
    public:
        int colour;
        int column;
        int row;
        bool freespace;

        void draw() {
            attrset(COLOR_PAIR(colour));
            mvprintw(-2*row+15,7*column+4,"┏━┓");
            mvprintw(-2*row+16,7*column+4,"┗━┛");
        }
};
class crate coordinates[7][6];//7 column, 6 rows 

class player {

    private:
        int x;
        int y;
        int arm;
        int maxarm;
    public:
        int button;
        string status; // standby, grabbing, placeing, lowering, raising
        class crate holding;


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
                for (int y = 0; y <= 6; y++) {
                    if (coordinates[x][y].freespace == true) {
                        maxarm = -2*y+15;
                        y=y;
                        break;
                    }
                    if ((coordinates[x][y].freespace == false) && (y == 6)) {
                        maxarm = 3;
                        break;
                    }
                }
                // maxarm = -2*i+15;
                if (arm < maxarm) {
                    arm++;
                }
                if (status == "standby") {
                    arm--;
                }
                if (arm == maxarm) {
                    holding = coordinates[x][y];
                    coordinates[x][y].colour = 0;
                    
                }
            }
        }

        void draw () {
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


int ch;
int currentscore = 0;
bool gameover = false;


player player;
// different colours
//COLOR_YELLOW, COLOR_CYAN, COLOR_BLUE, COLOR_WHITE, COLOR_RED, COLOR_GREEN, COLOR_MAGENTA

//functions

        void inventoryspawn() {
            // srand (time(0));
            int randomcrate = rand() % 101;
            int randomcoliumn = rand() % 7;
            string craterarity[100] = {"white", "pink", "pink", "red", "red", "red", "yellow", "yellow", "yellow", "yellow", "green", "green", "green", "green", "green", "green", "green", "green", "cyan", "cyan", "cyan", "cyan", "cyan", "cyan", "cyan", "cyan", "cyan", "cyan", "blue", "blue", "blue", "blue", "blue", "blue", "blue", "blue", "blue", "blue", "blue", "blue", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing"};

            if (!(craterarity[randomcrate] == "nothing") && !(player.button == randomcoliumn)) {
                for (int i = 0; i <= 6; i++) {
                    if (coordinates[randomcoliumn][i].freespace == true) {
                        crate newcrate;
                        if (craterarity[randomcrate] == "blue") {
                            newcrate.colour = 1;//blue
                        } else if (craterarity[randomcrate] == "cyan") {
                            newcrate.colour = 2;//cyan
                        } else if (craterarity[randomcrate] == "green") {
                            newcrate.colour = 3;//green
                        } else if (craterarity[randomcrate] == "yellow") {
                            newcrate.colour = 4;//yellow
                        } else if (craterarity[randomcrate] == "red") {
                            newcrate.colour = 5;//red
                        } else if (craterarity[randomcrate] == "pink") {
                            newcrate.colour = 6;//pink
                        } else if (craterarity[randomcrate] == "white") {
                            newcrate.colour = 7;//white
                        }
                        newcrate.column = randomcoliumn;
                        newcrate.row = i;
                        newcrate.freespace = false;
                        coordinates[randomcoliumn][i] = newcrate;
                        break;
                    }
                }
            }
        }

        void inventorydraw () {
            for (int c = 0; c < 7; c++) {
                for (int r = 0; r < 6; r++) {
                    if (coordinates[c][r].colour != 0) {
                        coordinates[c][r].draw();
                    }
                }
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
    inventorydraw();
    player.draw();


    attrset(COLOR_PAIR(1));

    mvprintw(10,55,"┏━┓");
    mvprintw(11,55,"┗━┛");

    attrset(COLOR_PAIR(2));

    mvprintw(10,60,"┏━┓");
    mvprintw(11,60,"┗━┛");

    attrset(COLOR_PAIR(3));

    mvprintw(10,65,"┏━┓");
    mvprintw(11,65,"┗━┛");

    attrset(COLOR_PAIR(4));

    mvprintw(10,70,"┏━┓");
    mvprintw(11,70,"┗━┛");

    attrset(COLOR_PAIR(5));

    mvprintw(10,75,"┏━┓");
    mvprintw(11,75,"┗━┛");

    attrset(COLOR_PAIR(6));

    mvprintw(10,80,"┏━┓");
    mvprintw(11,80,"┗━┛");

    attrset(COLOR_PAIR(7));

    mvprintw(10,85,"┏━┓");
    mvprintw(11,85,"┗━┛");

    attrset(COLOR_PAIR(8));

    mvprintw(10,90,"┏━┓");
    mvprintw(11,90,"┗━┛");

    attrset(COLOR_PAIR(7));
}

void game() {
	switch(ch) {
        case '1':
            player.button = 0;
            break;
        case '2': 
            player.button = 1;
            break;
        case '3': 
            player.button = 2;
            break;
        case '4':
            player.button = 3;
            break;
        case '5':
            player.button = 4;
            break;
        case '6':
            player.button = 5;
            break;
        case '7':
            player.button = 6;
            break;
        default: 
            break;
    }

    player.move();
    inventoryspawn();
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

    init_pair(1, COLOR_BLUE, -1);
    init_pair(2, COLOR_CYAN, -1);
    init_pair(3, COLOR_GREEN, -1);
    init_pair(4, COLOR_YELLOW, -1);
    init_pair(5, COLOR_RED, -1);
    init_pair(6, COLOR_MAGENTA, -1);
    init_pair(7, COLOR_WHITE, -1);
    init_pair(8, COLOR_BLACK, -1);

    player.status = "standby";
    player.status = "grabbing";
    for (int y = 0; y < 7; y++) {
        for (int x = 0; x < 6; x++) {
            coordinates[y][x].freespace = true;
        }
    }


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
    //exit the game
    endwin();
    return 0;
}


