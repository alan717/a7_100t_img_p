game=com.tencent.tmgp.speedmobile/com.tencent.tmgp.speedmobile.speedmobile
adb shell "am start -n $game " 1>/dev/null 2>&1
gamepid=`adb shell ps | grep speed | awk 'NR=="1"{print $2}'`
echo "game:$game.PID:$gamepid"
adb forward tcp:1234 tcp:1234
adb forward tcp:12345 jdwp:12345
#adb shell
gdbpid=`adb shell ps | grep gdbserver |awk 'NR=="1"{print $2}'`
if [ ! -z "$gdbpid" ]; then
    echo "gdbserver is running:$gdbpid"
    echo "gdbpid :"$gdbpid ."kill it!"
    adb shell su -c "kill -9 $gdbpid"
else
    echo "gdbserver first running! :=)"
fi



adb shell su -c  "/data/local/tmp/gdbserver :1234 --attach $gamepid"
