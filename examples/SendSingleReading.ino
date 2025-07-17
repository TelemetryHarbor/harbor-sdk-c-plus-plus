// TelemetryHarborSDK/examples/SendSingleReading/SendSingleReading.ino

#include <WiFi.h>
#include <HarborClient.h> // Include your new library

// --- WiFi & API Credentials ---
const char *ssid = "YOUR_WIFI_SSID";
const char *password = "YOUR_WIFI_PASSWORD";
const char *harborEndpoint = "YOUR_API_ENDPOINT";
const char *harborApiKey = "YOUR_API_KEY";

// --- Initialize the client ---
HarborClient harbor(harborEndpoint, harborApiKey);

void setup()
{
    Serial.begin(115200);
    delay(100);

    // Connect to WiFi
    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected!");
}

void loop()
{
    // 1. Create a reading struct
    GeneralReading reading;
    reading.ship_id = "esp32-freighter-01";
    reading.cargo_id = "temperature-hold-3";
    reading.value = 25.5 + random(-2, 2); // Simulate a sensor reading
    // Timestamp is optional. If you need accurate UTC time, you must use an NTP client.
    // reading.time = "2025-07-17T09:21:47Z";

    // 2. Send the reading
    Serial.println("Sending single reading...");
    int statusCode = harbor.send(reading);

    if (statusCode >= 200 && statusCode < 300)
    {
        Serial.printf("Successfully sent data with status code: %d\n", statusCode);
    }
    else
    {
        Serial.printf("Failed to send data. Final status code: %d\n", statusCode);
    }


    // Wait before sending the next reading
    delay(30000); // Wait 30 seconds
}
