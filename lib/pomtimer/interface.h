#ifndef POM_TIMER 
#define POM_TIMER
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t local_times;
    uint8_t sleep_time;
    bool focused;
} pomtimer_status_t;

typedef void (*pomtimer_runnable)(pomtimer_status_t*);

// start(main_time, rest_time, long_rest_time, times_to_repeat_loop)
uint8_t  start_pom  (uint8_t, uint8_t, uint8_t, uint8_t, pomtimer_runnable);
void stop_pom   (pomtimer_runnable);
uint16_t  minconvert (uint8_t);
extern uint8_t current_time;
extern bool focused;
extern volatile bool running;

#endif  /* POM_TIMER */
