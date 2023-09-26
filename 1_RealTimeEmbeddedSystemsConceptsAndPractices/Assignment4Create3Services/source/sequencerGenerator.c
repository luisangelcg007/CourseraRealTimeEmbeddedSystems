#define _GNU_SOURCE
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <syslog.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <errno.h>
#include <time.h> 
#include <signal.h>
#include <unistd.h>
#include <semaphore.h>
#include "globalDefine.h"

// POSIX thread declarations and scheduling attributes
pthread_t threads[NUM_THREADS];
threadParams_t threadParams[NUM_THREADS];
pthread_attr_t attr;
pthread_t mainthread;
cpu_set_t cpuset;
static timer_t timer_1;
static struct itimerspec itime = {{1,0}, {1,0}};
static struct itimerspec last_itime;
static unsigned long long interruptCounter = 0;
static unsigned long long totalCountere = 0;
sem_t semS1, semS2, semS3;

// it is called every 10 ms
void Sequencer(int id)
{
    struct timespec current_time_val;
    double current_realtime;
    int rc, flags=0;

    if(interruptCounter ==30)
    {
        interruptCounter = 0;
    }

    if(interruptCounter % 2)
    {
        //Service_1 start
        sem_post(&semS1);
    }
    
    if(interruptCounter == 1 || 
    interruptCounter == 11 || 
    interruptCounter == 21)
    {
        //Service_2 start
        sem_post(&semS2);
    }

    if(interruptCounter == 3 ||
    interruptCounter == 6 ||
    interruptCounter == 5 ||
    interruptCounter == 15 ||
    interruptCounter == 17)
    {
        //Service_2 start
        sem_post(&semS3);
    }
    
    // received interval timer signal
    interruptCounter++;
    totalCountere++;
}

void *Service_1(void *threadp)
{
    while(TRUE)
    {
        // wait for service request from the sequencer
        sem_wait(&semS1);
        //Do work

        openlog("pthread", LOG_PID|LOG_CONS, LOG_USER);
        syslog(LOG_INFO, "[Service Generator]: S1 T1=%d ms", (totalCountere*10L));
        closelog();
        pthread_exit(NULL);
    }
}

void *Service_2(void *threadp)
{
    while(TRUE)
    {
        // wait for service request from the sequencer
        sem_wait(&semS2);
        //Do work

        openlog("pthread", LOG_PID|LOG_CONS, LOG_USER);
        syslog(LOG_INFO, "[Service Generator]: S2 T2=%d ms", (totalCountere*10L));
        closelog();
        pthread_exit(NULL);
    }
}

void *Service_3(void *threadp)
{
    while(TRUE)
    {
        // wait for service request from the sequencer
        sem_wait(&semS3);
        //Do work

        openlog("pthread", LOG_PID|LOG_CONS, LOG_USER);
        syslog(LOG_INFO, "[Service Generator]: S3 T3=%d ms", (totalCountere*10L));
        closelog();
        pthread_exit(NULL);
    }
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
    int flags=0;
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
    syslog(LOG_INFO, "[Service Generator]: %s", buffer);
    closelog();    

    pthread_create(&threads[0], &attr, Service_1, (void *)&threadParams[0]);
    pthread_create(&threads[1], &attr, Service_2, (void *)&threadParams[0]);
    pthread_create(&threads[2], &attr, Service_3, (void *)&threadParams[0]);

    // Sequencer = RT_MAX	@ 100 Hz
    //
    /* set up to signal SIGALRM if timer expires */
    timer_create(CLOCK_REALTIME, NULL, &timer_1);
    signal(SIGALRM, (void(*)()) Sequencer);
    itime.it_interval.tv_sec = 0;
    itime.it_interval.tv_nsec = 10000000; //10 ms
    itime.it_value.tv_sec = 0;
    itime.it_value.tv_nsec = 10000000;
    timer_settime(timer_1, flags, &itime, &last_itime);



    for(int i = 0; i < NUM_THREADS; i++)
    {
       pthread_join(threads[i], NULL);
    }

    return 0;
}
