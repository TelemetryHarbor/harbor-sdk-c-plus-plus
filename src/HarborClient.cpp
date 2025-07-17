// TelemetryHarborSDK/src/HarborClient.cpp

#include "HarborClient.h"

HarborClient::HarborClient(const char *endpoint, const char *api_key)
{
    _endpoint = endpoint;
    _api_key = api_key;
}

int HarborClient::send(const GeneralReading &reading)
{
    // 1. Create JSON object
    JsonDocument doc; // ArduinoJson v7+
    doc["ship_id"] = reading.ship_id;
    doc["cargo_id"] = reading.cargo_id;
    doc["value"] = reading.value;
    // Set timestamp if provided, otherwise the server can set it
    if (reading.time.length() > 0)
    {
        doc["time"] = reading.time;
    }

    // 2. Serialize JSON to string
    String jsonPayload;
    serializeJson(doc, jsonPayload);

    // 3. Send request
    return _sendRequest(_endpoint, jsonPayload);
}

int HarborClient::sendBatch(const GeneralReading readings[], int count)
{
    if (count == 0)
    {
        return -1; // Indicate empty batch
    }

    // 1. Create JSON array
    JsonDocument doc;
    JsonArray batchArray = doc.to<JsonArray>();

    for (int i = 0; i < count; i++)
    {
        JsonObject readingObj = batchArray.add<JsonObject>();
        readingObj["ship_id"] = readings[i].ship_id;
        readingObj["cargo_id"] = readings[i].cargo_id;
        readingObj["value"] = readings[i].value;
        if (readings[i].time.length() > 0)
        {
            readingObj["time"] = readings[i].time;
        }
    }

    // 2. Serialize JSON to string
    String jsonPayload;
    serializeJson(doc, jsonPayload);

    // 3. Send request to the batch endpoint
    String batchUrl = _endpoint + "/batch";
    return _sendRequest(batchUrl, jsonPayload);
}

int HarborClient::_sendRequest(String &url, String &jsonPayload)
{
    int httpCode = 0;
    int retries = 0;

    while (retries <= _max_retries)
    {
        WiFiClient client; // Use a new client for each attempt
        HTTPClient http;
        http.begin(client, url);
        http.addHeader("Content-Type", "application/json");
        http.addHeader("X-API-Key", _api_key);

        Serial.printf("[HTTP] Attempt %d to POST to %s\n", retries + 1, url.c_str());
        httpCode = http.POST(jsonPayload);

        if (httpCode > 0)
        { // Check for a server response
            String response = http.getString();
            Serial.printf("[HTTP] Response code: %d\n", httpCode);
            Serial.printf("[HTTP] Response body: %s\n", response.c_str());

            // Success condition
            if (httpCode >= 200 && httpCode < 300)
            {
                http.end();
                return httpCode;
            }
            // Client error condition (don't retry)
            if (httpCode >= 400 && httpCode < 500 && httpCode != 429)
            {
                Serial.println("[HTTP] Client error. Will not retry.");
                http.end();
                return httpCode;
            }
        }
        else
        { // Network or connection error
            Serial.printf("[HTTP] POST failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end(); // IMPORTANT: free resources

        // Retry logic
        retries++;
        if (retries <= _max_retries)
        {
            long sleep_time = _initial_backoff * pow(2, retries - 1);
            Serial.printf("Retrying in %.2f seconds...\n", sleep_time / 1000.0);
            delay(sleep_time);
        }
    }

    Serial.printf("Failed to send data after %d attempts.\n", _max_retries + 1);
    return httpCode; // Return the last error code
}
