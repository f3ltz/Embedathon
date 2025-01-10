#include <Preferences.h>

Preferences preferences;

void setup() {
  Serial.begin(115200);
  delay(1000);
  preferences.begin("IEEE_NITK", false);
  preferences.putString("line0", "Embedathon");
  Serial.println("String 'Embedathon' successfully written into NVM!");
  String value = preferences.getString("line0", "default");
  Serial.println("Stored value in NVM: " + value);
  preferences.end();
}

void loop() {
}
