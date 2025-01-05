#include <nvs.h>
#include <nvs_flash.h>

void setup() {
    Serial.begin(115200);

    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err != ESP_OK) {
        Serial.println("NVS initialization failed.");
        return;
    }

    // List of possible namespaces to test
    const char* namespaces[] = {"Passwords"};
    nvs_handle_t nvs_handle;

    for (int i = 0; i < sizeof(namespaces) / sizeof(namespaces[0]); i++) {
        Serial.print("Testing namespace: ");
        Serial.println(namespaces[i]);

        // Try to open namespace
        err = nvs_open(namespaces[i], NVS_READONLY, &nvs_handle);
        if (err == ESP_OK) {
            Serial.print("Namespace found: ");
            Serial.println(namespaces[i]);

            // Attempt to read key
            int32_t value = 0; // Default value
            err = nvs_get_i32(nvs_handle, " line0", &value);
            if (err == ESP_OK) {
                Serial.print("Value read: ");
                Serial.println(value);
            } else {
                Serial.println("Key not found or other error.");
            }
            nvs_close(nvs_handle);
            return;
        }
    }
    Serial.println("No valid namespaces found.");
}

void loop() {
    // No repeated actions
}
