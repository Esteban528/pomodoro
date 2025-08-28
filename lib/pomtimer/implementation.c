#include "interface.h"
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

typedef struct {
  uint8_t main_time;
  uint8_t rest_time;
  uint8_t long_rest_time;
  uint8_t times;
  pomtimer_runnable func;
} time_config_t;

static pthread_t     thr;
static time_config_t tc;

uint8_t current_time;
bool focused;
volatile bool running = false;

void* thrd_fun   (void*);
uint8_t   init_thread(time_config_t*);
void  sleep_run  (uint8_t);

uint8_t start_pom(uint8_t main_time, uint8_t rest, uint8_t rest_long, uint8_t times, pomtimer_runnable func) {
  tc.main_time = main_time;
  tc.rest_time = rest;
  tc.times     = times;
  tc.func      = func;
  tc.long_rest_time = rest_long;
  
  running = true;
  init_thread(&tc);

  return 1;
}

void stop_pom(pomtimer_runnable func){
  running = false;
  func(NULL);
}

uint8_t init_thread(time_config_t *time_data) {

  if (pthread_create(&thr, NULL, thrd_fun, time_data) != 0) {
      perror("pthread_create failed");
      return -1;
  }
  //pthread_detach(thr);

  // pthread_join(thr, NULL);
  return 1;
}

void *thrd_fun(void *arg) {
  time_config_t *time_data = (time_config_t *) arg;

  pomtimer_status_t status = {
    .local_times = 0,
    .sleep_time  = time_data->main_time,
    .focused = false
  };

  printf("thread running\n");

  while (running) {

    if (status.local_times < time_data->times && status.focused == false) {
      status.sleep_time = time_data->main_time;
      status.focused    = true;
      status.local_times++;

    } else if (status.local_times < time_data->times && status.focused == true) {
      status.sleep_time = time_data->rest_time;
      status.focused = false;

    } else if (status.local_times >= time_data->times) {
      status.sleep_time = time_data->long_rest_time;
      status.focused = false;
      status.local_times = 0;
    }

    focused = status.focused;
    time_data->func(&status);
    sleep_run(status.sleep_time);
  }


  return NULL;
}

void sleep_run(uint8_t min){
  current_time = 0;
  for (uint16_t i = 0; (i < (minconvert(min))) && running; i++){
    sleep(1);
    if (i % 60 == 0)
      current_time++;
  }
}

uint16_t minconvert(uint8_t min) {
  return min*60; 
}
