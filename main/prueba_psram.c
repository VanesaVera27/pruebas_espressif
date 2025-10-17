#include <stdio.h>
#include "esp_system.h"
#include "esp_heap_caps.h"
#include "esp_psram.h"

void app_main(void)
{
    printf("\n===== TEST DE PSRAM =====\n");

    if (!esp_psram_is_initialized()) {
        printf("❌ PSRAM no detectada o no inicializada.\n");
        return;
    }

    printf("✅ PSRAM inicializada correctamente.\n");
    size_t psram_size = esp_psram_get_size();
    printf("Tamaño total de PSRAM: %d bytes (%.2f MB)\n", psram_size, psram_size / 1024.0 / 1024.0);

    // Intentar reservar 1 MB en PSRAM
    size_t test_size = 1024 * 1024;
    uint8_t *buffer = (uint8_t *)heap_caps_malloc(test_size, MALLOC_CAP_SPIRAM);

    if (buffer == NULL) {
        printf("❌ Error al reservar memoria en PSRAM.\n");
        return;
    }

    // Escribir y verificar
    for (size_t i = 0; i < test_size; i++) {
        buffer[i] = (uint8_t)(i % 256);
    }

    int ok = 1;
    for (size_t i = 0; i < test_size; i++) {
        if (buffer[i] != (uint8_t)(i % 256)) {
            ok = 0;
            printf("❌ Error de lectura en posición %d\n", (int)i);
            break;
        }
    }

    if (ok) {
        printf("✅ Prueba de escritura/lectura en PSRAM exitosa.\n");
    }

    free(buffer);
    printf("============================\n");
}
