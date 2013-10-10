

bin/main: build/__src_main.o build/__src_app.o 
	c++  -o $@ $^ `pkg-config --libs --cflags xcb` -ggdb 

build/__src_main.o: src/main.cc
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb` -ggdb -c -o build/__src_main.o ./src/main.cc

build/__src_app.o: src/app.cc src/app.h src/config.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb` -ggdb -c -o build/__src_app.o ./src/app.cc

