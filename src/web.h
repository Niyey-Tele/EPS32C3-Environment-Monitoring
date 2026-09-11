#ifndef WEB_PAGE_H
#define WEB_PAGE_H

#include <Arduino.h>

extern float temp;
extern float hum;

String getWebPage()
{
  String html = "<!DOCTYPE html>";
  html += "<html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Environment Monitor</title>";
  html += "<style>";
  html += "*{box-sizing:border-box;font-family:system-ui, sans-serif;}";
  html += "body{background:#f0f4f8;text-align:center;margin:0;padding:40px 20px;}";
  html += ".card{background:#fff;max-width:400px;margin:auto;padding:30px;border-radius:16px;box-shadow:0 2px 12px #00000018;}";
  html += ".data{font-size:24px;margin:12px 0;color:#222;}";
  html += ".label{color:#666;}";
  html += "</style></head><body>";
  html += "<div class='card'>";
  html += "<h2>ESP32 Environment Monitor</h2>";
  html += "<p class='label'>Temperature</p>";
  html += "<p class='data'>" + String(temp,1) + " °C</p>";
  html += "<p class='label'>Humidity</p>";
  html += "<p class='data'>" + String(hum,1) + " %RH</p>";
  html += "</div></body></html>";
  return html;
}

#endif
