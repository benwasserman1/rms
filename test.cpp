#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <sched.h>

using namespace std;

// Declare semaphores
sem_t *thread1;
sem_t *thread2;
sem_t *thread3;
sem_t *thread4;

// Declare counters
int thread1_count;
int thread2_count;
int thread3_count;
int thread4_count;

// Declare overrun counters
int thread1_overrun;
int thread2_overrun;
int thread3_overrun;
int thread4_overrun;

// Initialize the board
int board[10][10];


void *schedule(void *status) {
    // Do the schedule thing that is in my notebook

    while (true) {
      usleep(10);
      //call threads
    }
}


/*
Comment function
*/
void *doWork(void *status) {

  int thread = (int)(intptr_t)status;
  if (thread == 1) {
    thread1_count++;
  }
  else if (thread == 2) {
    thread2_count++;
  }
  else if (thread == 3) {
    thread3_count++;
  }
  else {
    thread4_count++;
  }

  int s = 5;
  int k = 0;

  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      // multiply board[0][0] by column 0, 5, 1, 6, 2, 7, 3, 8, 4, 9
      if (i % 2 == 0) {
        board[j][k] *= 1;
      }
      else {
        board[j][s] *= 1;
      }
    }
    if (i%2 == 0) {
      k+=1;
    }
    else {
      s+=1;
    }
  }
  pthread_exit(NULL);
}

int main() { // Main thread here

  printf("******* Welcome to our rate monotonic scheduler *********");

  int k = 1;
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      board[i][j] = k;
      k += 1;
      printf("value: %d", board[i][j]);
    }
  }

  pthread_t scheduler;
  pthread_t first_thread;
  pthread_t second_thread;
  pthread_t third_thread;
  pthread_t fourth_thread;

  // pthread_attr_t attr;
  // cpu_set_t cpus;
  // pthread_attr_init(&attr);
  // CPU_ZERO(&cpus);
  // CPU_SET(0, &cpus);
  //
  // thread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpus);

  // void *status = 0;
  // if (pthread_create(&scheduler, NULL, schedule, NULL)) {
  //   fprintf(stderr, "Error creating the scheduler\n");
  // }

  int x = 0;
  int i = 0;

  int thread1 = 1;
  int thread2 = 2;
  int thread3 = 3;
  int thread4 = 4;

  while (x < 10) {
    usleep(10);
    void *status1 = 0;
    if(pthread_create(&first_thread, NULL, doWork, (void *)(intptr_t)thread1)) {
      fprintf(stderr, "Error creating thread 1");
      return 1;
    }

    void *status2 = 0;
    for (i = 0; i < 2; ++i)
    {
      if(pthread_create(&second_thread, NULL, doWork,(void *)(intptr_t)thread2)) {
        fprintf(stderr, "Error creating thread 2");
        return 1;
      }
    }


    void *status3 = 0;
    for (i = 0; i < 4; ++i) {
      if(pthread_create(&third_thread, NULL, doWork, (void *)(intptr_t)thread3)) {
        fprintf(stderr, "Error creating thread 3");
        return 1;
      }
    }


    void *status4 = 0;
    for (i = 0; i < 16; ++i) {
      if(pthread_create(&fourth_thread, NULL, doWork, (void *)(intptr_t)thread4)) {
        fprintf(stderr, "Error creating thread 4");
        return 1;
      }
    }
    x++;
  }

  // Join the threads
  pthread_join(first_thread, NULL);
  printf("First thread joined");
  pthread_join(second_thread, NULL);
  printf("Second thread joined");
  pthread_join(third_thread, NULL);
  printf("Third thread joined");
  pthread_join(fourth_thread, NULL);
  printf("Fourth thread joined");

  pthread_join(scheduler, NULL);
  printf("Scheduler joined");

  printf("First thread joined: %d", thread1_count);
  printf("First thread joined: %d", thread2_count);
  printf("First thread joined: %d", thread3_count);
  printf("First thread joined: %d", thread4_count);
}
