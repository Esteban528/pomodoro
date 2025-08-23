#include "interface.h"
#include <libnotify/notify.h>

void show(char* title, char* message) {
  notify_init("PomodoroCTimer");
  NotifyNotification* n = notify_notification_new(title, message, 
                                                  "emblem-important-symbolic"); 	
  notify_notification_show(n, NULL); 	
}
