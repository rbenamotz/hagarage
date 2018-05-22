#include <Arduino.h>

extern bool is_ota_enabled;
extern const char* host_name;
extern bool isGarageDoorsOpen[2];

void init_common();
void write_to_log(String line, ...);
String read_log_buffer();
