#include <Preferences.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

Preferences preferences;
WebSocketsServer webSocket(81); // WebSocket server on port 81

const char* ssid = "Rudra";       // Wi-Fi SSID
const char* password = "rudra2903"; // Wi-Fi password

String combinedMessage = ""; // Define combinedMessage as a global variable

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  if (preferences.begin("Passwords", true)) {
    combinedMessage = ""; // Clear the message before combining
    for (int i = 0; i <= 6; i++) {
      String key = "line" + String(i);
      String storedValue = preferences.getString(key.c_str(), "default_value");

      // Extract only the portion after the '=' sign
      int equalsIndex = storedValue.indexOf('=');
      if (equalsIndex != -1) {
        storedValue = storedValue.substring(equalsIndex + 1); // Remove part before '='
      }

      combinedMessage += storedValue + "\n"; // Combine the messages
      Serial.printf("Processed key: %s, value: %s\n", key.c_str(), storedValue.c_str());
    }
    preferences.end();
  } else {
    Serial.println("Failed to open preferences.");
  }
}


void loop() {
  webSocket.loop();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("Client %u disconnected.\n", num);
      break;
    case WStype_CONNECTED:
      Serial.printf("Client %u connected from %s\n", num, webSocket.remoteIP(num).toString().c_str());
      webSocket.broadcastTXT(combinedMessage); // Use the global variable
      Serial.println("Broadcasted message: " + combinedMessage);
      break;
    case WStype_TEXT:
      Serial.printf("Received text: %s\n", payload);
      break;
    default:
      break;
  }
}
