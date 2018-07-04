#include "common.h"
#include "mqtt.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "doors.h"
#include "user_config.h"

#define MAX_ATTEMPTS_FOR_EACH_RECONNECT 1
#define DELAY_AFTER_FAILED_CONNECTION_MS 500


WiFiClient espClient;
PubSubClient client(espClient);
static const char* mqtt_topic_ins[] = MQTT_TOPICS_INS;
static const char* mqtt_topic_outs[] = MQTT_TOPICS_OUT;

void callback(char* topic, byte* payload, unsigned int length) {
  write_to_log("Message arrived. Topic: %s",topic);
  byte door = -1;
  for (int i=0; i<TOTAL_DOORS; i++) {
    if (strcmp(mqtt_topic_ins[i],topic)==0) {
      door = i;
      break;
    }
  }
  if (door == -1) {
    write_to_log("(no door defined)");
    return;
  }
  toggleDoor(door);
}


void reconnect_to_mqtt() {
  if (client.connected()) {
    return;
  }
  int attempts = 0;
  while (!client.connected()) {
    if (attempts > MAX_ATTEMPTS_FOR_EACH_RECONNECT) {
      write_to_log("Could not connect to MQTT for %d times. Yielding",attempts);
      return;
    }
    attempts++;
    if (!client.connect(HOST_NAME,MQTT_USER,MQTT_PASS)) {
      write_to_log("Failed to connect to MQTT server \"%s\", rc=%d.",MQTT_HOST,client.state());
      delay(DELAY_AFTER_FAILED_CONNECTION_MS);
      continue;
    }
    write_to_log("Connected to MQTT server \"%s\" as %s",MQTT_HOST,MQTT_USER);
    for (int i=0; i<TOTAL_DOORS; i++) {
      client.subscribe(mqtt_topic_ins[i]);
    }
  }
}

void setupMqtt() {
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(callback);
}

void publish_state(int door, bool isOpen) {
  String state = "closed";
  if (isOpen) {
    state = "open";
  }
  client.publish(mqtt_topic_outs[door], state.c_str());
}

void loopMqtt() {
  reconnect_to_mqtt();
  client.loop();
}
