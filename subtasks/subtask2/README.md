# CANDY QUEEN - SUBTASK 2

# **Working with Preferences in ESP32 Non-Volatile Memory (NVM)**

## **Table of Contents**
1. [Overview](#overview)
2. [Steps](#steps)
    1. [Including the `Preferences` Library](#including-the-preferences-library)
    2. [Creating a Preferences Object](#creating-a-preferences-object)
    3. [Initializing the Preferences Library](#initializing-the-preferences-library)
    4. [Writing Data to NVM](#writing-data-to-nvm)
    5. [Reading Data from NVM](#reading-data-from-nvm)
    6. [Ending Preferences Access](#ending-preferences-access)
    7. [Serial Output](#serial-output)
3. [Code](#code)
4. [Expected Output](#expected-output)
5. [References](#references)

---

## **Overview**

1. Writing a string to NVM.
2. Reading the stored string back from NVM.
3. Printing the stored and retrieved data via the serial monitor.

---

## **Steps**

### 1. [Including the `Preferences` Library](#including-the-preferences-library)
```cpp
#include <Preferences.h>
```

### 2. [Creating a Preferences Object](#creating-a-preferences-object)
```cpp
Preferences preferences;
```

---

### 3. [Initializing the Preferences Library](#initializing-the-preferences-library)
```cpp
preferences.begin("IEEE_NITK", false);
```
- **Purpose:** Opens a namespace (here, `IEEE_NITK`) to store key-value pairs.
- **Parameters:**
  - `namespace`: A string identifier to group related keys (e.g., `"IEEE_NITK"`).
  - `readOnly`: A boolean that specifies if the namespace is for read-only access. (`false` allows writing).

---

### 4. [Writing Data to NVM](#writing-data-to-nvm)
```cpp
preferences.putString("line0", "Embedathon");
```
- **Purpose:** Stores the string `"Embedathon"` in NVM under the key `"line0"`.
- **Function:** `putString(key, value)` writes a string to NVM.
- **Key-Value Pair:**
  - `key`: A unique identifier for the data (e.g., `"line0"`).
  - `value`: The data to store (e.g., `"Embedathon"`).

---

### 5. [Reading Data from NVM](#reading-data-from-nvm)
```cpp
String value = preferences.getString("line0", "default");
```
- **Purpose:** Retrieves the value associated with the key `"line0"`.
- **Function:** `getString(key, default_value)` reads a string from NVM.
- **Parameters:**
  - `key`: The unique identifier of the data to retrieve (e.g., `"line0"`).
  - `default_value`: A fallback value if the key does not exist (e.g., `"default"`).
- **Output:** If `"line0"` exists, it returns `"Embedathon"`. Otherwise, it returns `"default"`.

---

### 6. [Ending Preferences Access](#ending-preferences-access)
```cpp
preferences.end();
```
- **Purpose:** Closes the preferences to free resources.
- **When to Use:** Always call this function when you're done using preferences.

---

### 7. [Serial Output](#serial-output)
The following lines print information to the serial monitor:
```cpp
Serial.begin(115200);
```
- Starts the serial communication at 115200 baud.

```cpp
Serial.println("String 'Embedathon' successfully written into NVM!");
```
- Confirms that the string was successfully written.

```cpp
Serial.println("Stored value in NVM: " + value);
```
- Displays the retrieved value stored in NVM.

---


## **Code**
```cpp
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

void loop() {}
```

## **Expected Output**

When the code is run, the Serial Monitor will display:
```
String 'Embedathon' successfully written into NVM!
Stored value in NVM: Embedathon
```

---

## **References**
[Preferences Documentation](https://espressif-docs.readthedocs-hosted.com/projects/arduino-esp32/en/latest/api/preferences.html)
```