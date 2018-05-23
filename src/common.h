#include <Arduino.h>
#include "user_config.h"

extern const char* host_name;
extern bool is_door_open[TOTAL_DOORS];

void init_common();
void write_to_log(String line, ...);
String read_log_buffer();
