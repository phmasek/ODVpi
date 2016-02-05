#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sched.h>
#include <sys/mman.h>
#include <string.h>

#define MY_PRIORITY (49) /* we use 49 as the PRREMPT_RT use 50
                            as the priority of kernel tasklets
                            and interrupt handler by default */

#define MAX_SAFE_STACK (8*1024) /* The maximum stack size which is
                                   guaranteed safe to access without
                                   faulting */

#define NSEC_PER_SEC    (1000000000) /* The number of nsecs per sec. */

void stack_prefault(void) {

        unsigned char dummy[MAX_SAFE_STACK];

        memset(dummy, 0, MAX_SAFE_STACK);
        return;
}

int main(int argc, char* argv[]) {

        struct timespec t;
        struct sched_param param;
        int interval = NSEC_PER_SEC / 100; /* 1000hz */

        /* Declare ourself as a real time task */

        param.sched_priority = MY_PRIORITY;
        if(sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
            perror("sched_setscheduler failed");
            exit(-1);
        }

        /* Lock memory */

        if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1) {
            perror("mlockall failed");
            exit(-2);
        }

        /* Pre-fault our stack */

        stack_prefault();

        clock_gettime(CLOCK_MONOTONIC ,&t);
        /* start after one second */
        t.tv_sec++;

        // PI VARIABLES
        long double pi = 4.0;
        long double tempPi;
        int i = 1;
        int j = 3;


        while(1) {
            /* wait until next shot */
            clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);

            // PI Algorithm
            // tempPi = (double)(4)/j;
            
            // if (i%2 != 0) {
            //     pi -= tempPi;
            // } else if (i%2 == 0) {
            //     pi += tempPi;
            // }

            // i++;
            // j+=2;
            // :end of algorithm

            /* calculate next shot */
            printf("%ld\n", t.tv_nsec/1000000);
            t.tv_nsec += interval;
            

            while (t.tv_nsec >= NSEC_PER_SEC) {
                // printf("%ld\n", t.tv_nsec);
                t.tv_nsec -= NSEC_PER_SEC;
                // t.tv_sec++;

                
                // pi = 4.0;
                // i = 1;
                // j = 3;
            }
        }
}