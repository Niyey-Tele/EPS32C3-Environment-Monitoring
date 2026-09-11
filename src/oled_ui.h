#ifndef OLED_UI_H
#define OLED_UI_H
#include <SSD1306Wire.h>
extern SSD1306Wire display;

// Screen size: width 128 pixels, height 64 pixels
const int SCREEN_W = 128;
const int SCREEN_H = 64;

// Run once at start: turn OLED on and show Starting text
void oledInit()
{
  display.init();
  display.flipScreenVertically();
  display.clear();
  display.drawString(0, 28, "Starting...");
  display.display();
}

// Helper function to put text in the center of screen
void drawCenteredText(int y, String text)
{
  int textW = display.getStringWidth(text);
  int x = (SCREEN_W - textW) / 2;
  display.drawString(x, y, text);
}
// This function decides which status text and icon to show
String getStatusIcon(float temp, float hum)
{
  if (isnan(temp) || isnan(hum))
  {
    return "SENSOR ERROR";
  }

  if (temp > 30)
  {
    return "TOO HOT";
  }
  else if (temp < 15)
  {
    return "TOO COLD";
  }
  else if (hum > 70)
  {
    return "TOO WET";
  }
  else
  {
    return "NORMAL";
  }
}


// Refresh OLED screen: show temp, humidity and wifi signal status
void oledUpdateUI(float temp, float hum, bool wifiConnected)
{
  display.clear();

  drawCenteredText(4, "HUM & TEMP");

  if (isnan(temp) || isnan(hum))
  {
    drawCenteredText(26, "SENSOR ERROR");
  }
  else
  {
    drawCenteredText(22, "Temp: " + String(temp, 1) + " C");
    drawCenteredText(38, "Hum: " + String(hum, 1) + " %RH");
  }

  // Show status icon and message at bottom left
  String status = getStatusIcon(temp, hum);

  if (status == "TOO HOT")
  {
    display.drawString(4, 52, "HOT!");
  }
  else if (status == "TOO COLD")
  {
    display.drawString(4, 52, "COLD!");
  }
  else if (status == "TOO WET")
  {
    display.drawString(4, 52, "WET!");
  }
  else if (status == "NORMAL")
  {
    display.drawString(4, 52, "NORMAL");
  }
  else
  {
    display.drawString(4, 52, "ERROR");
  }

  // Show wifi status at bottom right
  if (wifiConnected)
  {
    display.drawString(90, 52, "WIFI:OK");
  }
  else
  {
    display.drawString(78, 52, "NO SIG");
  }

  display.display();
}

#endif
