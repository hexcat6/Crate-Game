#include <cstdlib>
#include <ncurses.h>

using namespace std;

class box {
    public:
        int x;
        int y;
        int number;
        void draw(){
            mvprintw(x,y,"2");
        }

};

class grid {
    public:
        class box coordinates[3][3];
        void spawn() {      
            //randomly spawning code
                int x = 0+ (rand() % 3);
                int y = 0+ (rand() % 3);
                class box newbox;
                newbox.x = x;
                newbox.y = y;
                newbox.number = 2;
                coordinates[x][y] = newbox;
        }
        void draw () {
            for (int x = 1; x <= 7; x++) {
                for (int y = 1; y <= 6; y++) {
                        coordinates[x][y].draw();
                }
            }
        }
};

grid grid;

int main() {
    initscr();
    noecho();
    keypad(stdscr, true);
    nodelay(stdscr, true);
    curs_set(0);

    clear();
    refresh();
    while(1){
        grid.draw();
        refresh();
    }
    endwin();
    return 0;
}
