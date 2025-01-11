#include <Preferences.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

int rows[] = {21, 22, 23, 25, 26, 27, 32, 33}; // Row pins
int cols[] = {4, 5, 13, 14, 16, 17, 18, 19};   // Column pins

#define MAX_TEXT_LENGTH 20
int matrices[MAX_TEXT_LENGTH][8][8];
int matricelist[8][160];
int received_integer = 0;

Preferences preferences;
WebSocketsServer webSocket(81); // WebSocket server on port 81

const char* ssid = "Rudra";       // Wi-Fi SSID
const char* password = "rudra2903"; // Wi-Fi password

String combinedMessage = ""; // Define combinedMessage as a global variable

void reshapeMatrix(int input[20][8][8], int output[8][160]) {
  // Initialize the output matrix with zeros
  memset(output, 0, sizeof(int) * 8 * 160);

  // Loop through each 8x8 matrix in the 3D input array
  for (int block = 0; block < 20; block++) {
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        // Map the (block, row, col) position in input to the (row, col) in output
        output[row][block * 8 + col] = input[block][row][col];
      }
    }
  }
}

void displayPattern(int matrix[8][8], int duration) {
  unsigned long startTime = millis();
  while (millis() - startTime < duration) {
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        if (matrix[row][col] == 1) {
          lightLED(row, col); // Light up the LED for a '1'
        }
      }
    }
  }
}

// Function to light up a single LED
void lightLED(int rowIdx, int colIdx) {
  // Turn off all rows and columns first
  for (int i = 0; i < 8; i++) {
    digitalWrite(rows[i], LOW);  // Turn all rows LOW
    digitalWrite(cols[i], HIGH); // Turn all columns HIGH
  }

  // Activate the desired row and column
  digitalWrite(rows[rowIdx], HIGH); // Set the specific row HIGH
  digitalWrite(cols[colIdx], LOW);  // Set the specific column LOW
  delay(1); // Small delay for stability
}
// Function to scroll the text across the LED matrix
void scrollText(int textArray[8][160], int scrollSpeed) {
  const int width = 160;      // Total number of columns in the input array
  const int height = 8;       // Number of rows in the input array
  const int displayWidth = 8; // Width of the scrolling display (8x8 matrix)

  for (int shift = 0; shift <= width - displayWidth; shift++) {
    int scrollMatrix[8][8] = {0}; // Temporary matrix to display the current window

    // Extract an 8x8 window from the textArray at the current shift position
    bool isEmpty = true; // Flag to check if the display is empty
    for (int row = 0; row < height; row++) {
      for (int col = 0; col < displayWidth; col++) {
        scrollMatrix[row][col] = textArray[row][col + shift];
        if (scrollMatrix[row][col] != 0) {
          isEmpty = false; // Mark as non-empty if any element is non-zero
        }
      }
    }

    // Break the loop if the display is empty
    if (isEmpty) {
      break;
    }

    // Display the current scroll matrix
    displayPattern(scrollMatrix, scrollSpeed);
  }
}


void setup() {
  // Set all row pins as OUTPUT
  for (int i = 0; i < 8; i++) {
    pinMode(rows[i], OUTPUT);
    digitalWrite(rows[i], LOW); // Turn rows off initially
  }

  // Set all column pins as OUTPUT
  for (int i = 0; i < 8; i++) {
    pinMode(cols[i], OUTPUT);
    digitalWrite(cols[i], HIGH); // Turn columns off initially
  }
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
  scrollText(matricelist, 200);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("Client %u disconnected.\n", num);
      break;

    case WStype_CONNECTED:
      Serial.printf("Client %u connected from %s\n", num, webSocket.remoteIP(num).toString().c_str());
      webSocket.broadcastTXT(combinedMessage); // Use the global variable
      Serial.println("Broadcasted message: " + combinedMessage);
      break;

    case WStype_TEXT: {
      Serial.printf("Received text: %s\n", payload);

      // Deserialize the received JSON message
      DynamicJsonDocument doc(2048); // Adjusted buffer size
      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.println("Failed to parse JSON!");
        return;
      }

      // Extract matrices from the JSON object
      if (doc.containsKey("matrices")) {
        JsonArray matricesJson = doc["matrices"].as<JsonArray>();
        int matrixIdx = 0;

        // Loop through the received matrices and store in the 3D array
        for (JsonArray matrixJson : matricesJson) {
          int rowIdx = 0;
          for (JsonArray rowJson : matrixJson) {
            int colIdx = 0;
            for (int val : rowJson) {
              matrices[matrixIdx][rowIdx][colIdx] = val; // Store the value in the matrix
              colIdx++;
            }
            rowIdx++;
          }
          matrixIdx++;
        }
      }

      reshapeMatrix(matrices,matricelist);

      // Extract the integer value from the JSON object
      if (doc.containsKey("integer_value")) {
        received_integer = doc["integer_value"].as<int>();
        Serial.printf("Received integer: %d\n", received_integer);
      } else {
        Serial.println("No integer value found in JSON.");
      }
      break;
    }

    default:
    break;
  }
}
