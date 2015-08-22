set $dir=/mnt
set $filesize=6000m
set $iosize=400m
set $nthreads=20
set $workingset=0
set $directio=0
set $filename=largefile4_600m
set $memsize=200m

define file name=$filename,path=$dir,size=$filesize,prealloc,reuse,paralloc

#define file name=largefile1_4g,path=$dir,size=$filesize,prealloc,reuse,paralloc

define process name=rand-read,instances=1
{
  thread name=rand-thread,memsize=$memsize,instances=$nthreads
  {
    flowop read name=rand-read1,filename=$filename,iosize=$iosize,random,workingset=$workingset,directio=$directio
  }

#  thread name=seqread,memsize=$memsize,instances=$nthreads
#  {
#    flowop read name=seqread,filename=$filename,iosize=$iosize
#  }

}

run 300 #running


echo "Random Read Version 3.0 personality successfully loaded"
usage "Usage: set \$dir=<dir>         defaults to $dir"
usage "       set \$filesize=<size>   defaults to $filesize"
usage "       set \$iosize=<value>    defaults to $iosize"
usage "       set \$nthreads=<value>  defaults to $nthreads"
usage "       set \$workingset=<value>  defaults to $workingset"
usage "       set \$directio=<bool>   defaults to $directio"
usage "       run runtime (e.g. run 60)"


#    name – its name (rand-read1).
#    memsize – the size of its thread memory (5 Mbytes)
#    instances – the number of operating system threads to create ($nthreads).
#    flowop read – read from a file.
#        name – The name of the flowop (rand-read1)
#        filename – The file to read from (largefile1)
#        iosize – The size of each read I/O, set from var $iosize.
#        random – Select each file offset randomly, within the working set.
#        workingset – Use only the first $workingset bytes of the file. If workingset is set to a value of 0, as is $workingset defaults to in this example, then the working set becomes the entire file.
#        directio – If this attribute is used without a value, it enables direct IO. If it is set to a value, as here were it is set to the value of $directio, a value of 0, as $diretio defaults to in this example, disables it, while any other value enables it.
#
#    flowop eventlimit – Limit the execution rate of the flowop sequence in this thread to one pass per event. A separate command is used to establish the rate, which defaults to 0. If the rate is 0, the event subsystem is disabled, and eventlimit becomes a no-op.
#        name – The name of the flowop (rand-rate)
# CDDL HEADER START
#
# default directory for the created files, set to /tmp
#    $nthreads – the number of thread instances to create for the single thread definition, set to 1
#    $iosize – the number of bytes in each I/O, set to 8196.
#    $filesize – the size in bytes of the test file, set to 1048576
#    $workingset – the subregion of the file actually used. Setting to 0 as done here means use the whole file.
#    $directio – bypass the file system or not. Setting to 0 as done here means do not bypass.
#
#Next comes the define file statement, which for randomread.f creates a single file. The attributes set are:
#
#    name – the name of the file, in this case largefile1
#    path – the pathname of the directory in which this largefile1 will be created.
#    size – the size of the file to be created, set to the latest value of $filesize at run time.
#    prealloc – directs FileBench to create and fill the file to the amount specified by the size attribute at the beginning of the run. If missing, a file will be created with size of zero.
#    reuse – directs FileBench to reuse the file if it already exists and is at least as large as specified by the filesize attribute. If too large it will be truncated to the correct size.
# The contents of this file are subject to the terms of the
# Common Development and Distribution License (the "License").
# You may not use this file except in compliance with the License.
#
# You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing permissions
# and limitations under the License.
#
# When distributing Covered Code, include this CDDL HEADER in each
## file and include the License file at usr/src/OPENSOLARIS.LICENSE.
# If applicable, add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your own identifying
# information: Portions Copyright [yyyy] [name of copyright owner]
#
# CDDL HEADER END
#
#
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
set $filesize=200m
set $iosize=2k
set $nthreads=1
set $workingset=0
set $directio=0

define file largefile1name=,path=,size=

#define file largefile1name=,path=,size=

define process name=rand-read,instances=1
{
  thread name=rand-thread,memsize=5m,instances=$nthreads
  {
    flowop read name=rand-read1,filename=largefile1,iosize=$iosize,random,workingset=$workingset,directio=$directio
  }
}

run 300 #running


echo "Random Read Version 3.0 personality successfully loaded"
usage "Usage: set \$dir=<dir>         defaults to $dir"
usage "       set \$filesize=<size>   defaults to $filesize"
usage "       set \$iosize=<value>    defaults to $iosize"
usage "       set \$nthreads=<value>  defaults to $nthreads"
usage "       set \$workingset=<value>  defaults to $workingset"
usage "       set \$directio=<bool>   defaults to $directio"
usage "       run runtime (e.g. run 60)"


#    name – its name (rand-read1).
#    memsize – the size of its thread memory (5 Mbytes)
#    instances – the number of operating system threads to create ($nthreads).
#    flowop read – read from a file.
#        name – The name of the flowop (rand-read1)
#        filename – The file to read from (largefile1)
#        iosize – The size of each read I/O, set from var $iosize.
#        random – Select each file offset randomly, within the working set.
#        workingset – Use only the first $workingset bytes of the file. If workingset is set to a value of 0, as is $workingset defaults to in this example, then the working set becomes the entire file.
#        directio – If this attribute is used without a value, it enables direct IO. If it is set to a value, as here were it is set to the value of $directio, a value of 0, as $diretio defaults to in this example, disables it, while any other value enables it.
#
#    flowop eventlimit – Limit the execution rate of the flowop sequence in this thread to one pass per event. A separate command is used to establish the rate, which defaults to 0. If the rate is 0, the event subsystem is disabled, and eventlimit becomes a no-op.
#        name – The name of the flowop (rand-rate)
# CDDL HEADER START
#
# default directory for the created files, set to /tmp
#    $nthreads – the number of thread instances to create for the single thread definition, set to 1
#    $iosize – the number of bytes in each I/O, set to 8196.
#    $filesize – the size in bytes of the test file, set to 1048576
#    $workingset – the subregion of the file actually used. Setting to 0 as done here means use the whole file.
#    $directio – bypass the file system or not. Setting to 0 as done here means do not bypass.
#
#Next comes the define file statement, which for randomread.f creates a single file. The attributes set are:
#
#    name – the name of the file, in this case largefile1
#    path – the pathname of the directory in which this largefile1 will be created.
#    size – the size of the file to be created, set to the latest value of $filesize at run time.
#    prealloc – directs FileBench to create and fill the file to the amount specified by the size attribute at the beginning of the run. If missing, a file will be created with size of zero.
#    reuse – directs FileBench to reuse the file if it already exists and is at least as large as specified by the filesize attribute. If too large it will be truncated to the correct size.
# The contents of this file are subject to the terms of the
# Common Development and Distribution License (the "License").
# You may not use this file except in compliance with the License.
#
# You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing permissions
# and limitations under the License.
#
# When distributing Covered Code, include this CDDL HEADER in each
## file and include the License file at usr/src/OPENSOLARIS.LICENSE.
# If applicable, add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your own identifying
# information: Portions Copyright [yyyy] [name of copyright owner]
#
# CDDL HEADER END
#
#
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
