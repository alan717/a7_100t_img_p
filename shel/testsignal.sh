#
# Why to trap signal, version 1
#
Take_input1()
{
 recno=0
 clear
 echo "Appointment Note keeper Application for Linux"
 echo -n "Enter your database file name : "
 read filename
if [ ! -f $filename ]; then
  echo "Sorry, $filename does not exit, Creating $filename database"
  echo "Appointment Note keeper Application database file" > $filename
fi
echo "Data entry start data: `date`" >/tmp/input0.$$
#
# Set a infinite loop
#
while :
do
     echo -n "Appointment Title:"
     read na
     echo -n "time :"
     read ti
     echo -n "Any Remark :"
     read remark
     echo -n "Is data okay (y/n) ?"
     read ans
if [ $ans = y -o $ans = Y ]; then
    recno=`expr $recno + 1`
    echo "$recno. $na $ti $remark" >> /tmp/input0.$$
fi
echo -n "Add next appointment (y/n)?"
read isnext
 if [ $isnext = n -o $isnext = N ]; then
     cat /tmp/input0.$$ >> $filename
     rm -f /tmp/input0.$$
    return # terminate loop
 fi
done
}
#
#
# Call our user define function : Take_input1
#
Take_input1
