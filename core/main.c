#include <pomtimer/interface.h>
#include <notify/interface.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

void notify_runnable(pomtimer_status_t *);
void notify_stop_runnable(pomtimer_status_t *);

uint8_t process_action(uint8_t *action){
  printf("Selected: %d\n", *action);

  switch(*action){
    case 1:
      start_pom(25,5,20,4, notify_runnable);
      show("Pomodoro started", "");
      break;
    case 2:
      uint8_t main_time = 25, rest_time = 5, long_rest_time = 20, times = 4;
      printf("\nFocus time (Minutes): ");
      if (scanf("%d",(int*) &main_time) != 1) {
        fprintf(stderr, "Invalid input!\n");
        return 1;
      }

      printf("\nRest time (Minutes): ");
      if (scanf("%d",(int*) &rest_time) != 1) {
        fprintf(stderr, "Invalid input!\n");
        return 1;
      }

      printf("\nLong rest time (Minutes): ");
      if (scanf("%d",(int*) &long_rest_time) != 1) {
        fprintf(stderr, "Invalid input!\n");
        return 1;
      }

      printf("\nTimes: ");
      if (scanf("%d",(int*) &times) != 1) {
        fprintf(stderr, "Invalid input!\n");
        return 1;
      }

      start_pom(main_time, rest_time, long_rest_time, times, notify_runnable);
      break;
    case 3:
      stop_pom(notify_stop_runnable);
      break;
  }

  return 0;
}

void main() {
    uint8_t option = 0;
    // start_pom(10,5,8,4, notify_runnable);
    // stop_pom(notify_stop_runnable);

    while(true) {
      printf("\033[H\033[2J");
      printf(
          "                                 _                 \n"
          " _ __   ___  _ __ ___   ___   __| | ___  _ __ ___  \n"
          "| '_ \\ / _ \\| '_ ` _ \\ / _ \\ / _` |/ _ \\| '__/ _ \\ \n"
          "| |_) | (_) | | | | | | (_) | (_| | (_) | | | (_) |\n"
          "| .__/ \\___/|_| |_| |_ \\___/ \\__,_|\\___/|_|  \\___/ \n"
          "|_|                                                \n"
          "Type ^C to exit\n"
      );
      if (running){
        printf("Current time: %d minutes, ", current_time);
        printf("Status: %s\n", focused ? "Focus mode" : "Rest mode");
      }
      if (!running){
        printf("1. Start default (25 Focus time, 5 Rest time, 20 Long Rest Time)\n");
        printf("2. Start with a modified timer\n");
      }
      if (running){
        printf("3. Stop timer\n");
      }

      putchar('\n');
      char input[100];
      if (!fgets(input, sizeof(input), stdin)) {
          fprintf(stderr, "Error reading input!\n");
          continue;
      }

      if (sscanf(input, "%d",(int*) &option) != 1) {
          fprintf(stderr, "Type a valid number!\n");
      } else {
          process_action(&option);
      }
    }
}


void notify_runnable(pomtimer_status_t *timer_data){
  char *title;
  char message[60];
  if (!timer_data->focused){
    title = "Rest time";
    sprintf(message, "Rest time of %d minutes - Rest #%d", timer_data->sleep_time, timer_data->local_times);
  }else {
    title = "Focus time!";
    sprintf(message, "Focus time init of %d minutes", timer_data->sleep_time);
  }

  show(title, message); 
}

void notify_stop_runnable(pomtimer_status_t *_){
  show("Pomodoro stopped", "The timer is not longer running");
}
