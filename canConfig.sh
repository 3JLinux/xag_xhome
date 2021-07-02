#!/bin/sh
ip link set can0 type can bitrate 500000
ifconfig can0 up
