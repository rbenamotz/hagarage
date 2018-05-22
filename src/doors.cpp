#include "common.h"
#include "doors.h"
#include "mqtt.h"
#include "user_config.h"

unsigned long last_time_state_reported = 0;
static const int reed_switch_pins[] = REED_SWITCH_PINS;
static const int push_button_pins[] = BUTTON_PINS;


void toggle_door_state(int door) {
  int p = push_button_pins[door];
  write_to_log("Toggling door %d on pin %d", door + 1, p);
  if (p==-1) {
    return;
  }
  digitalWrite(p,LOW);
  delay(300);
  digitalWrite(p,HIGH);
}

void init_doors() {
  for (int i=0; i<TOTAL_DOORS; i++) {
    write_to_log("Setting up door %d (switch %d, relay %d)" , i+1 ,reed_switch_pins[i], push_button_pins[i]);
    pinMode(reed_switch_pins[i], INPUT_PULLUP);
    if (push_button_pins[i] != -1) {
      pinMode(push_button_pins[i], OUTPUT);
      digitalWrite(push_button_pins[i],HIGH);
    }
  }
}


void loop_doors() {
  unsigned long timeSinceLastUpdate = millis() - last_time_state_reported;
  bool must_report = timeSinceLastUpdate > 15000;
  for (int i=0; i<TOTAL_DOORS; i++) {
    int p = digitalRead(reed_switch_pins[i]);
    bool b = isGarageDoorsOpen[i];
    isGarageDoorsOpen[i] = (p!=0);
    if (isGarageDoorsOpen[i]==b && !must_report) {
      continue;
    }
    if (isGarageDoorsOpen[i]!=b) {
      write_to_log("Door state changed to %d for door %d (p=%d)",isGarageDoorsOpen[i],i+1,p);
    }
    publish_state(i,isGarageDoorsOpen[i]);
    last_time_state_reported = millis();
  }
}
