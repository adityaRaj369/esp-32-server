#include <SPI.h>
#include <SD.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "edge 40 neo_9314";  // Replace with your WiFi SSID
const char *password = "qwertyuiop";    // Replace with your WiFi password
const char *serverBaseUrl = "https://esp-32-server.onrender.com"; // Your Render URL
const int CS = 5;

bool wifiConnected = false;
bool sdInitialized = false;

void connectWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        Serial.print(".");
        delay(1000);
        attempts++;
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected to WiFi");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        wifiConnected = true;
    } else {
        Serial.println("\nFailed to connect. Restarting in 5s...");
        delay(5000);
        ESP.restart();
    }
}

bool downloadFile(String url, String filename) {
    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode != HTTP_CODE_OK) {
        Serial.printf("Failed to download '%s', HTTP code: %d\n", filename.c_str(), httpCode);
        http.end();
        return false;
    }

    File file = SD.open("/" + filename, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file: " + filename);
        http.end();
        return false;
    }

    WiFiClient *stream = http.getStreamPtr();
    uint8_t buffer[512];
    int bytesRead;
    while ((bytesRead = stream->readBytes(buffer, sizeof(buffer))) > 0) {
        file.write(buffer, bytesRead);
    }
    file.close();
    http.end();
    Serial.println("Downloaded: " + filename);
    return true;
}

String getFileList() {
    HTTPClient http;
    String url = String(serverBaseUrl) + "/files";
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode != HTTP_CODE_OK) {
        Serial.printf("Failed to get file list, HTTP code: %d\n", httpCode);
        http.end();
        return "";
    }

    String fileList = http.getString();
    http.end();
    return fileList;
}

void setup() {
    Serial.begin(115200);
    connectWiFi();

    if (!SD.begin(CS)) {
        Serial.println("SD card initialization failed!");
        return;
    }
    Serial.println("SD card initialized!");
    sdInitialized = true;
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi lost! Reconnecting...");
        wifiConnected = false;
        connectWiFi();
    }

    if (!sdInitialized) {
        if (SD.begin(CS)) {
            Serial.println("SD card initialized!");
            sdInitialized = true;
        } else {
            Serial.println("SD card initialization failed!");
        }
    }

    if (wifiConnected && sdInitialized) {
        String fileList = getFileList();
        if (fileList.length() > 0) {
            // Parse JSON array (simple parsing assuming ["file1", "file2"])
            fileList.replace("[", "");  // Remove opening bracket
            fileList.replace("]", "");  // Remove closing bracket
            fileList.replace("\"", ""); // Remove quotes
            int startIndex = 0;
            while (startIndex < fileList.length()) {
                int endIndex = fileList.indexOf(',', startIndex);
                if (endIndex == -1) endIndex = fileList.length();

                String filename = fileList.substring(startIndex, endIndex);
                filename.trim();
                if (filename.length() > 0 && !SD.exists("/" + filename)) {
                    String url = String(serverBaseUrl) + "/download/" + filename;
                    downloadFile(url, filename);
                }
                startIndex = endIndex + 1;
            }
        }
    }

    delay(30000); // Check every 30 seconds
}
