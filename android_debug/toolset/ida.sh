#@echo on
adb forward tcp:23946 tcp:23946
gdbpid=`adb shell ps | grep android_server |awk 'NR=="1"{print $2}'`
if [ ! -z "$gdbpid" ]; then
    echo "idaserver is running:$gdbpid"
    echo "gdbpid :"$gdbpid ."kill it!"
    # adb shell su -c "kill -9 $gdbpid" 普通手机
	adb shell su 0 kill -9 $gdbpid
else
    echo "gdbserver first running! :=)"
fi
adb shell am start -D -n com.netease.mhxyhtb/com.netease.mhxyhtb.XyqMobile
# adb push ./android_server /sdcard/
# adb shell cp /sdcard/android_server /data/local/tmp/android_server
# 这条命令适用于普通机型;
 #adb shell "su -c \"/data/local/tmp/android_server \""
 #这条是pixel xl2 
adb shell "su   0 /data/local/tmp/android_server "
