#include "webserver.h"
#include "common.h"
#include "ota.h"
#include "mqtt.h"
#include "doors.h"
#include "user_config.h"
#include "wifi.h"


void setup()
{
  setupCommon();
  setupDoors();
  setupWifi();
  setupOta();
  setupMqtt();
  setupWebServer();
}


void loop()
{
  loopWifi();
  loopOta();
  loopDoors();
  loopMqtt();
  loopWebServer();
}
