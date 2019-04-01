#ifndef _COMMON_H_
#define _COMMOMN_H_

#include <Arduino.h>
#include "user_config.h"

extern const char* host_name;
extern bool is_door_open[TOTAL_DOORS];
extern const String door_names[];


void setupCommon();
void write_to_log(String line, ...);
String read_log_buffer();
#endif 