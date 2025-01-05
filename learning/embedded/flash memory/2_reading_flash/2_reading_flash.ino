#include <nvs_flash.h>
#include <nvs.h>

// Function to read data (integer) from NVS
int32_t readData(const char* key) {
    nvs_handle_t nvs_handle;
    int32_t value = 0; // Default value if key is not found
    esp_err_t err = nvs_open("storage", NVS_READONLY, &nvs_handle); // Open NVS in read-only mode

    if (err == ESP_OK) {
        // Read the integer value
        err = nvs_get_i32(nvs_handle, key, &value);
        if (err == ESP_OK) {
            Serial.print("Data read successfully for key: ");
            Serial.print(key);
            Serial.print(" -> Value: ");
            Serial.println(value);
        } else if (err == ESP_ERR_NVS_NOT_FOUND) {
            Serial.print("Key not found: ");
            Serial.println(key);
        } else {
            Serial.println("Error reading data.");
        }
        nvs_close(nvs_handle); // Close NVS
    } else {
        Serial.println("Failed to open NVS.");
    }
    return value;
}

void setup() {
    Serial.begin(115200);

    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        Serial.println("Failed to initialize NVS.");
        return; // Stop if NVS initialization fails
    }
    Serial.println("NVS initialized.");

    // Read the data from NVS
    int32_t counterValue = readData("counter");

    // Display the value
    Serial.print("Counter Value Retrieved: ");
    Serial.println(counterValue);
}

void loop() {
    // Nothing to do in the loop for this example
}
