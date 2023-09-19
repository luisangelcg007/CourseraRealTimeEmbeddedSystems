Instructions

This Programming Assignment requires to create ,execute and join 128 threads in the Linux system running on raspberry pi with no scheduling policy governing them. Submit the syslog file with the logs printed as below guidelines

Print the output of uname -a in the first line of the syslog file. 

Each syslog statement should have [COURSE:X][ASSIGNMENT:Y] where X corresponds to Course Number i.e. 1 in this case and Y corresponds to Assignment No.

Its expected to have syslogs for 128 threads with respective thread idx and sum printed on a single line for respective thread in the following format as below 

<System Time>  <Host Name>  [COURSE:1][ASSIGNMENT:2]: Thread idx=10, sum[1...10]=55

It's not required to use any scheduling mechanisms governing the execution of the threads.