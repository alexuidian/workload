#!/bin/bash

echo "# file server with tmem tesing" > profiles
filesizes=`echo 5000 5500 6000 6500 7000 7500 8000 8500 9000 9500`
#filesizes=`echo 5000`
workloads=`echo fs.f`
for loadwork in $workloads
do

times=1
for filesize in $filesizes
do
#filesize=15g
time_in_sec=600

workload_file=$loadwork

sed -e 's/\(set\) \($nfiles=\).*/\1 \2'$filesize'/' -i $workload_file 

sed -e 's/\(.*\) #running/run '$time_in_sec' #running/' -i $workload_file

echo "'" | sudo -S rm -rf /mnt/bigfileset/ /mnt/logfiles/

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
