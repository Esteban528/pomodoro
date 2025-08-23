#ifndef POM_TIMER 
#define POM_TIMER
#include <stdbool.h>

typedef struct {
    int local_times;
    int sleep_time;
    bool focused;
} pomtimer_status_t;

typedef void (*pomtimer_runnable)(pomtimer_status_t*);

// start(main_time, rest_time, long_rest_time, times_to_repeat_loop)
int start_pom(int, int, int, int, pomtimer_runnable);
void stop_pom(pomtimer_runnable);
int minconvert(int);
extern int current_time;
extern volatile bool running;

#endif  /* POM_TIMER */
