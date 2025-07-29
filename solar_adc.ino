// File: solar_simulator.ino

#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

const int ADC_PIN = 35;   // ADC1_CH7 on GPIO35
const float VREF = 3.3;   // Reference voltage
const int ADC_MAX = 4095; // 12-bit ADC resolution

int adcRaw = 0;
float voltage = 0.0;
float solarIndex = 0.0;


unsigned long lastRead = 0;
const unsigned long interval = 1000; // 1 second


void setup() {
  Serial.begin(115200);
  delay(1000); // Ensure serial begins cleanly

  WiFi.mode(WIFI_AP);           // ap mode

  // Start the AP with your chosen SSID & password:
  const char* ssid = "ESP-Solar";
  const char* password = "s3cretPW";
  WiFi.softAP(ssid, password);

  // Get esp IP on the AP network...
  IPAddress apIP = WiFi.softAPIP();

  // print ip to serial monitor
  Serial.print("📡 ESP‑AP IP address: ");
  Serial.println(apIP);   // e.g. “192.168.4.1”

  // data package to send to app 
  server.on("/solar", HTTP_GET, []() {
    String json = "{ \"solar_index\": " + String(solarIndex, 3) + " }";

    server.send(200, "application/json", json);
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  adcRaw = analogRead(ADC_PIN);
  voltage = (adcRaw / (float)ADC_MAX) * VREF;
  solarIndex = adcRaw / (float)ADC_MAX;

  unsigned long now = millis();
  if (now - lastRead >= interval) {
    lastRead = now;

    // Serial.println("ADC Raw: " + String(adcRaw) + " | Voltage: " + String(voltage, 3) + " | SolarIndex: " + String(solarIndex, 3));
  }
}