#!/bin/bash
echo "" > profiles
filesizes=`echo 1000 2000 4000 7000 8000 9000 11000 14000`
workloads=`echo ws.f`
for loadwork in $workloads
do

times=1
for filesize in $filesizes
do
#filesize=15g
time_in_sec=600

workload_file=$loadwork

sed -e 's/\(set\) \($nfiles=\).*/\1 \2'$filesize'/' -i $workload_file 

echo "'" | sudo -S rm -rf /mnt/*

trash-empty

echo "'" | sudo -S echo "Now Filebench Starts"

echo 0 | sudo tee /proc/sys/kernel/randomize_va_space

echo "'" | sudo -S sync && echo 3 | sudo tee /proc/sys/vm/drop_caches

dmesg | grep vmli	>> profiles

echo "#profile for " $filesize workload  file: $workload_file >> profiles
echo "'" | sudo -S filebench -f $workload_file >>  profiles &
PID1=$!
./create_log.sh log_$filesize &
PID2=$!
wait $PID1
echo "FIlebench Finished"
kill $PID2

(( times-- ))
done

newfolder=`date +%H%M%S`
newfolder=`echo $loadwork$newfolder`
mkdir $newfolder

mv log_* $newfolder
mv profiles $newfolder

done
