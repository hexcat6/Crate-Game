ifneq ($(OS),Windows_NT)
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        NCURSES += -lncursesw
    endif
    ifeq ($(UNAME_S),Darwin)
        NCURSES += -lncurses
    endif
endif


bin/main: ./bin/main.o 
	g++ -g -o bin/crate bin/main.o -std=c++20 $(NCURSES) # -Wall -pedantic -Werror -fsanitize=undefined,address

run: ./bin/main
	./bin/crate

clean: ./src/main.cpp
	rm -rf ./bin

framework: bin/main
	install_name_tool -change @rpath/libclang_rt.asan_osx_dynamic.dylib @executable_path/../framework/libclang.dylib ./bin/crate
	install_name_tool -change /usr/lib/libc++.1.dylib @executable_path/../framework/libc++.1.dylib ./bin/crate
	install_name_tool -change /usr/lib/libncurses.5.4.dylib @executable_path/../framework/libncurses.5.4.dylib ./bin/crate
	install_name_tool -change /usr/lib/libSystem.B.dylib @executable_path/../framework/libSystem.B.dylib ./bin/crate
#	install_name_tool -change /usr/lib/libc++.1.dylib @executable_path/../framework/libc++.1.tbd ./bin/crate

bin/main.o: ./src/main.cpp
	mkdir -p bin	
	g++ -g -c -o bin/main.o src/main.cpp
