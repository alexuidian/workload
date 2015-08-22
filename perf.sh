#!/bin/bash
cp profiles profiles.bak
echo "" > profiles

echo "'" | sudo -S  chmod 777 /sys/kernel/debug/

filesizes=`echo 200m 100m 300m`
#filesizes=`echo 2g`
workloads=`echo rr.f`

for loadwork in $workloads
do

echo "profile for $workload"

for each in `seq 1`
do


echo `dmesg | grep vmlinu` >> profiles

for filesize in $filesizes
do

failedgets="`cat /sys/kernel/debug/cleancache/failed_gets`"
flushes="`cat /sys/kernel/debug/cleancache/flushes`"
puts="`cat /sys/kernel/debug/cleancache/puts`"
succ_gets="`cat /sys/kernel/debug/cleancache/succ_gets`"

#filesize=15g
time_in_sec=300
dir="\/mnt"
iosize=2k
nthreads=1
workingset=0
directio=0
memsize=200m
reuse=",reuse"
paralloc=",paralloc"
prealloc=",prealloc"
name=largefile1_$filesize
workload_file=$loadwork

sed -e 's/\(set\) \($dir=\).*/\1 \2'$dir'/' -i $workload_file
#echo 1
sed -e 's/\(set\) \($filesize=\).*/\1 \2'$filesize'/' -i $workload_file 
#echo 2
sed -e 's/\(set\) \($iosize=\)\([0-9]*[a-z]*\)/\1 \2'$iosize'/' -i $workload_file
#echo 3
sed -e 's/\(set\) \($nthreads=\)\([0-9]*[a-z]*\)/\1 \2'$nthreads'/' -i $workload_file
#echo 4
sed -e 's/\(set\) \($workingset=\)\([0-9]*[a-z]*\)/\1 \2'$workingset'/' -i $workload_file
#echo 5
sed -e 's/\(set\) \($directio=\)\([0-9]*[a-z]*\)/\1 \2'$directio'/' -i $workload_file

sed -e 's/\(set\) \($filename=\).*/\1 \2'$name'/' -i $workload_file


#echo 6

sed -e 's/\(set\) \($memsize=\).*/\1 \2'$memsize'/' -i $workload_file


#sed -e 's/\(define file \)\(name=.*\)\(,path=\).*\(,size=\).*/\1\2\3$dir\4$filesize'$prealloc''$reuse''$paralloc'/' -i $workload_file

sed -e 's/\(.*\) #running/run '$time_in_sec' #running/' -i $workload_file


echo "'" | sudo -S echo "Now Filebench Starts"

echo 0 | sudo tee /proc/sys/kernel/randomize_va_space

echo "'" | sudo -S sync && echo 3 | sudo tee /proc/sys/vm/drop_caches

echo "#profile for " $filesize workload $workload_file >> profiles

echo "'" | sudo -S filebench -f $workload_file >>  profiles &
PID1=$!
./create_log.sh log_$filesize &
PID2=$!
wait $PID1
echo "FIlebench Finished"
kill $PID2


failedgets2="`cat /sys/kernel/debug/cleancache/failed_gets`"
flushes2="`cat /sys/kernel/debug/cleancache/flushes`"
puts2="`cat /sys/kernel/debug/cleancache/puts`"
succ_gets2="`cat /sys/kernel/debug/cleancache/succ_gets`"

echo "# cleanache logs for $filesize " >> profile_cleancache
echo "# failedgets flushes puts succ_gets" >> profile_cleancache
echo `expr $failedgets2 - $failedgets` `expr $flushes2 - $flushes` `expr $puts2 - $puts` `expr $succ_gets2 - $succ_gets` >> profile_cleancache

done

newfolder=`date +%y%m%d%H%M%S`
newfolder=`echo s_nocl-$loadwork$newfolder`
mkdir $newfolder

mv log_* $newfolder
mv profiles $newfolder
mv profile_cleancache $newfolder


done


done
