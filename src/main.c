#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <signal.h>
#include <dbus/dbus.h>

#define MINUTE_SECONDS 60

typedef void (*pom_runnable)();
typedef void (*pom_biconsumer)(void *, void *);
typedef enum {
  RUN,
  REST,
  PAUSE
} pom_status;

pom_status tmp_status;
pom_status status;
fd_set     fds;
uint8_t loops             = 0;
uint8_t max_rest_loops    = 4;
uint8_t run_minutes       = 25;
uint8_t rest_minutes      = 5;
uint8_t long_rest_minutes = 10;

int8_t send_notification(char* , char*);

void reproduce_sound() {
  system("pw-play ./bell.ogg&");
}

int16_t recv_to() {
  struct timeval tv = {0L, 0L};
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
}

void clear_stdin(){
  char buf[64];
  read(STDIN_FILENO, buf, sizeof(buf));
}

void swap_status(){
  if (status != PAUSE) {
    tmp_status = status;
    status = PAUSE;
    return;
  }
  status = tmp_status;
}

void timer(uint16_t secs, pom_biconsumer on_each, pom_runnable on_finish) {
  uint16_t i = 0;
  while(i < secs){
    on_each(&secs, &i);
    if (status == PAUSE){
      continue;
    }
    sleep(1);
    i++;
  }
  on_finish();
}

void update(void *max_p, void *actual_p){
  uint16_t max         = *(uint16_t *) max_p;
  uint16_t secs        = *(uint16_t *) actual_p;
  uint8_t  actual_time = (secs/MINUTE_SECONDS);

  if (status != PAUSE){
    printf("\033[H\033[2J");
    printf("\033[47m|\033[0m\033[1;37;%dm %s #%d\033[0m \033[1;32m\033[0m\n\x1b[0m", 
        status == RUN ? 42 : 41, 
        status == RUN ? "TIME TO FOCUS" : "TIME FOR A BREAK", loops);
    printf("Time: %d/%d minutes\n",actual_time, max/MINUTE_SECONDS, loops);

    printf("\n\x1b[36;4mctrl + c to exit\nenter to pause \n\x1b[0m");
  }
  int ret = recv_to();

  if (ret > 0 && FD_ISSET(STDIN_FILENO, &fds)){
    printf("\n\nPaused. Press enter again to resume\n",ret);
    swap_status();
    clear_stdin();
  }
}

void next_loop(){
  if (status == PAUSE){
    printf("Timer aborted\n");
    exit(EXIT_FAILURE);
  }
  uint16_t secs;
    
  if (status == RUN){
    if (loops >= 4){
      secs = long_rest_minutes*MINUTE_SECONDS;
      loops = 0;
    }
    else 
      secs = rest_minutes*MINUTE_SECONDS;
    status = REST;
  }else {
    secs   = run_minutes*MINUTE_SECONDS;
    status = RUN;
    loops++;
  }
  
  char *title;
  char message[60];
  if (status == REST){
    title = "Rest time";
    sprintf(message, "Rest time of %d minutes - #%d", 
        loops >= 4 ? long_rest_minutes : rest_minutes,
        loops);
  }else {
    title = "Focus time!";
    sprintf(message, "Focus time init of %d minutes", run_minutes);
  }

  send_notification(title, message);
  reproduce_sound();
  timer(secs, update, next_loop);
}

void start(){
  status = RUN;
  send_notification("Focus time started", "Pomodoro timer started");
  timer(run_minutes*MINUTE_SECONDS, update, next_loop);
}

void int_handler(int sig){
  printf("\x1b[%dm", 0);
  exit(0);
}

int8_t main(int8_t argc, char *argv[]) {
  // signal(SIGINT, int_handler);
  // start();
  reproduce_sound();
  return 0;
}

int8_t send_notification(char* title, char* message) {
  DBusConnection *conn;
  DBusMessage *msg;
  DBusMessageIter args, array, dict;
  DBusError err;
  dbus_uint32_t serial = 0;

  dbus_error_init(&err);

  conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
  if (dbus_error_is_set(&err)) {
      dbus_error_free(&err);
  }
  if (!conn) return 1;

  msg = dbus_message_new_method_call(
      "org.freedesktop.Notifications", 
      "/org/freedesktop/Notifications",
      "org.freedesktop.Notifications",
      "Notify"                         
  );
  if (!msg) {
      fprintf(stderr, "Message Null\n");
      return 1;
  }
  dbus_message_iter_init_append(msg, &args);

  const char *app_name = "pom";
  dbus_uint32_t replaces_id = 0;
  const char *app_icon = "";
  const char *summary = title;
  const char *body = message;
  int32_t timeout = 5000;

  dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &app_name);
  dbus_message_iter_append_basic(&args, DBUS_TYPE_UINT32, &replaces_id);
  dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &app_icon);
  dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &summary);
  dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &body);

  dbus_message_iter_open_container(&args, DBUS_TYPE_ARRAY, "s", &array);
  dbus_message_iter_close_container(&args, &array);
  dbus_message_iter_open_container(&args, DBUS_TYPE_ARRAY, "{sv}", &dict);
  dbus_message_iter_close_container(&args, &dict);

  dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &timeout);

  if (!dbus_connection_send(conn, msg, &serial)) {
      fprintf(stderr, "Out Of Memory!\n");
      return 1;
  }

  dbus_connection_flush(conn);
  dbus_message_unref(msg);

  return 0;
}
