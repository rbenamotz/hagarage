#include <common.h>
#include <stdarg.h>
#include <stdio.h>
#include "user_config.h"

String log_buffer[LOG_SIZE];
int log_buffer_index = -1;

void setupCommon() {
  if (LOG_TO_SERIAL) {
    Serial.begin(115200);
  }
}

void write_to_log(String line, ...) {
  char temp[300];
  char temp2[350];
  const char* l = line.c_str();
  log_buffer_index++;
  int realIndex = log_buffer_index % LOG_SIZE;
  va_list args;
  va_start(args, l);
  vsprintf(temp,l ,args);
  sprintf(temp2,"%lu : %s",millis(), temp);
  log_buffer[realIndex] = temp2;
  va_end(args);
  if (LOG_TO_SERIAL) {
    Serial.println(temp2);
  }
}

String read_log_buffer() {
  String output;
  if (log_buffer_index<=0) {
    return "";
  }
  for (int i=LOG_SIZE; i>0; i--) {
    int p = i + log_buffer_index;
    int l = p % LOG_SIZE;
    output+=log_buffer[l];
    output+="\n";
  }
  return output;

}
