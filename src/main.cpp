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
#include <cmath> // math library

using namespace std;

//variables

int score = 0;
bool gameover = false;

class crate {
    public:
        enum type {
            air,
            basic,
            tnt,
        } type;
        int colour;
        bool infected;
        int explode;

        void draw(int x, int y) {
            if (type == air) return;
            if (type == basic) {
                attrset(COLOR_PAIR(colour));
                mvprintw(-2*y+15,7*x+4,"┏━┓");
                mvprintw(-2*y+16,7*x+4,"┗━┛");
            }
            if (type == tnt) {
                attrset(COLOR_PAIR(colour));
                if (sin(0.03 * pow(explode, 1.6)) < 0) {
                    attrset(COLOR_PAIR(7));
                }
                explode++;
                mvprintw(-2*y+15,7*x+4,"┓┳┏");
                mvprintw(-2*y+16,7*x+4,"┛┻┗");
                if (explode  > 495) {
                    mvprintw(-2*y+15,7*x+4,"@#*");
                    mvprintw(-2*y+16,7*x+4,"&!ඞ");
                }
            }
        }
};

class inventory {
    private:
        int nextspawntime;
    public:
        bool spawned;
        short top[7];
        class crate coordinates[7][8];
        inventory() {
            for (int x = 0; x < 7; x++) {
                top[x] = -1;
                for (int y = 0; y < 8; y++) {
                    coordinates[x][y].type = crate::air;
                    coordinates[x][y].colour = 0;
                    coordinates[x][y].infected = false;
                    coordinates[x][y].explode = 0;
                }
            }
        }

        bool isEmpty(int x) {
            return ((top[x] < 0) && (coordinates[x][0].type == crate::air));
        };

        bool isFull(int x) {
            return ((top[x] > 4) && (coordinates[x][5].type != crate::air));
        };

        class crate pop(int x) {
            if (!isEmpty(x)) {
                class crate item = coordinates[x][top[x]];
                coordinates[x][top[x]--].type = crate::air;
                return item;
            } else {
                class crate item = {};
                item.type = crate::air;
                return item;
            }
        };

        class crate slash(int x, int y) {
            if ((-1 < x) && (x < 7) && (-1 < y) && (y < 6)) {
                if (coordinates[x][y].type != crate::air) {
                    class crate item = coordinates[x][y];
                    coordinates[x][y].type = crate::air;
                    top[x]--;
                    return item;
                }
            }
        }

        bool push(int x, class crate item) {
            if (!isFull(x)) {
                top[x]++;
                coordinates[x][top[x]] = item;
                return true;
            } else {
                return false;
            }
        };

        void draw() {
            for (int x = 0; x < 7; x++) {
                for (int y = 0; y < 8; y++) {
                    coordinates[x][y].draw(x, y);
                }
            }
        };

        void spawn(int playerx) {//wow
            if (time(0) >= nextspawntime) {
                srand(time(0));
                int randomcrate=rand()%48;
                int randomx = rand() % 7;
                const char craterarity[49] = {"ryyyyggggggggccccccccccbbbbbbbbbbbbbbbbbttttttth"};
                if (playerx != randomx) {
                    if (!(isFull(randomx))) {
                        class crate newcrate = {};
                        newcrate.explode = 0;
                        switch (craterarity[randomcrate]) {
                            case 'b'://blue
                                newcrate.colour = 1;
                                newcrate.type = crate::basic;
                                break;
                            case 'c'://cyan
                                newcrate.colour = 2;
                                newcrate.type = crate::basic;
                                break;
                            case 'g'://green
                                newcrate.colour = 3;
                                newcrate.type = crate::basic;
                                break;
                            case 'y'://yellow
                                newcrate.colour = 4;
                                newcrate.type = crate::basic;
                                break;
                            case 'r'://red
                                newcrate.colour = 5;
                                newcrate.type = crate::basic;
                                break;
                            case 't'://tnt
                                newcrate.colour = 5;//red
                                newcrate.type = crate::tnt;
                                break;
                            case 'h'://mega tnt
                                newcrate.colour = 6;//magenta
                                newcrate.type = crate::tnt;
                                break;
                        }
                        top[randomx]++;
                        coordinates[randomx][7] = newcrate;
                    }
                }
                nextspawntime = time(0) + rand() % 3 + 1;
            }
        }

        void gravity() {
            for (int x = 0; x < 7; x++) {
                for (int y = 1; y < 8; y++) {
                    if ((coordinates[x][y].type != crate::air) && (coordinates[x][y-1].type == crate::air)) {
                        coordinates[x][y-1] = coordinates[x][y];
                        coordinates[x][y].type = crate::air;
                    }
                }
            }
        }

        void crunch() {
            for (int x = 0; x < 7; x++) {
                for (int y = 0; y < 4; y++) {
                    if ((coordinates[x][y].type != crate::air) && (coordinates[x][y].type == coordinates[x][y+1].type) && (coordinates[x][y].type == coordinates[x][y+2].type)) {
                        if ((coordinates[x][y].colour == coordinates[x][y+1].colour) && (coordinates[x][y].colour == coordinates[x][y+2].colour)) {
                            if (coordinates[x][y].colour < 8) {
                                coordinates[x][y].colour++;
                                coordinates[x][y].explode = 0;
                                pop(x);pop(x);
                                score += coordinates[x][y].colour;
                            }
                        }
                    }
                }
            }
        }

        void explode() {
            for (int x = 0; x < 7; x++) {
                for (int y = 0; y < 8; y++) {
                    if ((coordinates[x][y].type == crate::tnt) && (coordinates[x][y].explode > 500)) {
                        switch(coordinates[x][y].colour) {
                            case 5://bomb
                                slash(x, y);slash(x, y-1);slash(x, y+1);
                                break;
                            case 6:// mega bomb
                                slash(x, y);slash(x, y-1);slash(x, y+1);slash(x+1, y);slash(x-1, y);
                                break;
                            case 7:// hyper bomb
                                slash(x, y);slash(x, y-1);slash(x, y+1);slash(x+1, y);slash(x-1, y);slash(x+1, y+1);slash(x+1, y-1);slash(x-1, y+1);slash(x-1, y-1);
                                break;
                        }
                    }
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
        int maxarm;
    public:
        int x;
        int button;
        class crate holding;
        bool down;

        player() {
            holding.type = crate::air;
            holding.colour = 0;
            holding.infected = false;
            holding.explode = 0;
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
                    if (holding.type != crate::air) {// work out how low the arm can go when holding nothing
                        if (inventory.isFull(x)) {// column is full and arm is holding nothing
                            maxarm = 0;
                        } else if (inventory.isEmpty(x)) {// column is empty and arm is holding nothing
                            maxarm = 13;
                        } else { // is holding nothing
                            maxarm = -2*y+11;
                        }
                    } else if (holding.type == crate::air) {// work out how low the arm can go when holding a crate
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
                    if (holding.type == crate::air) { // grabbing
                        holding = inventory.pop(x);
                    } else if (holding.type != crate::air) { // placing
                        bool pushed = inventory.push(x, holding);
                        if (pushed) {
                            holding.type = crate::air;
                        }
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
            attrset(COLOR_PAIR(holding.colour));
            if (holding.type == crate::air) return;
            if (holding.type == crate::basic) {
                mvprintw(arm+2,7*x+4,"┏━┓");
                mvprintw(arm+3,7*x+4,"┗━┛");
            }
            if (holding.type == crate::tnt) {
                attrset(COLOR_PAIR(holding.colour));
                if (sin(0.03 * pow(holding.explode, 1.6)) < 0) {
                    attrset(COLOR_PAIR(7));
                }
                holding.explode++;

                mvprintw(arm+2,7*x+4,"┓┳┏");
                mvprintw(arm+3,7*x+4,"┛┻┗");
                if (holding.explode  > 495) {
                    mvprintw(arm+2,7*x+4,"@#*");
                    mvprintw(arm+3,7*x+4,"&!ඞ");
                }
            }
        }

};

class player player;

int ch;

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

void endgame() {
    if ((player.holding.explode > 500) || (player.holding.type != crate::air) && (inventory.isFull(0)) && (inventory.isFull(1)) && (inventory.isFull(2)) && (inventory.isFull(3)) && (inventory.isFull(4)) && (inventory.isFull(5)) && (inventory.isFull(6))) {
        gameover = true;
    }
}

void draw() {
    // print the first of the screen line to cover up shape
    attrset(COLOR_PAIR(7)); // delete this line for a cool effect :)

    const string screenstring = "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓    ┏━━━━━score━━━━━┓\n┃                                                  ┃    ┃ current: 0    ┃\n┃                                                  ┃    ┃ high: 0       ┃\n┃                                                  ┃    ┗━━━━━━━━━━━━━━━┛\n┃                                                  ┃    ┏━━━━━items━━━━━┓\n┃                                                  ┃    ┃               ┃\n┃                                                  ┃    ┃     T * |     ┃\n┃                                                  ┃    ┗━━━━━━━━━━━━━━━┛\n┃                                                  ┃\n┃                                                  ┃\n┃                                                  ┃\n┃                                                  ┃\n┃                                                  ┃\n┃                                                  ┃\n┃                                                  ┃\n┃                                                  ┃\n┃                                                  ┃\n┗━━┗━1━┛━━┗━2━┛━━┗━3━┛━━┗━4━┛━━┗━5━┛━━┗━6━┛━━┗━7━┛━┛\n";
    for (int i = 0; i < 18; i++) {
        mvprintw(i,0,readLine(screenstring, i).c_str());
    }

    inventory.draw();
    player.draw();
    // mvprintw(12,60,"          ");
    // mvprintw(12,60,"top: %d", inventory.top[player.x]);
    // mvprintw(13,60,"          ");
    // mvprintw(13,60,"type: %d", player.holding.type);
    attrset(COLOR_PAIR(7));
    mvprintw(1,67,"%d", score);// draws the score to the screen
    if (gameover) {
        const string gameoverstring = "┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n┃                    Game Over!                    ┃\n┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫";
        for (int i = 0; i < 3; i++) {
            mvprintw(9+i,0,readLine(gameoverstring, i).c_str());
        }
    }
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
        case KEY_RIGHT:
            if (player.button < 6) {
                player.button++;
            }
            break;
        case KEY_LEFT:
            if (player.button > 0) {
                player.button--;
            }
            break;
        case KEY_DOWN:
            player.down = true;
            break;
        default: 
            break;
    }

    player.move();
    inventory.gravity();
    inventory.crunch();
    inventory.explode();
    inventory.spawn(player.x);
    endgame();
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
        if (gameover == false) {
            game();
            draw();
            refresh();
            usleep(50000);
        }
    }
    //and ends here
    refresh();
    //exit the game
    endwin();
    return 0;
}