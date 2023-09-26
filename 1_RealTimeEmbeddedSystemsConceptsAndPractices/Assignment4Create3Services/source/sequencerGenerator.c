#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <semaphore.h>
#include <syslog.h>
#include "globalDefine.h"

#define USEC_PER_MSEC (1000)

struct timeval start_time_val;
int abortTest=FALSE;
int abortS3=FALSE;
sem_t semS3;

// Service_3 thread

void *Service_3(void *threadp)
{
    struct timeval current_time_val;
    double current_time;
    unsigned long long S3Cnt=0;
    threadParams_t *threadParams = (threadParams_t *)threadp;

    gettimeofday(&current_time_val, (struct timezone *)0);
    syslog(LOG_CRIT, "Difference Image Proc thread @ sec=%d, msec=%d\n", (int)(current_time_val.tv_sec-start_time_val.tv_sec), (int)current_time_val.tv_usec/USEC_PER_MSEC);
    printf("Difference Image Proc thread @ sec=%d, msec=%d\n", (int)(current_time_val.tv_sec-start_time_val.tv_sec), (int)current_time_val.tv_usec/USEC_PER_MSEC);

    while(!abortS3)
    {
        sem_wait(&semS3);
        S3Cnt++;

        gettimeofday(&current_time_val, (struct timezone *)0);
        syslog(LOG_CRIT, "Difference Image Proc release %llu @ sec=%d, msec=%d\n", S3Cnt, (int)(current_time_val.tv_sec-start_time_val.tv_sec), (int)current_time_val.tv_usec/USEC_PER_MSEC);
    }

    pthread_exit((void *)0);
}


// Sequencer thread

void *Sequencer(void *threadp)
{
    struct timeval current_time_val;
    double current_time;
    unsigned long long seqCnt=0;
    threadParams_t *threadParams = (threadParams_t *)threadp;

    gettimeofday(&current_time_val, (struct timezone *)0);
    syslog(LOG_CRIT, "Sequencer thread @ sec=%d, msec=%d\n", (int)(current_time_val.tv_sec-start_time_val.tv_sec), (int)current_time_val.tv_usec/USEC_PER_MSEC);
    printf("Sequencer thread @ sec=%d, msec=%d\n", (int)(current_time_val.tv_sec-start_time_val.tv_sec), (int)current_time_val.tv_usec/USEC_PER_MSEC);

    do
    {
        seqCnt++;
        // 30 Hz
        if((seqCnt % 30) == 0) sem_post(&semS3);

        gettimeofday(&current_time_val, (struct timezone *)0);
        syslog(LOG_CRIT, "Sequencer cycle %llu @ sec=%d, msec=%d\n", seqCnt, (int)(current_time_val.tv_sec-start_time_val.tv_sec), (int)current_time_val.tv_usec/USEC_PER_MSEC);
    } while(!abortTest && (seqCnt < threadParams->sequencePeriods));

    sem_post(&semS3);
    abortS3=TRUE;

    pthread_exit((void *)0);
}


int main(void)
{
    int i, rc;
    struct timeval current_time_val;
    double current_time;
    threadParams_t threadParams[NUM_THREADS];

    printf("Starting Sequencer Demo\n");
    gettimeofday(&start_time_val, (struct timezone *)0);

    // initialize the sequencer semaphore
    sem_init(&semS3, 0, 0);

    // create the sequencer thread

    rc = pthread_create(&threadParams[0].threadId, NULL, Sequencer, &threadParams[0]);
    if (rc)
    {
        printf("Error creating sequencer thread\n");
        exit(-1);
    }

    // create the service_3 thread

    rc = pthread_create(&threadParams[1].threadId, NULL, Service_3, &threadParams[1]);
    if (rc)
    {
        printf("Error creating service_3 thread\n");
        exit(-1);
    }

    // wait for all threads to terminate

    for (i=0; i<NUM_THREADS; i++)
    {
        rc = pthread_join(threadParams[i].threadId, NULL);
        if (rc)
        {
            printf("Error joining thread %d\n", i);
            exit(-1);
        }
    }

    printf("Sequencer Demo finished\n");

    return 0;
}