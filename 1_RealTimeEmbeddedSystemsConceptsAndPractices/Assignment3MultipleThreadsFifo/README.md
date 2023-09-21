Instructions

This assignment requires to implement 128 real -time threads with FIFO scheduling policy printing the thread id [1 ...128] , sum from 1 to thread id and core which it is running on CPU.    The assignment should be executed on Linux system running on raspberry pi. Submit the syslog file with the logs printed as below guidelines

Print the output of uname -a in the first line of the syslog file.

Each syslog statement should have [COURSE:X][ASSIGNMENT:Y] where X corresponds to Course Number i.e. 1 in this case and Y corresponds to Assignment No.

Its expected to have syslogs for 128 threads with respective thread idx and sum printed on a single line for respective thread in the following format as below

<Current System Time>  <Host Name> [COURSE:1][ASSIGNMENT:3]: Thread idx=10, sum[1...10]=55 Running on core : XYZ

It's expected to use First Input First Output (FIFO) scheduling mechanism governing the execution of the threads with 99 as maximum priority and 1 as minimum priority.