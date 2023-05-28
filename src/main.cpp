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

//variabless
class crate {
    public:
        int colour;
        bool air;

        void draw(int x, int y) {
            if (!air) {
                attrset(COLOR_PAIR(colour));
                mvprintw(-2*y+15,7*x+4,"┏━┓");
                mvprintw(-2*y+16,7*x+4,"┗━┛");
            }
        }
};

class inventory {
    public:
        bool spawned;
        int top[7];
        class crate coordinates[7][6];
        inventory() {
            for (int i = 0; i < 7; i++) {
                top[i] = -1;
            }
            for (int y = 0; y < 7; y++) {
                for (int x = 0; x < 6; x++) {
                    coordinates[y][x].air = true;
                }
            }
        }

        bool isEmpty(int x) {
            return ((top[x] <= 0) && (coordinates[x][top[x]].air));
        };

        bool isFull(int x) {
            return (top[x] == 5);
        };

        class crate pop(int x) {
            if ((!isEmpty(x)) && (coordinates[x][top[x]].air == false)) {
                class crate item = coordinates[x][top[x]];
                coordinates[x][top[x]--].air = true;
                return item;
            }
        };

        bool push(int x, class crate item) {
            if (isFull(x)) {//full
                return false;
            } else {
                top[x]++;
                coordinates[x][top[x]] = item;
                return true;
            }
        };

        void draw() {
            for (int x = 0; x < 7; x++) {
                for (int y = 0; y < 6; y++) {
                    coordinates[x][y].draw(x,y);
                }
            }
        };

        void spawn(int playerx) {//wow
            // srand (time(0));
            int randomcrate = rand() % 100;
            int randomx = rand() % 7;
            string craterarity[100] = {"white", "pink", "pink", "red", "red", "red", "yellow", "yellow", "yellow", "yellow", "green", "green", "green", "green", "green", "green", "green", "green", "cyan", "cyan", "cyan", "cyan", "cyan", "cyan", "cyan", "cyan", "cyan", "cyan", "blue", "blue", "blue", "blue", "blue", "blue", "blue", "blue", "blue", "blue", "blue", "blue", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing"};
            if (!(craterarity[randomcrate] == "nothing") && !(playerx == randomx)) {
                if (!(isFull(randomx))) {
                    class crate newcrate = {};
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
                    } else {
                        newcrate.colour = 8;//black
                    }
                    newcrate.air = false;
                    spawned = push(randomx, newcrate);
                }
            }
        }
};
class inventory inventory;

// player code need fixing work on it stupid dumass
class player {

    private:
        int arm;
        int y;
    public:
        int maxarm;
        int x;
        int button;
        class crate holding;
        bool down;

        player() {
            holding.air = true;
            down = false;
        }

        void move() {

            if (button != x) {
                if (arm > 0) {
                    arm--;
                } else if (button < x) {
                    x--;
                } else if (button > x) {
                    x++;
                }
            }

            if (button == x) {
                y = inventory.top[x];
                if (down == true) {
                    if (holding.air == false) {// work out how low the arm can go when holding nothing
                        if (inventory.isFull(x)) {// column is full and arm is holding nothing
                            maxarm = 0;
                        } else if (inventory.isEmpty(x)) {// column is empty and arm is holding nothing
                            maxarm = 13;
                        } else { // is holding nothing
                            maxarm = -2*y+11;
                        }
                    } else if (holding.air == true) {// work out how low the arm can go when holding a crate
                        if (inventory.isFull(x)) {// column is full and arm is holding a crate
                            maxarm = 3;
                        } else if (inventory.isEmpty(x)) {// column is empty and arm is holding a crate
                            maxarm = 15;
                        } else { // is holding a crate
                            maxarm = -2*y+13;
                        }
                    }
                }

                if ((arm == maxarm) && (maxarm != 0)) { //pick up or place
                    if (holding.air == true) { // grabbing
                        holding = inventory.pop(x);
                    } else if (holding.air == false) { // placing
                        holding.air = !(inventory.push(x, holding));
                        holding.air = true;
                    }
                    down = false;
                    maxarm = 0;
                }

                if ((maxarm < arm) && (!down)) {// raise and lower arm animation
                    arm--;
                } else if ((maxarm > arm) && (down)) {
                    arm++;
                }

            }//yo if you are reading this then you are cool :)

        }

        void draw () {
            attrset(COLOR_PAIR(7));
            mvprintw(0,7*x+5,"┳");
            if (arm) {
                for (int i = 1; i <= arm; i++) {
                    mvprintw(i,7*x+5,"┃");
                }
            }
            mvprintw(arm+1,7*x+3,"┏━┻━┓");
            if (!holding.air && holding.colour != 0) {
                attrset(COLOR_PAIR(holding.colour));
                mvprintw(arm+2,7*x+4,"┏━┓");
                mvprintw(arm+3,7*x+4,"┗━┛");
            }
        }

};

class player player;

int ch;
int currentscore = 0;
bool gameover = false;

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

    const string screenstring = "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓    ┏━━━━━score━━━━━┓\n┃                                                  ┃    ┃ current: 0    ┃\n┃                                                  ┃    ┃ high: 0       ┃\n┃                                                  ┃    ┗━━━━━━━━━━━━━━━┛\n┃                                                  ┃    ┏━━━━━items━━━━━┓\n┃                                                  ┃    ┃               ┃\n┃                                                  ┃    ┃     T * |     ┃\n┃                                                  ┃    ┗━━━━━━━━━━━━━━━┛\n┃                                                  ┃\n┃                                                  ┃\n┃                                                  ┃\n┃                                                  ┃\n┃                                                  ┃\n┃                                                  ┃\n┃                                                  ┃\n┃                                                  ┃\n┃                                                  ┃\n┗━━┗━1━┛━━┗━2━┛━━┗━3━┛━━┗━4━┛━━┗━5━┛━━┗━6━┛━━┗━7━┛━┛\n";
    for (int i = 0; i < 18; i++) {
        mvprintw(i,0,readLine(screenstring, i).c_str());
    }

    inventory.draw();
    player.draw();
    mvprintw(12,60,"             ");
    mvprintw(12,60,"top: %d",inventory.top[player.x]);
    mvprintw(11,60,"             ");
    mvprintw(11,60,"max arm: %d",player.maxarm);
    mvprintw(13,60,"         ");
    if (!player.holding.air) {
        mvprintw(13,60,"air: false ");
    } else if (player.holding.air) {
        mvprintw(13,60,"air: true ");
    }
    mvprintw(41,60,"             ");
    if (inventory.spawned == false) {
        mvprintw(14,60,"spawned: false ");
    } else if (inventory.spawned == true) {
        mvprintw(14,60,"spawned: true ");
    }
    attrset(COLOR_PAIR(7)); // delete this line for a cool effect :)
}

void game() {
	switch(ch) {
        case '1':
            player.button = 0;
            player.down = true;
            break;
        case '2': 
            player.button = 1;
            player.down = true;
            break;
        case '3': 
            player.button = 2;
            player.down = true;
            break;
        case '4':
            player.button = 3;
            player.down = true;
            break;
        case '5':
            player.button = 4;
            player.down = true;
            break;
        case '6':
            player.button = 5;
            player.down = true;
            break;
        case '7':
            player.button = 6;
            player.down = true;
            break;
        case KEY_MOUSE: 
            MEVENT event;
            player.down = true;
            if (getmouse(&event) == OK) {
                if (event.x >= 0 && event.x <= 8) {
                    player.button = 0;
                }
                if (event.x >= 9 && event.x <= 15) {
                    player.button = 1;
                }
                if (event.x >= 16 && event.x <= 22) {
                    player.button = 2;
                }
                if (event.x >= 23 && event.x <= 29) {
                    player.button = 3;
                }
                if (event.x >= 30 && event.x <= 36) {
                    player.button = 4;
                }
                if (event.x >= 37 && event.x <= 43) {
                    player.button = 5;
                }
                if (event.x >= 44) {
                    player.button = 6;
                }
            }

            break; 
        default: 
            break;
    }

    player.move();
    inventory.spawn(player.x);
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
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
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