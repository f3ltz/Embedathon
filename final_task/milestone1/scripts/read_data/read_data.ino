#include <Preferences.h>


Preferences preferences;

void setup() {
  
  Serial.begin(115200);

 
  if (preferences.begin("Passwords", true)) { // 'true' is for read only mode
   
    for (int i = 0; i <= 6; i++) {
      String key = "line" + String(i); 
      String storedValue = preferences.getString(key.c_str(), "default_value"); 
      Serial.println("Stored value for " + key + ": " + storedValue);
    }
  } else {
    Serial.println("Failed to open preferences.");
  }

  preferences.end();
}

void loop() {
  // no need for loop:)
}
