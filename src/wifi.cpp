#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "user_config.h"
#include "common.h"
#include <WiFiManager.h>

static void onStationModeConnected(const WiFiEventStationModeConnected &event)
{
  write_to_log("WiFi Connected. SSID=%s", event.ssid.c_str());
}

static void onStationModeDisconnected(const WiFiEventStationModeDisconnected &event)
{
  write_to_log("WiFi Disconnected. Reason code=%d", event.reason);
}

static void onStationModeGotIP(const WiFiEventStationModeGotIP &event)
{
  write_to_log("WiFi Got IP. localIP=%s, hostname=%s", event.ip.toString().c_str(), WiFi.hostname().c_str());
  MDNS.begin(HOST_NAME);
}

static void onStationModeDHCPTimeout()
{
  write_to_log("WiFi DHCP timed out.");
}

void setupWifiWithManager()
{
}

void setupWifi()
{
  write_to_log("Entering WiFi setup");
  static WiFiEventHandler e1 = WiFi.onStationModeConnected(onStationModeConnected);
  static WiFiEventHandler e2 = WiFi.onStationModeDisconnected(onStationModeDisconnected);
  static WiFiEventHandler e3 = WiFi.onStationModeGotIP(onStationModeGotIP);
  static WiFiEventHandler e4 = WiFi.onStationModeDHCPTimeout(onStationModeDHCPTimeout);
  write_to_log("Starting WiFi Manager");
  WiFiManager wifiManager;
  // wifiManager.resetSettings();
  wifiManager.setDebugOutput(false);
  wifiManager.autoConnect(HOST_NAME);
}
void loopWifi()
{
}
