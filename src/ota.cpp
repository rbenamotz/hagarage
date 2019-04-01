#include "common.h"
#include <ArduinoOTA.h>
#include "user_config.h"

#define OTA_INITIAL_STATE true

bool is_ota_enabled;

void setupOta()
{
  is_ota_enabled = OTA_INITIAL_STATE;
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
    {
      type = "sketch";
    }
    else
    { // U_SPIFFS
      type = "filesystem";
    }
    write_to_log("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    write_to_log("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    write_to_log("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    write_to_log("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
    {
      write_to_log("Auth Failed\n");
    }
    else if (error == OTA_BEGIN_ERROR)
    {
      write_to_log("Begin Failed\n");
    }
    else if (error == OTA_CONNECT_ERROR)
    {
      write_to_log("Connect Failed\n");
    }
    else if (error == OTA_RECEIVE_ERROR)
    {
      write_to_log("Receive Failed\n");
    }
    else if (error == OTA_END_ERROR)
    {
      write_to_log("End Failed\n");
    }
  });
  ArduinoOTA.begin();
}

void loopOta()
{
  if (!is_ota_enabled)
  {
    return;
  }
  ArduinoOTA.handle();
}
