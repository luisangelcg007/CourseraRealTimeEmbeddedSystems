#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <syslog.h>
#include <sys/utsname.h>
#include "globalDefine.h"

// POSIX thread declarations and scheduling attributes
pthread_t thread[NUM_THREADS];
threadParams_t threadParams[NUM_THREADS];

/**
 * @brief Prints a "Hello World from Thread" message.
 * @param threadp A pointer to a threadParams_t structure that contains the thread index.
 * @return void.
*/
void *printMessageThread(void *threadp)
{
    threadParams_t *params = (threadParams_t *)threadp;
    int sum = 0;
    for (int i = 1; i <= params->threadIdx; i++) {
        sum += i;
    }

    openlog("pthread", LOG_PID|LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:2]: Thread idx=%d, sum[1...%d]=%d", params->threadIdx, params->threadIdx, sum);
    closelog();
    pthread_exit(NULL);
}

/**
 * @brief The main function of the program.
*/
int main (int argc, char *argv[])
{
    struct utsname unameData;
    char buffer[1024];

    // Clear the syslog file
    system("truncate -s 0 /var/log/syslog");

    // execute uname -a and read output into buffer
    FILE* uname_output = popen("uname -a", "r");
    fgets(buffer, sizeof(buffer), uname_output);
    pclose(uname_output);

    openlog("pthread", LOG_PID|LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "[COURSE:1][ASSIGNMENT:2]: %s", buffer);
    closelog();

    for(int i = 1; i <= NUM_THREADS; i++)
    {
        threadParams[i].threadIdx=i;

        pthread_create(&thread[i], NULL, printMessageThread, (void *)&threadParams[i]);
        pthread_join(thread[i], NULL);
    }

    for(int i = 0; i < NUM_THREADS; i++)
    {
       pthread_join(thread[i], NULL);
    }

    return 0;
}
