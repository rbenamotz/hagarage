#ifndef _USER_CONFIG_H_
#define _USER_CONFIG_H_



//Hardware
#define TOTAL_DOORS 2
#define REED_SWITCH_PINS {4,14}
#define BUTTON_PINS {13, 5}


//WiFi
#define WIFI_SSID "ssid name"
#define  WIFI_PASS "wifi password"


//MQTT
#define MQTT_HOST "mqtt server"
#define MQTT_PORT 1883
#define MQTT_USER "mqtt usernamwe"
#define MQTT_PASS "mqtt password"
#define MQTT_TOPICS_OUT {"home-assistant/cover/gd1/state","home-assistant/cover/gd2/state"}
#define MQTT_TOPICS_INS {"home-assistant/cover/gd1/set","home-assistant/cover/gd2/set"}

//Logging
#define LOG_SIZE 15
#define LOG_TO_SERIAL false

//Personalization
#define GARAGE_NAME "garage name"
#define DOOR_NAMES {"car1" , "car2"}

//other
#define OTA_INITIAL_STATE true


#endif
