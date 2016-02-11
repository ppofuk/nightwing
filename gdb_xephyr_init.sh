Xephyr +extension RANDR -ac -screen 800x600 -br -reset -terminate 2> /dev/null :1 & (sleep 1; export DISPLAY=:1; xterm & xsetroot -cursor_name left_ptr & gdb ./bin/debug/nightwing)
