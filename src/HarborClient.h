// TelemetryHarborSDK/src/HarborClient.h

#ifndef HarborClient_h
#define HarborClient_h

#include "Arduino.h"
#include <ArduinoJson.h> // For JSON functionality

#if defined(ESP8266)
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#elif defined(ESP32)
#include <HTTPClient.h>
#include <WiFi.h>
#else
#error "This library currently only supports ESP8266 and ESP32."
#endif

// Equivalent to your Pydantic 'GeneralReading' model
struct GeneralReading
{
    String ship_id;
    String cargo_id;
    float value;
    String time; // Should be in ISO8601 format, e.g., "2025-07-17T09:21:47Z"
};

class HarborClient
{
public:
    // Constructor
    HarborClient(const char *endpoint, const char *api_key);

    // Methods
    int send(const GeneralReading &reading);
    int sendBatch(const GeneralReading readings[], int count);

private:
    String _endpoint;
    String _api_key;
    int _max_retries = 5;
    float _initial_backoff = 1000; // In milliseconds

    // Private helper for sending requests
    int _sendRequest(String &url, String &jsonPayload);
};

#endif
