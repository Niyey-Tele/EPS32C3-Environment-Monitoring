#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <SSD1306Wire.h>

#include "oled_ui.h"
#include "web.h"

// ========== SET YOUR WIFI HERE ==========
// Must use phone hotspot on 2.4GHz, NOT 5GHz
const char* WIFI_SSID = "YOUR_PHONE_2.4G_HOTSPOT_NAME";
const char* WIFI_PASS = "YOUR_HOTSPOT_PASSWORD";

// DHT11 sensor pin setup
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// LED pins
#define RED_LED_PIN     7    // Red = bad, humidity too high
#define YELLOW_LED_PIN  8    // Yellow = okay, normal humidity
#define BLUE_LED_PIN    9    // Blue = good, nice humidity

SSD1306Wire display(0x3c, 0, 1);  // OLED screen i2c address and pins
WebServer server(80);             // Web server for browser page

float temp = 0.0;                 // Store temperature value
float hum = 0.0;                  // Store humidity value
bool wifiConnected = false;       // Track if wifi connects or not

unsigned long lastReadTime = 0;
const unsigned long READ_INTERVAL = 5000; // Update sensor / OLED / LED every 5 seconds

// This function runs when someone open the web page
void handleRoot()
{
  server.send(200, "text/html", getWebPage());
}

// Try connect to wifi, stop trying after 10 seconds if can't connect
void connectWiFi()
{
  Serial.print("Connecting WiFi: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  unsigned long startTimeout = millis();
  const unsigned long timeoutMs = 10000; // Max wait time for wifi = 10 seconds

  // Keep trying until wifi connect OR time out
  while(WiFi.status() != WL_CONNECTED && (millis() - startTimeout) < timeoutMs){
    delay(500);
    Serial.print(".");
  }

  if(WiFi.status() == WL_CONNECTED){
    wifiConnected = true;
    Serial.println("\nWiFi Connected!");
    Serial.print("ESP32 IP: ");
    Serial.println(WiFi.localIP());
  }else{
    wifiConnected = false;
    Serial.println("\nWiFi FAILED, continue offline");
  }
}

// Turn on the right LED based on humidity reading
void updateHumidityLED(float humVal)
{
  // First turn all 3 LEDs off
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);

  // If sensor reading error, leave all LEDs off
  if (isnan(humVal))
  {
    return;
  }

  if (humVal > 70)
  {
    digitalWrite(RED_LED_PIN, HIGH);    // RED ON: too wet, bad
  }
  else if (humVal >= 40 && humVal <=70)
  {
    digitalWrite(YELLOW_LED_PIN, HIGH); // YELLOW ON: okay range
  }
  else
  {
    digitalWrite(BLUE_LED_PIN, HIGH);   // BLUE ON: good, dry enough
  }
}

// Get new temp and humidity from DHT11 sensor
void readSensorData()
{
  hum = dht.readHumidity();
  temp = dht.readTemperature();
}

// Runs ONE TIME when ESP32 powers on or reset
void setup()
{
  Serial.begin(115200);

  // Set LED pins as output (so we can turn LEDs on/off)
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  // Start with all LEDs OFF
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);

  dht.begin();        // Start DHT11 sensor
  oledInit();         // Start OLED screen, show "Starting..."

  connectWiFi();      // Run wifi connect function

  server.on("/", handleRoot); // Link web page request to our function
  server.begin();             // Start web server
  Serial.println("Web server ready");
}

// This loop runs forever while ESP32 power on
void loop()
{
  server.handleClient(); // Always keep web server working, don't block it

  unsigned long currentTime = millis();
  // Check if 5 seconds have passed
  if (currentTime - lastReadTime >= READ_INTERVAL)
  {
    lastReadTime = currentTime; // Reset timer
    readSensorData();           // Read new sensor data
    oledUpdateUI(temp, hum, wifiConnected); // Refresh OLED screen
    updateHumidityLED(hum);     // Update LED lights
  }
}
