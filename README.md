
# TelemetryHarborSDK for C++ (Arduino)

<!-- Arduino -->
![Arduino Library](https://img.shields.io/badge/Arduino-Library-00979D.svg)
![Platform](https://img.shields.io/badge/platform-ESP32-orange.svg)
![License](https://img.shields.io/github/license/TelemetryHarbor/harbor-sdk-c-plus-plus.svg)

<!-- GitHub -->
![Last Commit](https://img.shields.io/github/last-commit/TelemetryHarbor/harbor-sdk-c-plus-plus.svg)
![Issues](https://img.shields.io/github/issues/TelemetryHarbor/harbor-sdk-c-plus-plus.svg)
![Pull Requests](https://img.shields.io/github/issues-pr/TelemetryHarbor/harbor-sdk-c-plus-plus.svg)
![Repo Size](https://img.shields.io/github/repo-size/TelemetryHarbor/harbor-sdk-c-plus-plus.svg)
![Contributors](https://img.shields.io/github/contributors/TelemetryHarbor/harbor-sdk-c-plus-plus.svg)

<!-- Fun / Community -->
![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)
![Stars](https://img.shields.io/github/stars/TelemetryHarbor/harbor-sdk-c-plus-plus.svg?style=social)
![Forks](https://img.shields.io/github/forks/TelemetryHarbor/harbor-sdk-c-plus-plus.svg?style=social)


A C++ client SDK for sending telemetry data to the **Telemetry Harbor** service, designed for Arduino-based microcontrollers like the ESP32 and ESP8266.

This library simplifies sending sensor data by handling HTTP communication, JSON serialization, and robust error handling with automatic retries.

For full details and advanced usage, please see our official documentation at [docs.telemetryharbor.com](https://docs.telemetryharbor.com).

***

## Features

* ✅ **Simple Interface**: Easily send data with `send()` and `sendBatch()` methods.
* 📦 **JSON Handling**: Automatically serializes data structs into the required JSON format.
* ⚙️ **Robust Retries**: Implements exponential backoff to automatically retry sending data on network or server errors.
* 📡 **ESP32 & ESP8266 Ready**: Built for the most popular Wi-Fi enabled microcontrollers in the Arduino ecosystem.
* 🔌 **Extensible**: Easily add new data structures for different telemetry types.

***

## Installation

1.  **Install from Library Manager**:
    * Open the Arduino IDE.
    * Go to `Sketch` > `Include Library` > `Manage Libraries...`.
    * Search for "**TelemetryHarborSDK**" and click "Install".
    * The IDE will prompt you to also install its dependency, "**ArduinoJson**". Click "Install all".

2.  **Manual Installation**:
    * Download the latest release from the repository as a ZIP file.
    * In the Arduino IDE, go to `Sketch` > `Include Library` > `Add .ZIP Library...` and select the downloaded file.
    * Install the **ArduinoJson** library separately from the Library Manager.

***

## Quickstart Guide

Here is a basic example of how to use the library to send a single sensor reading.

### Example Sketch

```cpp
#include <WiFi.h>
#include <HarborClient.h> // 1. Include the library

// --- WiFi & API Credentials ---
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* harborEndpoint = "YOUR_API_ENDPOINT";
const char* harborApiKey = "YOUR_API_KEY";

// 2. Initialize the client globally
HarborClient harbor(harborEndpoint, harborApiKey);

void setup() {
  Serial.begin(115200);
  delay(100);

  // Connect to WiFi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
}

void loop() {
  // 3. Create a reading struct
  GeneralReading reading;
  reading.ship_id = "esp32-freighter-01";
  reading.cargo_id = "temperature-hold-3";
  reading.value = 25.5 + random(-2, 2); // Simulate a sensor reading

  // 4. Send the reading
  Serial.println("Sending a reading...");
  int statusCode = harbor.send(reading);

  if (statusCode >= 200 && statusCode < 300) {
    Serial.printf("Successfully sent data (Status: %d)\n", statusCode);
  } else {
    Serial.printf("Failed to send data (Status: %d)\n", statusCode);
  }

  // Wait 30 seconds before sending the next reading
  delay(30000);
}
````

-----

## API Reference

### `TelemetryHarborClient(const char* endpoint, const char* api_key)`

The constructor for the client.

  * `endpoint`: The URL of your Telemetry Harbor ingestion endpoint.
  * `api_key`: Your unique API key for authentication.

### `int send(const GeneralReading& reading)`

Sends a single telemetry reading.

  * `reading`: A `GeneralReading` struct containing the data.
  * **Returns**: The final HTTP status code after all retries.

### `int sendBatch(const GeneralReading readings[], int count)`

Sends an array of readings in a single HTTP request.

  * `readings`: A C-style array of `GeneralReading` structs.
  * `count`: The number of elements in the array.
  * **Returns**: The final HTTP status code.

### `GeneralReading` Struct

The primary data structure for telemetry.

```cpp
struct GeneralReading {
    String ship_id;
    String cargo_id;
    float value;
    String time; // Optional, ISO8601 format (e.g., "2025-07-17T10:08:55Z")
};
```


