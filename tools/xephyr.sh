#!/bin/bash
# Use this script with nightwing --exec option to run nightwing
# in Xephyr.
# This allows us to run nightwing in gdb from editor with all
# the fency editor functions (breakpoint here we come!)
# TODO: a smarter script
Xephyr +extension RANDR -ac -screen 800x600 -br -reset 2> /dev/null :1 &
export DISPLAY=:1
sleep 1
xterm &
xsetroot -cursor_name left_ptr
