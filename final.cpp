/*
Ben Wasserman
CPSC 380: Operating Systems
May 6th, 2018
2280906
Professor Joseph Hansen

Purpose of this source file:
*/

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>

using namespace std;

// Declare semaphores
sem_t *thread1;
sem_t *thread2;
sem_t *thread3;
sem_t *thread4;

//Declare threads
pthread_t first_thread;
pthread_t second_thread;
pthread_t third_thread;
pthread_t fourth_thread;

// Declare counters
int thread1_count = 0;
int thread2_count = 0;
int thread3_count = 0;
int thread4_count = 0;

// Declare overrun counters
int thread1_overrun = 0;
int thread2_overrun = 0;
int thread3_overrun = 0;
int thread4_overrun = 0;

// Initialize the board
int board[10][10];

int doWorkCount = 0;

int first = 0;
int second = 0;
int third = 0;
int fourth = 0;

int flag1 = 0;
int completion_check1 = 0;
int completion_check2 = 0;
int completion_check3 = 0;
int completion_check4 = 0;

void doWork() {

  int s = 5;
  int k = 0;
  int x = 0;

  first++;

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
}


void *thread_first(void *status1) {
  while(flag1 == 0) {
    completion_check1 = 1;
    sem_wait(thread1);
    completion_check1 = 0;
    if (thread1_count != 160) {
      thread1_count++;
    }    
    for (int i = 0; i < 1; i++){
      doWork();
    }
  }
  pthread_exit(NULL);
}

void *thread_second(void *status2) {
  while(flag1 == 0) {
    completion_check2 = 1;
    sem_wait(thread2);
    completion_check2 = 0;
    if (thread2_count != 80) {
      thread2_count++;
    }
    for (int i = 0; i < 2; i++){
      doWork();
    }
  }
  pthread_exit(NULL);
}

void *thread_third(void *status3) {
  while(flag1 == 0) {
    completion_check3 = 1;
    sem_wait(thread3);
    completion_check3 = 0;
    if (thread3_count != 40) {
      thread3_count++;
    }
    for (int i = 0; i < 4; i++){
      doWork();
    }
  }
  pthread_exit(NULL);
}

void *thread_fourth(void *status4) {
  while(flag1 == 0) {
    completion_check4 = 1;
    sem_wait(thread4);
    completion_check4 = 0;
    if (thread4_count != 10) {
      thread4_count++;
    }
    for (int i = 0; i < 16; i++){
      doWork();
    }
  }
  pthread_exit(NULL);
}

void *schedule(void *status) {
    // Do the schedule thing that is in my notebook

    while (doWorkCount < 160) {
      usleep(10000);
      // call threads and make appropriate checks
      // Who needs to run based on period
      if (doWorkCount % 16 == 0) {
        sem_post(thread1);
        sem_post(thread2);
        sem_post(thread3);
        sem_post(thread4);
      }
      else if (doWorkCount % 4 == 0) {
        sem_post(thread1);
        sem_post(thread2);
        sem_post(thread3);
      }
      else if (doWorkCount % 2 ==  0) {
        sem_post(thread1);
        sem_post(thread2);
      }
      else {
        sem_post(thread1);
      }
      doWorkCount++;
    }
    flag1 = 1;
    // usleep(100000);
    sem_post(thread1);
    sem_post(thread2);
    sem_post(thread3);
    sem_post(thread4);
    pthread_exit(NULL);
}

int main() { // Main thread here

  printf("******* Welcome to our rate monotonic scheduler *********\n");

  // Initialize the semaphores
  thread1 = sem_open("first_sem", O_CREAT| O_CLOEXEC, S_IRUSR | S_IWUSR, 0);
  thread2 = sem_open("second_sem", O_CREAT| O_CLOEXEC, S_IRUSR | S_IWUSR, 0);
  thread3 = sem_open("third_sem", O_CREAT| O_CLOEXEC, S_IRUSR | S_IWUSR, 0);
  thread4 = sem_open("fourth_sem", O_CREAT| O_CLOEXEC, S_IRUSR | S_IWUSR, 0);

  // int set1 = sem_post(thread1);
  // int set2 = sem_post(thread2);
  // int set3 = sem_post(thread3);
  // int set4 = sem_post(thread4);

  //Initialize the board cells
  int k = 1;
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < 10; ++j) {
      board[i][j] = k;
      k += 1;
      // cout << board[i][j];
    }
    // cout << endl;
  }

  // cpu_set_t cpuset;

  //Set scheduler thread priority
  pthread_attr_t tattr;
  int ret;
  int newprio = 20;
  sched_param param;

  /* initialized with default attributes */
  ret = pthread_attr_init (&tattr);

  // /* safe to get existing scheduling param */
  // ret = pthread_attr_getschedparam (&tattr, &param);

  /* set the priority; others are unchanged */
  param.sched_priority = newprio;

  /* setting the new scheduling param */
  ret = pthread_attr_setschedparam (&tattr, &param);


  pthread_t scheduler;

  void *status = 0;
  if (pthread_create(&scheduler, &tattr, schedule, NULL)) {
    fprintf(stderr, "Error creating the scheduler\n");
  }

  int x = 0;
  int i = 0;

  int thread1 = 1;
  int thread2 = 2;
  int thread3 = 3;
  int thread4 = 4;

  newprio = 10;
  param.sched_priority = newprio;
  ret = pthread_attr_setschedparam (&tattr, &param);

  void *status1 = 0;
  if(pthread_create(&first_thread, &tattr, thread_first, (void *)(intptr_t)thread1)) {
    fprintf(stderr, "Error creating thread 1\n");
    return 1;
  }

  newprio = 5;
  param.sched_priority = newprio;
  ret = pthread_attr_setschedparam (&tattr, &param);

  void *status2 = 0;
  if(pthread_create(&second_thread, &tattr, thread_second,(void *)(intptr_t)thread2)) {
    fprintf(stderr, "Error creating thread 2\n");
    return 1;
  }

  newprio = 2;
  param.sched_priority = newprio;
  ret = pthread_attr_setschedparam (&tattr, &param);

  void *status3 = 0;
  if(pthread_create(&third_thread, &tattr, thread_third, (void *)(intptr_t)thread3)) {
    fprintf(stderr, "Error creating thread 3\n");
    return 1;
  }

  newprio = 1;
  param.sched_priority = newprio;
  ret = pthread_attr_setschedparam (&tattr, &param);

  void *status4 = 0;
  if(pthread_create(&fourth_thread, &tattr, thread_fourth, (void *)(intptr_t)thread4)) {
    fprintf(stderr, "Error creating thread 4\n");
    return 1;
  }

  // Join the threads
  pthread_join(first_thread, NULL);
  printf("First thread joined\n");
  pthread_join(second_thread, NULL);
  printf("Second thread joined\n");
  pthread_join(third_thread, NULL);
  printf("Third thread joined\n");
  pthread_join(fourth_thread, NULL);
  printf("Fourth thread joined\n");

  pthread_join(scheduler, NULL);
  printf("Scheduler joined\n");

  printf("First thread count: %d\n", thread1_count);
  printf("Second thread count: %d\n", thread2_count);
  printf("Third thread count: %d\n", thread3_count);
  printf("Fourth thread count: %d\n", thread4_count);

}
