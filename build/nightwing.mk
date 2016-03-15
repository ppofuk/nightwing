bin/nightwing: build/release/__src_test.o build/release/__src_point.o build/release/__src_rect.o build/release/__src_window.o build/release/__src_window-handler.o build/release/__src_main.o build/release/__src_drawing-handler.o build/release/__src_decorator.o build/release/__src_drawing-surface.o build/release/__src_session.o build/release/__src_event-handler.o 
	c++  -o $@ $^ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` 

build/release/__src_test.o: src/test.cc src/test.h src/build.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` -c -o build/release/__src_test.o ./src/test.cc

build/release/__src_point.o: src/point.cc src/point.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` -c -o build/release/__src_point.o ./src/point.cc

build/release/__src_rect.o: src/rect.cc src/rect.h src/point.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` -c -o build/release/__src_rect.o ./src/rect.cc

build/release/__src_window.o: src/window.cc src/window.h src/rect.h src/point.h src/build.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` -c -o build/release/__src_window.o ./src/window.cc

build/release/__src_window-handler.o: src/window-handler.cc src/window-handler.h src/build.h src/type-helpers.h src/window.h src/rect.h src/point.h src/decorator.h src/drawing-surface.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` -c -o build/release/__src_window-handler.o ./src/window-handler.cc

build/release/__src_main.o: src/main.cc src/session.h src/build.h src/type-helpers.h src/window-handler.h src/window.h src/rect.h src/point.h src/decorator.h src/drawing-surface.h src/drawing-handler.h src/event-handler.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` -c -o build/release/__src_main.o ./src/main.cc

build/release/__src_drawing-handler.o: src/drawing-handler.cc src/drawing-handler.h src/build.h src/decorator.h src/window.h src/rect.h src/point.h src/drawing-surface.h src/type-helpers.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` -c -o build/release/__src_drawing-handler.o ./src/drawing-handler.cc

build/release/__src_decorator.o: src/decorator.cc src/decorator.h src/build.h src/window.h src/rect.h src/point.h src/drawing-surface.h src/type-helpers.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` -c -o build/release/__src_decorator.o ./src/decorator.cc

build/release/__src_drawing-surface.o: src/drawing-surface.cc src/drawing-surface.h src/rect.h src/point.h src/build.h src/type-helpers.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` -c -o build/release/__src_drawing-surface.o ./src/drawing-surface.cc

build/release/__src_session.o: src/session.cc src/session.h src/build.h src/type-helpers.h src/window-handler.h src/window.h src/rect.h src/point.h src/decorator.h src/drawing-surface.h src/drawing-handler.h src/event-handler.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` -c -o build/release/__src_session.o ./src/session.cc

build/release/__src_event-handler.o: src/event-handler.cc src/event-handler.h src/build.h src/type-helpers.h src/window-handler.h src/window.h src/rect.h src/point.h src/decorator.h src/drawing-surface.h src/drawing-handler.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` -c -o build/release/__src_event-handler.o ./src/event-handler.cc


bin/debug/nightwing: build/debug/__src_test.o build/debug/__src_point.o build/debug/__src_rect.o build/debug/__src_window.o build/debug/__src_window-handler.o build/debug/__src_main.o build/debug/__src_point-test.o build/debug/__src_rect-test.o build/debug/__src_drawing-handler.o build/debug/__src_decorator.o build/debug/__src_drawing-surface.o build/debug/__src_session.o build/debug/__src_event-handler.o 
	c++  -o $@ $^ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` -ggdb -DNIGHTWING_DEBUG -Wall 

build/debug/__src_test.o: src/test.cc src/test.h src/build.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` -ggdb -DNIGHTWING_DEBUG -Wall -c -o build/debug/__src_test.o ./src/test.cc

build/debug/__src_point.o: src/point.cc src/point.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` -ggdb -DNIGHTWING_DEBUG -Wall -c -o build/debug/__src_point.o ./src/point.cc

build/debug/__src_rect.o: src/rect.cc src/rect.h src/point.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` -ggdb -DNIGHTWING_DEBUG -Wall -c -o build/debug/__src_rect.o ./src/rect.cc

build/debug/__src_window.o: src/window.cc src/window.h src/rect.h src/point.h src/build.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` -ggdb -DNIGHTWING_DEBUG -Wall -c -o build/debug/__src_window.o ./src/window.cc

build/debug/__src_window-handler.o: src/window-handler.cc src/window-handler.h src/build.h src/type-helpers.h src/window.h src/rect.h src/point.h src/decorator.h src/drawing-surface.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` -ggdb -DNIGHTWING_DEBUG -Wall -c -o build/debug/__src_window-handler.o ./src/window-handler.cc

build/debug/__src_main.o: src/main.cc src/session.h src/build.h src/type-helpers.h src/window-handler.h src/window.h src/rect.h src/point.h src/decorator.h src/drawing-surface.h src/drawing-handler.h src/event-handler.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` -ggdb -DNIGHTWING_DEBUG -Wall -c -o build/debug/__src_main.o ./src/main.cc

build/debug/__src_point-test.o: src/point-test.cc src/point.h src/test.h src/build.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` -ggdb -DNIGHTWING_DEBUG -Wall -c -o build/debug/__src_point-test.o ./src/point-test.cc

build/debug/__src_rect-test.o: src/rect-test.cc src/rect.h src/point.h src/test.h src/build.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` -ggdb -DNIGHTWING_DEBUG -Wall -c -o build/debug/__src_rect-test.o ./src/rect-test.cc

build/debug/__src_drawing-handler.o: src/drawing-handler.cc src/drawing-handler.h src/build.h src/decorator.h src/window.h src/rect.h src/point.h src/drawing-surface.h src/type-helpers.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` -ggdb -DNIGHTWING_DEBUG -Wall -c -o build/debug/__src_drawing-handler.o ./src/drawing-handler.cc

build/debug/__src_decorator.o: src/decorator.cc src/decorator.h src/build.h src/window.h src/rect.h src/point.h src/drawing-surface.h src/type-helpers.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` -ggdb -DNIGHTWING_DEBUG -Wall -c -o build/debug/__src_decorator.o ./src/decorator.cc

build/debug/__src_drawing-surface.o: src/drawing-surface.cc src/drawing-surface.h src/rect.h src/point.h src/build.h src/type-helpers.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` -ggdb -DNIGHTWING_DEBUG -Wall -c -o build/debug/__src_drawing-surface.o ./src/drawing-surface.cc

build/debug/__src_session.o: src/session.cc src/session.h src/build.h src/type-helpers.h src/window-handler.h src/window.h src/rect.h src/point.h src/decorator.h src/drawing-surface.h src/drawing-handler.h src/event-handler.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` -ggdb -DNIGHTWING_DEBUG -Wall -c -o build/debug/__src_session.o ./src/session.cc

build/debug/__src_event-handler.o: src/event-handler.cc src/event-handler.h src/build.h src/type-helpers.h src/window-handler.h src/window.h src/rect.h src/point.h src/decorator.h src/drawing-surface.h src/drawing-handler.h
	c++ -Ithirdparty/ `pkg-config --libs --cflags xcb-ewmh` `pkg-config --libs --cflags xcb-icccm` `pkg-config --libs --cflags cairo` -ggdb -DNIGHTWING_DEBUG -Wall -c -o build/debug/__src_event-handler.o ./src/event-handler.cc


