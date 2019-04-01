#include <common.h>
#include <stdarg.h>
#include <stdio.h>
#include "user_config.h"

const String door_names[] = DOOR_NAMES;
String log_buffer[LOG_SIZE];
unsigned long log_buffer_ms[LOG_SIZE];
int log_buffer_index = -1;



void setupCommon()
{
  if (LOG_TO_SERIAL)
  {
    Serial.begin(115200);
  }
}

void write_to_log(String line, ...)
{
  char temp[300];
  const char *l = line.c_str();
  log_buffer_index++;
  if (log_buffer_index == LOG_SIZE)
  {
    log_buffer_index = 0;
  }
  va_list args;
  va_start(args, l);
  vsprintf(temp, l, args);
  log_buffer[log_buffer_index] = temp;
  log_buffer_ms[log_buffer_index] = millis();
  va_end(args);
  if (LOG_TO_SERIAL)
  {
    Serial.println(temp);
  }
}

String read_log_buffer()
{
  String output;
  if (log_buffer_index <= 0)
  {
    return "";
  }
  unsigned long now = millis();
  char buf[300];
  for (int i = LOG_SIZE; i > 0; i--)
  {
    int p = i + log_buffer_index;
    int l = p % LOG_SIZE;
    // output+= (now - log_buffer_ms[l]) / 1000;
    unsigned long elapsed = (now - log_buffer_ms[l]) / 1000;
    sprintf(buf, "%01.0f:%02.0f:%02.0f", floor(elapsed / 3600.0), floor(fmod(elapsed, 3600.0) / 60.0), fmod(elapsed, 60.0));
    output += buf;
    output += " - ";
    output += log_buffer[l];
    output += "\n";
  }
  return output;
}
