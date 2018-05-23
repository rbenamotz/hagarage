#include "common.h"
#include "doors.h"
#include "mqtt.h"
#include "user_config.h"

#define FORCE_UPDATE_MS 15000
#define GARAGE_BUTTON_DELAY_MS 100

static const int reed_switch_pins[] = REED_SWITCH_PINS;
static const int push_button_pins[] = BUTTON_PINS;

unsigned long last_time_state_reported[TOTAL_DOORS];
bool is_door_open[TOTAL_DOORS];


void toggle_door_state(int door) {
  int p = push_button_pins[door];
  write_to_log("Toggling door %d on pin %d", door + 1, p);
  if (p==-1) {
    return;
  }
  digitalWrite(p,LOW);
  delay(GARAGE_BUTTON_DELAY_MS);
  digitalWrite(p,HIGH);
}


void init_door(int door) {
  last_time_state_reported[door] = 0;
  is_door_open[door] = false;
  write_to_log("Setting up door %d (switch %d, relay %d)" , door+1 ,reed_switch_pins[door], push_button_pins[door]);
  pinMode(reed_switch_pins[door], INPUT_PULLUP);
  if (push_button_pins[door] != -1) {
    pinMode(push_button_pins[door], OUTPUT);
    digitalWrite(push_button_pins[door],HIGH);
  }
}

void init_doors() {
  for (int i=0; i<TOTAL_DOORS; i++) {
    init_door(i);
  }
}


void loop_door(int door) {
  unsigned long timeSinceLastUpdate = millis() - last_time_state_reported[door];
  bool forceReport = timeSinceLastUpdate > FORCE_UPDATE_MS;

  int p = digitalRead(reed_switch_pins[door]);
  bool previousState = is_door_open[door];
  is_door_open[door] = (p!=0);
  if (is_door_open[door]==previousState && !forceReport) {
    return;
  }
  if (is_door_open[door]!=previousState) {
    write_to_log("Door state changed to %d for door %d (p=%d)",is_door_open[door],door+1,p);
  }
  publish_state(door,is_door_open[door]);
  last_time_state_reported[door] = millis();
}

void loop_doors() {
  for (int i=0; i<TOTAL_DOORS; i++) {
    loop_door(i);
  }
}
