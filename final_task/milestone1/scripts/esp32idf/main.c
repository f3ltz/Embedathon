#include <stdio.h>
#include <inttypes.h>  
#include "nvs_flash.h"
#include "nvs.h"

void app_main(void) {
    esp_err_t err;

    // Step 1: Initialize NVS
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        printf("NVS partition issue detected. Reinitialization skipped to avoid data erasure.\n");
        return;
    }
    ESP_ERROR_CHECK(err);

    // Step 2: Create an NVS iterator for the "Passwords" namespace
    nvs_iterator_t it = NULL;
    err = nvs_entry_find(NVS_DEFAULT_PART_NAME, "Passwords", NVS_TYPE_ANY, &it);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        printf("No entries found in namespace 'Passwords'.\n");
        return;
    }
    ESP_ERROR_CHECK(err);

    printf("Keys in namespace 'Passwords':\n");

    // Step 3: Iterate through all keys in the namespace
    while (it != NULL) {
        nvs_entry_info_t info;
        nvs_entry_info(it, &info);
        printf("Key: %s, Type: %d\n", info.key, info.type);

        // Step 4: Open the namespace and read the value for each key
        nvs_handle_t handle;
        err = nvs_open("Passwords", NVS_READONLY, &handle);
        if (err == ESP_OK) {
            if (info.type == NVS_TYPE_I32) {
                int32_t value = 0;
                nvs_get_i32(handle, info.key, &value);
                printf("Value (int32): %" PRId32 "\n", value);  // Use PRId32 for int32_t
            } else if (info.type == NVS_TYPE_STR) {
                size_t required_size = 0;
                nvs_get_str(handle, info.key, NULL, &required_size);
                if (required_size > 0) {
                    char* str_value = malloc(required_size);
                    nvs_get_str(handle, info.key, str_value, &required_size);
                    printf("Value (string): %s\n", str_value);
                    free(str_value);
                }
            }
            nvs_close(handle);
        } else {
            printf("Failed to open namespace for key: %s\n", info.key);
        }

        // Advance to the next key
        err = nvs_entry_next(&it);
        if (err == ESP_ERR_NVS_NOT_FOUND) {
            break;
        }
        ESP_ERROR_CHECK(err);
    }

    printf("Done listing keys and values.\n");
}
