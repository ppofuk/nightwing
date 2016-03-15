Xephyr +extension RANDR -ac -screen 800x600 -br -noreset :1 &
sleep 1
export DISPLAY=:1.0
xterm &
xsetroot -cursor_name left_ptr &
gdb ./bin/debug/nightwing
