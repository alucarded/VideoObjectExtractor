#!/bin/sh
echo "Turning off automatic white balance temperature..."
v4l2-ctl -d /dev/video1 --set-ctrl white_balance_temperature_auto=0
echo "Turning off backlight compenastion..."
v4l2-ctl -d /dev/video1 --set-ctrl backlight_compensation=0
echo "Turning off automatic exposure..."
v4l2-ctl -d /dev/video1 --set-ctrl exposure_auto=1

