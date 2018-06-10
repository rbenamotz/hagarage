#include "webserver.h"
#include "common.h"
#include "ota.h"
#include "mqtt.h"
#include "doors.h"
#include "user_config.h"
#include "wifi.h"


void setup()
{
  init_common();
  init_doors();
  init_wifi();
  init_ota();
  init_mqtt();
  init_web_server();
}


void loop()
{
  reconect_to_wifi();
  loop_ota();
  loop_doors();
  loop_mqtt();
  loop_web_server();
}
