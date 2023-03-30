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
        int column;
        int row;
        bool freespace;

        void draw() {
            attrset(COLOR_PAIR(colour));
            mvprintw(-2*row+15,7*column+4,"┏━┓");
            mvprintw(-2*row+16,7*column+4,"┗━┛");
        }
};

class inventory {
    public:
        int top[7];
        class crate coordinates[7][6];

        bool push(int x, class crate item) {
            if (top[x] > 6) {//full
                return false;
            } else {
                coordinates[x][top[x]++] = item;
                return true;
            }
        };

        class crate pop(int x) {
            class crate item = coordinates[x][top[x]--];
            coordinates[x][top[x]].freespace = true;
            return item;
        };

        bool isEmpty(int x) {
            return (top[x] == 0);
        };

        bool isFull(int x) {
            return (top[x] == 6);
        };

        void draw() {
            for (int x = 0; x < 7; x++) {
                for (int y = 0; y < 6; y++) {
                    if (coordinates[x][y].colour != 0 && coordinates[x][y].freespace == false) {
                        coordinates[x][y].draw();
                    }
                }
            }
        };

        void spawn(int playerx) {
            // srand (time(0));
            int randomcrate = rand() % 100;
            int randomx = rand() % 7;
            string craterarity[100] = {"white", "pink", "pink", "red", "red", "red", "yellow", "yellow", "yellow", "yellow", "green", "green", "green", "green", "green", "green", "green", "green", "cyan", "cyan", "cyan", "cyan", "cyan", "cyan", "cyan", "cyan", "cyan", "cyan", "blue", "blue", "blue", "blue", "blue", "blue", "blue", "blue", "blue", "blue", "blue", "blue", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing"};
            if (!(craterarity[randomcrate] == "nothing") && !(playerx == randomx)) {
                if (!(isFull(randomx))) {
                int y = top[randomx];
                    if (coordinates[randomx][y].freespace == true) {
                        class crate newcrate;
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
                        newcrate.column = randomx;
                        newcrate.row = y;
                        newcrate.freespace = false;
                        push(randomx, newcrate);
                    }
                }
            }
        }
        
};
class inventory inventory;

class player {

    private:
        int arm;
        int maxarm;
        int y;
    public:
        int x;
        int button;
        int onclick;
        // standby, grabbing, placeing, lowering, raising
        enum status {
            standby,
            raising,
            lowering
        };
        enum status status;
        class crate holding;
        bool isHolding;


        void move() {

            if (button != x) {
                if (arm > 0) {
                    arm--;
                } else if (button < x) {
                    x--;
                } else if (button > x) {
                    x++;
                }
                
                if (button == x) {
                    status = lowering;
                } else if (button != x) {
                    status = standby;
                }

            }

            if (onclick == x || button == x) {
                y = inventory.top[x];
                if (isHolding == false) {// work out how low the arm can go when holding nothing
                    if (inventory.isFull(x)) {// column is full and arm is holding nothing
                        maxarm = 3;
                    } else if (inventory.isEmpty(x)) {// column is empty and arm is holding nothing
                        maxarm = 15;
                    } else { // is holding nothing
                        maxarm = -2*y+16;
                    }
                } else if (isHolding == true) {// work out how low the arm can go when holding a crate
                    if (inventory.isFull(x)) {// column is full and arm is holding a crate
                        maxarm = 0;
                    } else if (inventory.isEmpty(x)) {// column is empty and arm is holding a crate
                        maxarm = 13;
                    } else { // is holding a crate
                        maxarm = -2*y+13;
                    }
                }

                if (arm < maxarm) { //lower and raise arm animation
                    status = lowering;
                }
                if (arm < 0) {
                    status = raising;
                }

                if (arm == maxarm) { //pick up or place
                    onclick = -1;
                    if (holding.freespace) { // grabbing
                        if (!(inventory.isEmpty(x))) {
                            holding = inventory.pop(x);
                        }
                    } else if (!holding.freespace) { // placing
                        isHolding = !(inventory.push(x, holding));
                    }
                }
            }
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
            if (holding.freespace && holding.colour != 0) {
                attrset(COLOR_PAIR(holding.colour));
                mvprintw(arm+2,7*x+4,"┏━┓");
                mvprintw(arm+3,7*x+4,"┗━┛");
            }
        }

};


int ch;
int currentscore = 0;
bool gameover = false;

class player player;

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
    mvprintw(12,60,"         ");
    mvprintw(12,60,"%d",inventory.top[player.x]);
    mvprintw(13,60,"         ");
    if (!player.holding.freespace) {
        mvprintw(13,60,"false");
    } else if (player.holding.freespace) {
        mvprintw(13,60,"true");
    }
    attrset(COLOR_PAIR(7)); // delete this line for a cool effect :)
}

void game() {
	switch(ch) {
        case '1':
            player.button = 0;
            player.onclick = 0;
            break;
        case '2': 
            player.button = 1;
            player.onclick = 1;
            break;
        case '3': 
            player.button = 2;
            player.onclick = 2;
            break;
        case '4':
            player.button = 3;
            player.onclick = 3;
            break;
        case '5':
            player.button = 4;
            player.onclick = 4;
            break;
        case '6':
            player.button = 5;
            player.onclick = 5;
            break;
        case '7':
            player.button = 6;
            player.onclick = 6;
            break;
        case KEY_MOUSE: 
            MEVENT event;
            if (getmouse(&event) == OK) {
                if (event.x >= 0 && event.x <= 8) {
                    player.button = 0;
                    player.onclick = 0;
                }
                if (event.x >= 9 && event.x <= 15) {
                    player.button = 1;
                    player.onclick = 1;
                }
                if (event.x >= 16 && event.x <= 22) {
                    player.button = 2;
                    player.onclick = 2;
                }
                if (event.x >= 23 && event.x <= 29) {
                    player.button = 3;
                    player.onclick = 3;
                }
                if (event.x >= 30 && event.x <= 36) {
                    player.button = 4;
                    player.onclick = 4;
                }
                if (event.x >= 37 && event.x <= 43) {
                    player.button = 5;
                    player.onclick = 5;
                }
                if (event.x >= 44) {
                    player.button = 6;
                    player.onclick = 6;
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

    player.status = player::standby;
    player.isHolding = false;
    player.holding.freespace = true;
    for (int y = 0; y < 7; y++) {
        for (int x = 0; x < 6; x++) {
            inventory.coordinates[y][x].freespace = true;
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


