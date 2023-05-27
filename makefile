ifneq ($(OS),Windows_NT)
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        NCURSES += -lncursesw
    endif
    ifeq ($(UNAME_S),Darwin)
        NCURSES += -lncurses
    endif
endif

bin/main: bin/main.o 
	g++ -g -o bin/crate bin/main.o -std=c++20 -Wall -pedantic -Werror $(NCURSES) -fsanitize=undefined,address


bin/main.o: src/main.cpp
	mkdir -p bin	
	g++ -g -c -o bin/main.o src/main.cpp