@echo off
adb push a.out /data/local/tmp
adb shell chmod 775 /data/local/tmp/a.out
adb shell /data/local/tmp/a.out