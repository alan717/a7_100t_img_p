#@echo on
adb forward tcp:23946 tcp:23946
gdbpid=`adb shell ps | grep android_server |awk 'NR=="1"{print $2}'`
if [ ! -z "$gdbpid" ]; then
    echo "idaserver is running:$gdbpid"
    echo "gdbpid :"$gdbpid ."kill it!"
    adb shell su -c "kill -9 $gdbpid"
else
    echo "gdbserver first running! :=)"
fi
adb shell am start -D -n com.ss.android.ugc.aweme/com.ss.android.ugc.aweme.splash.SplashActivity
adb shell "su -c \"/data/local/tmp/android_server \""
 

