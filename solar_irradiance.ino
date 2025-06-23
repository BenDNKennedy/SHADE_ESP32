// File: solar_simulator.ino

#include <WiFi.h>
#include <WebServer.h>

// WiFi config
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PW";

IPAddress local_IP(192, 168, 1, 87);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);

// Simulate solar index between 0.0 – 1.0
float simulateSolarIndex() {
  return random(0, 100) / 100.0;
}

void setup() {
  Serial.begin(115200);
  delay(1000); // Ensure serial begins cleanly

  WiFi.config(local_IP, gateway, subnet);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ Failed to connect.");
  }

  server.on("/solar", HTTP_GET, []() {
    float index = simulateSolarIndex();

    String json = "{ \"solar_index\": " + String(index, 2) + " }";
    server.send(200, "application/json", json);
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
