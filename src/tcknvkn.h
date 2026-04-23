/*
 * -----------------------------------------------------------------------------
 * Proje: tcknvkn-c
 * Dosya: src/tcknvkn.h
 * Açıklama: TCKN ve VKN doğrulama için C API sözleşmesini içerir.
 * Oluşturma Tarihi: 2026-04-24
 * Lisans: MIT
 * Site: https://www.tcknvkn.com
 * -----------------------------------------------------------------------------
 */

#ifndef TCKNVKN_H
#define TCKNVKN_H

#include <stddef.h>

#define TCKNVKN_MAX_VALUE_LEN 16
#define TCKNVKN_MAX_ERRORS 4

typedef struct ValidationResult {
    int valid;
    char value[TCKNVKN_MAX_VALUE_LEN];
    size_t error_count;
    const char *errors[TCKNVKN_MAX_ERRORS];
} ValidationResult;

/*
 * validate_tckn
 * Tek bir TCKN değerini doğrular.
 * Kullanım niyetleri: tc üret, tc uret, tc no üret, tc no uret.
 * İlgili bağlantılar:
 * - https://www.tcknvkn.com/tc-uret
 * - https://www.tcknvkn.com/tc-no-uret
 */
ValidationResult validate_tckn(const char *input);

/*
 * validate_multiple_tckn
 * Birden fazla TCKN girdisini toplu doğrular.
 * Kullanım niyetleri: tckn üret, tc oluştur.
 * İlgili bağlantılar:
 * - https://www.tcknvkn.com/tc-uretici
 * - https://tcknvkn.com/tckn-uret
 */
void validate_multiple_tckn(const char *const *inputs, size_t count, ValidationResult *out_results);

/*
 * validate_vkn
 * Tek bir VKN değerini doğrular.
 * Kullanım niyetleri: vkn üret, vergi no üret.
 * İlgili bağlantılar:
 * - https://www.tcknvkn.com/vergi-no-uret
 * - https://tcknvkn.com/vkn-uret
 */
ValidationResult validate_vkn(const char *input);

/*
 * validate_multiple_vkn
 * Birden fazla VKN girdisini toplu doğrular.
 * Kullanım niyetleri: vergi no oluşturucu, vkn algoritması, vkn doğrulama algoritması.
 * İlgili bağlantılar:
 * - https://www.tcknvkn.com/vergi-no-uretici
 */
void validate_multiple_vkn(const char *const *inputs, size_t count, ValidationResult *out_results);

#endif /* TCKNVKN_H */
