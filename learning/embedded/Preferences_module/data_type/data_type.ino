#include <Preferences.h>

// Create a Preferences object
Preferences preferences;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Open preferences in read-only mode
  if (preferences.begin("Passwords", true)) { // 'true' means read-only mode
    // Loop through keys "line0" to "line6"
    for (int i = 0; i <= 6; i++) {
      String key = "line" + String(i); 
      
      // Get the type of the stored data
      uint8_t type = preferences.getType(key.c_str());
      
      // Print the key and its type number
      Serial.print("Key: " + key + " - Type: ");
      Serial.println(type);
    }
  } else {
    Serial.println("Failed to open preferences.");
  }

  
  preferences.end();
}

void loop() {
  // :)
}
