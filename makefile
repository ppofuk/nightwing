.DEFAULT_GOAL := bin/debug/nightwing

bin/nightwing: build/release/__src_test.o build/release/__src_window.o build/release/__src_main.o build/release/__src_session.o build/release/__src_point.o build/release/__src_event.o build/release/__src_rect.o 
	c++  -o $@ $^ `pkg-config --libs --cflags xcb` 

build/release/__src_test.o: src/test.cc src/test.h src/build.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb` -c -o build/release/__src_test.o ./src/test.cc

build/release/__src_window.o: src/window.cc src/window.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb` -c -o build/release/__src_window.o ./src/window.cc

build/release/__src_main.o: src/main.cc src/session.h src/build.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb` -c -o build/release/__src_main.o ./src/main.cc

build/release/__src_session.o: src/session.cc src/session.h src/build.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb` -c -o build/release/__src_session.o ./src/session.cc

build/release/__src_point.o: src/point.cc src/point.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb` -c -o build/release/__src_point.o ./src/point.cc

build/release/__src_event.o: src/event.cc
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb` -c -o build/release/__src_event.o ./src/event.cc

build/release/__src_rect.o: src/rect.cc src/rect.h src/point.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb` -c -o build/release/__src_rect.o ./src/rect.cc

make: 
	lbtcli -m .lbt
	
clean: 
	rm -f build/*.o build/release/*.o build/debug/*.o bin/debug/nightwing bin/nightwing
	
release: bin/nightwing
	
	
bin/debug/nightwing: build/debug/__src_test.o build/debug/__src_window.o build/debug/__src_main.o build/debug/__src_session.o build/debug/__src_point_test.o build/debug/__src_point.o build/debug/__src_event.o build/debug/__src_rect.o 
	c++  -o $@ $^ `pkg-config --libs --cflags xcb` -ggdb 

build/debug/__src_test.o: src/test.cc src/test.h src/build.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb` -ggdb -c -o build/debug/__src_test.o ./src/test.cc

build/debug/__src_window.o: src/window.cc src/window.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb` -ggdb -c -o build/debug/__src_window.o ./src/window.cc

build/debug/__src_main.o: src/main.cc src/session.h src/build.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb` -ggdb -c -o build/debug/__src_main.o ./src/main.cc

build/debug/__src_session.o: src/session.cc src/session.h src/build.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb` -ggdb -c -o build/debug/__src_session.o ./src/session.cc

build/debug/__src_point_test.o: src/point_test.cc src/point.h src/test.h src/build.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb` -ggdb -c -o build/debug/__src_point_test.o ./src/point_test.cc

build/debug/__src_point.o: src/point.cc src/point.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb` -ggdb -c -o build/debug/__src_point.o ./src/point.cc

build/debug/__src_event.o: src/event.cc
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb` -ggdb -c -o build/debug/__src_event.o ./src/event.cc

build/debug/__src_rect.o: src/rect.cc src/rect.h src/point.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb` -ggdb -c -o build/debug/__src_rect.o ./src/rect.cc

