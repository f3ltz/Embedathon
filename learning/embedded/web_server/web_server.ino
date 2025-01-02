#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "Rudra";
const char* password = "rudra2903";

// Create a Wi-Fi server on port 80
WiFiServer server(80);

void setup() {
    // Start the serial communication 
    Serial.begin(115200);

    // Connect to Wi-Fi
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected.");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Start the server
    server.begin();
    Serial.println("Server started");
}

void loop() {
    // Check if a client is connected
    WiFiClient client = server.available();
    if (client) {
        Serial.println("New Client Connected");
        String request = client.readStringUntil('\r');
        Serial.println(request);
        client.flush();

        // Send a response to the client
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/html");
        client.println();
        client.println("<h1>Hello from ESP32 Web Server</h1>");
        client.println();

        // Close the connection
        client.stop();
        Serial.println("Client Disconnected");
    }
}
