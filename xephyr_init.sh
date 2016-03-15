Xephyr +extension RANDR -ac -screen 800x600 -br -noreset :1 &
sleep 1
export DISPLAY=:1.0
# The following is in ~/autostart.sh
# xterm &
# xsetroot -cursor_name left_ptr &
./bin/debug/nightwing -e ~/autostart.sh
