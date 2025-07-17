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

    // --- Example for sending a batch ---

    Serial.println("\nSending batch of readings...");
    GeneralReading batch[2];
    batch[0] = {"esp32-freighter-01", "pressure-hold-1", 101.3, ""};
    batch[1] = {"esp32-freighter-01", "humidity-hold-1", 65.2, ""};
    int batchStatusCode = harbor.sendBatch(batch, 2);
    Serial.printf("Batch send finished with code: %d\n", batchStatusCode);

    // Wait before sending the next reading
    delay(30000); // Wait 30 seconds
}
