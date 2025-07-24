// TelemetryHarborSDK/examples/SendSingleReading/SendSingleReading.ino

#include <WiFi.h>
#include "HarborClient.h"

const char* ssid = "Harbor-WIFI";
const char* password = "";

// Replace with your actual endpoint and API key
const char* harborEndpoint = "ENDPOINT";
const char* harborApiKey = "API_KEY";

HarborClient harbor(harborEndpoint, harborApiKey);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
}

void loop() {
  GeneralReading reading;
  reading.ship_id = "esp32-freighter-01";
  reading.cargo_id = "temperature-hold-3";
  reading.value = 25.5 + random(-2, 2); // Simulated sensor reading
  reading.time = "2025-06-24T19:24:00.948Z";

  Serial.println("Sending a reading...");
  int statusCode = harbor.send(reading);

  if (statusCode >= 200 && statusCode < 300) {
    Serial.printf("✅ Successfully sent data (Status: %d)\n", statusCode);
  } else {
    Serial.printf("❌ Failed to send data (Status: %d)\n", statusCode);
  }

  delay(30000); // Wait 30 seconds
}
