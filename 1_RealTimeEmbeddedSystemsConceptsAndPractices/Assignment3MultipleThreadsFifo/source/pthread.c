#define _GNU_SOURCE
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <syslog.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include "globalDefine.h"

// POSIX thread declarations and scheduling attributes
pthread_t threads[NUM_THREADS];
threadParams_t threadParams[NUM_THREADS];
pthread_attr_t attr;
pthread_t mainthread;
cpu_set_t cpuset;

/**
 * @brief Prints a message with thread id, sum from 1 to thread id, and core running on CPU.
 * @param threadp A pointer to a threadParams_t structure that contains the thread index.
 * @return void.
*/
void *printMessageThread(void *threadp)
{
    threadParams_t *params = (threadParams_t *)threadp;
    int sum = 0;
    int core = sched_getcpu();

    for (int i = 1; i <= params->threadIdx; i++) {
        sum += i;
    }

    openlog("pthread", LOG_PID|LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:3]: Thread idx=%d, sum[1...%d]=%d Running on core: %d", params->threadIdx, params->threadIdx, sum, core);
    closelog();
    pthread_exit(NULL);
}

/**
 * @brief Sets the scheduling policy and CPU affinity for the threads.
 * 
 * This function sets the scheduling policy to FIFO and the CPU affinity to a single core. 
 * It initializes the thread attribute object with the scheduling policy and CPU affinity. 
 * The CPU affinity ensures that each thread runs only on the specified core, which can 
 * improve performance by reducing cache invalidation. 
 * 
 * @return void.
 */
void set_scheduler(void)
{
    int cpu_set_t;
    int cpuIndex;

    // zero out the set of CPU cores.
    CPU_ZERO(&cpuset);

    //Here we assign the threads to run ONLY on core 2.
    cpuIndex=(1);
    CPU_SET(cpuIndex, &cpuset);
    
    // Set scheduling policy to FIFO
    struct sched_param schedParam;
    schedParam.sched_priority = sched_get_priority_max(SCHED_FIFO); //99
    sched_setscheduler(0, SCHED_FIFO, &schedParam);

    // Set thread attributes to use FIFO scheduling policy
    pthread_attr_init(&attr);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
    pthread_attr_setschedparam(&attr, &schedParam);
}

/**
 * @brief The main function of the program.
*/
int main (int argc, char *argv[])
{
    struct utsname unameData;
    char buffer[1024];
    
    set_scheduler();
    
    mainthread = pthread_self();
    pthread_attr_setaffinity_np(&mainthread, sizeof(cpu_set_t), &cpuset);

    // Clear the syslog file
    system("truncate -s 0 /var/log/syslog");

    // execute uname -a and read output into buffer
    FILE* uname_output = popen("uname -a", "r");
    fgets(buffer, sizeof(buffer), uname_output);
    pclose(uname_output);

    openlog("pthread", LOG_PID|LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:3]: %s", buffer);
    closelog();    

    for(int i = 0; i < NUM_THREADS; i++)
    {
        threadParams[i].threadIdx = i+1;
        pthread_create(&threads[i], &attr, printMessageThread, (void *)&threadParams[i]);
    }

    for(int i = 0; i < NUM_THREADS; i++)
    {
       pthread_join(threads[i], NULL);
    }

    return 0;
}
