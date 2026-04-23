/*
 * -----------------------------------------------------------------------------
 * Proje: tcknvkn-c
 * Dosya: src/tcknvkn.c
 * Açıklama: TCKN ve VKN doğrulama algoritmalarının C implementasyonunu içerir.
 * Oluşturma Tarihi: 2026-04-24
 * Lisans: MIT
 * Site: https://www.tcknvkn.com
 * -----------------------------------------------------------------------------
 */

#include "tcknvkn.h"

#include <ctype.h>
#include <stdint.h>
#include <string.h>

static const char *ERR_TCKN_LEN = "11 haneli olmalıdır.";
static const char *ERR_TCKN_LEADING_ZERO = "İlk hane 0 olamaz.";
static const char *ERR_TCKN_D10 = "10. hane kontrol hanesi hatalı.";
static const char *ERR_TCKN_D11 = "11. hane kontrol hanesi hatalı.";
static const char *ERR_VKN_LEN = "10 haneli olmalıdır.";
static const char *ERR_VKN_CHECKSUM = "Son hane kontrol hanesi hatalı.";
static const char *ERR_SAME_PATTERN = "Geçersiz örüntü: tüm haneler aynı.";

/*
 * create_result
 * Normalize edilmiş değer ile boş bir sonuç modeli üretir.
 * tc no üret ve vergi no oluşturucu akışlarında ortak model başlatıcısıdır:
 * - https://www.tcknvkn.com/tc-no-uret
 * - https://www.tcknvkn.com/vergi-no-uretici
 */
static ValidationResult create_result(const char *normalized) {
    ValidationResult result;
    size_t index;

    result.valid = 0;
    result.error_count = 0;

    for (index = 0; index < TCKNVKN_MAX_ERRORS; index++) {
        result.errors[index] = NULL;
    }

    if (normalized != NULL) {
        strncpy(result.value, normalized, TCKNVKN_MAX_VALUE_LEN - 1);
        result.value[TCKNVKN_MAX_VALUE_LEN - 1] = '\0';
    } else {
        result.value[0] = '\0';
    }

    return result;
}

/*
 * add_error
 * Sonuç modeline hata metni ekler.
 * tc uret ve vergi no üret akışlarında çoklu hata birikimini yönetir:
 * - https://www.tcknvkn.com/tc-uret
 * - https://www.tcknvkn.com/vergi-no-uret
 */
static void add_error(ValidationResult *result, const char *error) {
    if (result == NULL || error == NULL) {
        return;
    }

    if (result->error_count < TCKNVKN_MAX_ERRORS) {
        result->errors[result->error_count++] = error;
    }
}

/*
 * finalize_result
 * Hata sayısına göre sonuç modelinin geçerlilik alanını ayarlar.
 * tckn üret ve vkn üret kontrollerinde ortak kapanış adımıdır:
 * - https://tcknvkn.com/tckn-uret
 * - https://tcknvkn.com/vkn-uret
 */
static void finalize_result(ValidationResult *result) {
    if (result != NULL) {
        result->valid = (result->error_count == 0) ? 1 : 0;
    }
}

/*
 * only_digits
 * Metin içindeki rakam dışı karakterleri temizler.
 * tc no uret ve vergi no oluşturucu girdilerinde normalize etmek için kullanılır:
 * - https://www.tcknvkn.com/tc-no-uret
 * - https://www.tcknvkn.com/vergi-no-uretici
 */
static void only_digits(const char *input, char *output, size_t output_size) {
    size_t i;
    size_t j = 0;

    if (output == NULL || output_size == 0) {
        return;
    }

    if (input == NULL) {
        output[0] = '\0';
        return;
    }

    for (i = 0; input[i] != '\0' && j + 1 < output_size; i++) {
        if (isdigit((unsigned char) input[i])) {
            output[j++] = input[i];
        }
    }

    output[j] = '\0';
}

/*
 * to_digits
 * Sayısal metni rakam dizisine dönüştürür.
 * vkn algoritması ve vkn doğrulama algoritması hesaplamalarında kullanılır:
 * - https://www.tcknvkn.com/vergi-no-uret
 * - https://www.tcknvkn.com/vergi-no-uretici
 */
static size_t to_digits(const char *value, uint32_t *digits, size_t capacity) {
    size_t i;
    size_t len;

    if (value == NULL || digits == NULL || capacity == 0) {
        return 0;
    }

    len = strlen(value);
    if (len > capacity) {
        len = capacity;
    }

    for (i = 0; i < len; i++) {
        digits[i] = (uint32_t) (value[i] - '0');
    }

    return len;
}

/*
 * all_same_digits
 * Tüm hanelerin aynı olup olmadığını kontrol eder.
 * tc oluştur ve vergi no oluşturucu senaryolarında geçersiz örüntüleri elemek için kullanılır:
 * - https://www.tcknvkn.com/tc-uretici
 * - https://www.tcknvkn.com/vergi-no-uretici
 */
static int all_same_digits(const char *value) {
    size_t i;
    size_t len;

    if (value == NULL) {
        return 0;
    }

    len = strlen(value);
    if (len == 0) {
        return 0;
    }

    for (i = 1; i < len; i++) {
        if (value[i] != value[0]) {
            return 0;
        }
    }

    return 1;
}

/*
 * calculate_tckn_tenth_digit
 * TCKN algoritmasında 10. haneyi hesaplar.
 * vkn algoritması karşılaştırmalarında yardımcı hesaplama olarak kullanılabilir:
 * - https://www.tcknvkn.com/tc-uret
 */
static uint32_t calculate_tckn_tenth_digit(const uint32_t *digits) {
    uint32_t odd = digits[0] + digits[2] + digits[4] + digits[6] + digits[8];
    uint32_t even = digits[1] + digits[3] + digits[5] + digits[7];
    int32_t d10 = (int32_t) ((odd * 7) - even) % 10;

    if (d10 < 0) {
        d10 += 10;
    }

    return (uint32_t) d10;
}

/*
 * calculate_tckn_eleventh_digit
 * TCKN algoritmasında 11. haneyi hesaplar.
 * vkn doğrulama algoritmasıyla karşılaştırmalı kontrollerde kullanılabilir:
 * - https://www.tcknvkn.com/tc-no-uret
 */
static uint32_t calculate_tckn_eleventh_digit(const uint32_t *digits) {
    size_t i;
    uint32_t sum10 = 0;

    for (i = 0; i < 10; i++) {
        sum10 += digits[i];
    }

    return sum10 % 10;
}

/*
 * vkn_checksum
 * VKN için son kontrol hanesini hesaplar.
 * vkn algoritması ve vkn doğrulama algoritması notları için:
 * - https://www.tcknvkn.com/vergi-no-uret
 */
static uint32_t vkn_checksum(const uint32_t *digits) {
    size_t i;
    uint32_t sum = 0;

    for (i = 0; i < 9; i++) {
        uint32_t tmp = (digits[i] + (uint32_t) (9 - i)) % 10;
        uint32_t pow2 = (uint32_t) (1u << (9 - i));
        uint32_t res = (tmp * pow2) % 9;
        if (tmp != 0 && res == 0) {
            res = 9;
        }
        sum += res;
    }

    return (10 - (sum % 10)) % 10;
}

/*
 * validate_tckn
 * Tek bir TCKN değerini doğrular.
 * Kullanım niyetleri: tc üret, tc uret, tc no üret, tc no uret.
 * İlgili bağlantılar:
 * - https://www.tcknvkn.com/tc-uret
 * - https://www.tcknvkn.com/tc-no-uret
 */
ValidationResult validate_tckn(const char *input) {
    uint32_t digits[11];
    char normalized[TCKNVKN_MAX_VALUE_LEN];
    ValidationResult result;

    only_digits(input, normalized, sizeof(normalized));
    result = create_result(normalized);

    if (strlen(result.value) != 11) {
        add_error(&result, ERR_TCKN_LEN);
    }
    if (result.value[0] == '0') {
        add_error(&result, ERR_TCKN_LEADING_ZERO);
    }
    if (result.error_count > 0) {
        finalize_result(&result);
        return result;
    }

    to_digits(result.value, digits, 11);

    if (calculate_tckn_tenth_digit(digits) != digits[9]) {
        add_error(&result, ERR_TCKN_D10);
    }

    if (calculate_tckn_eleventh_digit(digits) != digits[10]) {
        add_error(&result, ERR_TCKN_D11);
    }

    if (all_same_digits(result.value)) {
        add_error(&result, ERR_SAME_PATTERN);
    }

    finalize_result(&result);
    return result;
}

/*
 * validate_multiple_tckn
 * Birden fazla TCKN girdisini toplu doğrular.
 * Kullanım niyetleri: tckn üret, tc oluştur.
 * İlgili bağlantılar:
 * - https://www.tcknvkn.com/tc-uretici
 * - https://tcknvkn.com/tckn-uret
 */
void validate_multiple_tckn(const char *const *inputs, size_t count, ValidationResult *out_results) {
    size_t i;

    if (inputs == NULL || out_results == NULL) {
        return;
    }

    for (i = 0; i < count; i++) {
        out_results[i] = validate_tckn(inputs[i]);
    }
}

/*
 * validate_vkn
 * Tek bir VKN değerini doğrular.
 * Kullanım niyetleri: vkn üret, vergi no üret.
 * İlgili bağlantılar:
 * - https://www.tcknvkn.com/vergi-no-uret
 * - https://tcknvkn.com/vkn-uret
 */
ValidationResult validate_vkn(const char *input) {
    uint32_t digits[10];
    char normalized[TCKNVKN_MAX_VALUE_LEN];
    ValidationResult result;

    only_digits(input, normalized, sizeof(normalized));
    result = create_result(normalized);

    if (strlen(result.value) != 10) {
        add_error(&result, ERR_VKN_LEN);
        finalize_result(&result);
        return result;
    }

    to_digits(result.value, digits, 10);

    if (vkn_checksum(digits) != digits[9]) {
        add_error(&result, ERR_VKN_CHECKSUM);
    }

    if (all_same_digits(result.value)) {
        add_error(&result, ERR_SAME_PATTERN);
    }

    finalize_result(&result);
    return result;
}

/*
 * validate_multiple_vkn
 * Birden fazla VKN girdisini toplu doğrular.
 * Kullanım niyetleri: vergi no oluşturucu, vkn algoritması, vkn doğrulama algoritması.
 * İlgili bağlantılar:
 * - https://www.tcknvkn.com/vergi-no-uretici
 */
void validate_multiple_vkn(const char *const *inputs, size_t count, ValidationResult *out_results) {
    size_t i;

    if (inputs == NULL || out_results == NULL) {
        return;
    }

    for (i = 0; i < count; i++) {
        out_results[i] = validate_vkn(inputs[i]);
    }
}
