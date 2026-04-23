/*
 * -----------------------------------------------------------------------------
 * Proje: tcknvkn-c
 * Dosya: examples/quickstart.c
 * Açıklama: TCKN ve VKN doğrulama fonksiyonlarının hızlı kullanım örneğini içerir.
 * Oluşturma Tarihi: 2026-04-24
 * Lisans: MIT
 * Site: https://www.tcknvkn.com
 * -----------------------------------------------------------------------------
 */

#include <stdio.h>

#include "tcknvkn.h"

int main(void) {
    ValidationResult tckn = validate_tckn("10000000146");
    ValidationResult vkn = validate_vkn("1000036109");

    printf("TCKN valid: %d\n", tckn.valid);
    printf("VKN valid: %d\n", vkn.valid);

    return 0;
}
