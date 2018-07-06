#include <common.h>
#include <stdarg.h>
#include <stdio.h>
#include "user_config.h"


String log_buffer[LOG_SIZE];
unsigned long log_buffer_ms[LOG_SIZE];
int log_buffer_index = -1;

void setupCommon() {
  if (LOG_TO_SERIAL) {
    Serial.begin(115200);
  }
}

void write_to_log(String line, ...) {
  char temp[300];
  const char* l = line.c_str();
  log_buffer_index++;
  int realIndex = log_buffer_index % LOG_SIZE;
  va_list args;
  va_start(args, l);
  vsprintf(temp,l ,args);
  log_buffer[realIndex] = temp;
  log_buffer_ms[realIndex] = millis();
  va_end(args);
  if (LOG_TO_SERIAL) {
    Serial.println(temp);
  }
}

String read_log_buffer() {
  String output;
  if (log_buffer_index<=0) {
    return "";
  }
  unsigned long now = millis();
  for (int i=LOG_SIZE; i>0; i--) {
    int p = i + log_buffer_index;
    int l = p % LOG_SIZE;
    output+= (now - log_buffer_ms[l]) / 1000;
    output+= " - ";
    output+=log_buffer[l];
    output+="\n";
  }
  return output;

}
