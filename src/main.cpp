#include "main.h"
#include "webserver.h"
#include "common.h"
#include "ota.h"
#include <ESP8266mDNS.h>
#include "mqtt.h"
#include "doors.h"
#include "user_config.h"

#define WIFI_CONNECTION_DELAY_MS 500
#define LOOP_DELAY 100

const char* host_name = "garagecontrol";



void init_wifi() {
  WiFi.hostname(host_name);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    write_to_log("Connection Failed! Rebooting...");
    delay(WIFI_CONNECTION_DELAY_MS);
    ESP.restart();
  }
  write_to_log("Connected to WiFi. Local IP is " + WiFi.localIP().toString());
  if (MDNS.begin(host_name)) {
    write_to_log("MDNS started. Host name: %s",host_name);
  }
}

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
  loop_ota();
  loop_doors();
  loop_mqtt();
  loop_web_server();
  delay(LOOP_DELAY);
}
