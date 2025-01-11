## Milestone 1: How we got the values from esp32's NVS

### Step 1. EXTRACTING NVS DATA USING HEX EDITOR
1. First install [esptool.py](https://docs.espressif.com/projects/esptool/en/latest/esp32/) 
```python 
pip install esptool
```
2. Get the whole firmware that was loaded onto the esp32-id ;)
```bash
python -m esptool --port COM6 read_flash 0x0 0x400000 firmware_backup.bin
```
```bash
python -m esptool --port COM6 write_flash 0x0 firmware_backup.bin
```

3. Reading the firmware:
Using a hex editor like [hxd](https://mh-nexus.de/en/hxd/), we could view the whole firmware and things written to it.

Here is what we found:

![hex editor image output](/final_task/photos/hex_editor_output.png)

### Step 2. Converting the bin data to a usable format
#### Method 1: USING ESP32 IDF:
1. Setup esp32 idf using [setup](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html#installation)
2. Create your project and replace main.c with <b>[this](final_task\milestone1\scripts\esp32idf\main.c)</b> file.
3. Run the [commands](final_task/esp32idf/commands.md) 

This is the output we receive:

```bash
Keys in namespace 'Passwords':
Key: line0, Type: 33
Value (string): �?=18141312131254144313133
Key: line1, Type: 33
Value (string): �?=1711131213131415111313121312131
Key: line2, Type: 33
Value (string): �?=16131211141314141312131213121
Key: line3, Type: 33
Value (string): �?=16131225541413124313133
Key: line4, Type: 33
Value (string): �?=1652111413141452111413161
Key: line5, Type: 33
Value (string): �?=1613121213131414131212131312131
Key: line6, Type: 33
Value (string): �?=521312131213141413121313343
Done listing keys and values.
```

#### Method 2: Using Preference Library

Using this [code](final_task\milestone1\scripts\read_data\read_data.ino) we could read the value from NVS.

```bash
Stored value for line0: �?=18141312131254144313133
Stored value for line1: �?=1711131213131415111313121312131
Stored value for line2: �?=16131211141314141312131213121
Stored value for line3: �?=16131225541413124313133
Stored value for line4: �?=1652111413141452111413161
Stored value for line5: �?=1613121213131414131212131312131
Stored value for line6: �?=521312131213141413121313343
```

##### Code
```cpp
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
}
```

---