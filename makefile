.DEFAULT_GOAL := bin/nightwing

make: 
	lbtcli -m .lbt
	
clean: 
	rm -f obj/*.o obj/lib/*.o bin/nightwing
	
bin/nightwing: build/__src_main.o build/__src_app.o build/__src_observable.o build/__src_event.o build/__src_screen.o 
	c++  -o $@ $^ `pkg-config --libs --cflags xcb` -ggdb 

build/__src_main.o: src/main.cc src/screen.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb` -ggdb -c -o build/__src_main.o ./src/main.cc

build/__src_app.o: src/app.cc src/app.h src/config.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb` -ggdb -c -o build/__src_app.o ./src/app.cc

build/__src_observable.o: src/observable.cc src/observable.h src/build.h src/observer.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb` -ggdb -c -o build/__src_observable.o ./src/observable.cc

build/__src_event.o: src/event.cc
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb` -ggdb -c -o build/__src_event.o ./src/event.cc

build/__src_screen.o: src/screen.cc src/screen.h src/observable.h src/build.h src/observer.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb` -ggdb -c -o build/__src_screen.o ./src/screen.cc

