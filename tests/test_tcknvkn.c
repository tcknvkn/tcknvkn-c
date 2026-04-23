/*
 * -----------------------------------------------------------------------------
 * Proje: tcknvkn-c
 * Dosya: tests/test_tcknvkn.c
 * Açıklama: TCKN ve VKN doğrulama fonksiyonları için varyasyonlu birim testlerini içerir.
 * Oluşturma Tarihi: 2026-04-24
 * Lisans: MIT
 * Site: https://www.tcknvkn.com
 * -----------------------------------------------------------------------------
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "tcknvkn.h"

/* contains_error hata listesinde beklenen hata metnini arar. */
static int contains_error(const ValidationResult *result, const char *expected) {
    size_t i;

    if (result == NULL || expected == NULL) {
        return 0;
    }

    for (i = 0; i < result->error_count; i++) {
        if (result->errors[i] != NULL && strcmp(result->errors[i], expected) == 0) {
            return 1;
        }
    }

    return 0;
}

/* test_validate_tckn_valid geçerli bir TCKN doğrulamasını test eder. */
static void test_validate_tckn_valid(void) {
    ValidationResult result = validate_tckn("10000000146");
    assert(result.valid == 1);
    assert(strcmp(result.value, "10000000146") == 0);
    assert(result.error_count == 0);
}

/* test_validate_tckn_normalize normalize edilmiş TCKN girdisini test eder. */
static void test_validate_tckn_normalize(void) {
    ValidationResult result = validate_tckn("100-000 00146");
    assert(result.valid == 1);
    assert(strcmp(result.value, "10000000146") == 0);
}

/* test_validate_tckn_invalid_length uzunluğu hatalı TCKN girdisini test eder. */
static void test_validate_tckn_invalid_length(void) {
    ValidationResult result = validate_tckn("12345");
    assert(result.valid == 0);
    assert(contains_error(&result, "11 haneli olmalıdır."));
}

/* test_validate_tckn_leading_zero ilk hanesi sıfır olan TCKN girdisini test eder. */
static void test_validate_tckn_leading_zero(void) {
    ValidationResult result = validate_tckn("01234567890");
    assert(result.valid == 0);
    assert(contains_error(&result, "İlk hane 0 olamaz."));
}

/* test_validate_tckn_checksum checksum hatalı TCKN girdisini test eder. */
static void test_validate_tckn_checksum(void) {
    ValidationResult result = validate_tckn("10000000145");
    assert(result.valid == 0);
    assert(contains_error(&result, "11. hane kontrol hanesi hatalı."));
}

/* test_validate_tckn_same_digits tüm haneleri aynı TCKN girdisini test eder. */
static void test_validate_tckn_same_digits(void) {
    ValidationResult result = validate_tckn("11111111111");
    assert(result.valid == 0);
    assert(contains_error(&result, "Geçersiz örüntü: tüm haneler aynı."));
}

/* test_validate_tckn_multiple_errors çoklu hata üretimini test eder. */
static void test_validate_tckn_multiple_errors(void) {
    ValidationResult result = validate_tckn("0");
    assert(result.valid == 0);
    assert(contains_error(&result, "11 haneli olmalıdır."));
    assert(contains_error(&result, "İlk hane 0 olamaz."));
}

/* test_validate_vkn_valid geçerli bir VKN doğrulamasını test eder. */
static void test_validate_vkn_valid(void) {
    ValidationResult result = validate_vkn("1000036109");
    assert(result.valid == 1);
    assert(strcmp(result.value, "1000036109") == 0);
}

/* test_validate_vkn_normalize normalize edilmiş VKN girdisini test eder. */
static void test_validate_vkn_normalize(void) {
    ValidationResult result = validate_vkn("100-003-6109");
    assert(result.valid == 1);
    assert(strcmp(result.value, "1000036109") == 0);
}

/* test_validate_vkn_invalid_length uzunluğu hatalı VKN girdisini test eder. */
static void test_validate_vkn_invalid_length(void) {
    ValidationResult result = validate_vkn("1234");
    assert(result.valid == 0);
    assert(contains_error(&result, "10 haneli olmalıdır."));
}

/* test_validate_vkn_checksum checksum hatalı VKN girdisini test eder. */
static void test_validate_vkn_checksum(void) {
    ValidationResult result = validate_vkn("1000036108");
    assert(result.valid == 0);
    assert(contains_error(&result, "Son hane kontrol hanesi hatalı."));
}

/* test_validate_vkn_same_digits tüm haneleri aynı VKN girdisini test eder. */
static void test_validate_vkn_same_digits(void) {
    ValidationResult result = validate_vkn("1111111111");
    assert(result.valid == 0);
    assert(contains_error(&result, "Geçersiz örüntü: tüm haneler aynı."));
}

/* test_validate_multiple_tckn çoklu TCKN doğrulamasını test eder. */
static void test_validate_multiple_tckn(void) {
    const char *inputs[] = {"10000000146", "10000000145", "11111111111"};
    ValidationResult results[3];

    validate_multiple_tckn(inputs, 3, results);
    assert(results[0].valid == 1);
    assert(results[1].valid == 0);
    assert(results[2].valid == 0);
}

/* test_validate_multiple_tckn_empty boş TCKN listesi senaryosunu test eder. */
static void test_validate_multiple_tckn_empty(void) {
    ValidationResult results[1];
    validate_multiple_tckn(NULL, 0, results);
}

/* test_validate_multiple_vkn çoklu VKN doğrulamasını test eder. */
static void test_validate_multiple_vkn(void) {
    const char *inputs[] = {"1000036109", "1000036108", "1111111111"};
    ValidationResult results[3];

    validate_multiple_vkn(inputs, 3, results);
    assert(results[0].valid == 1);
    assert(results[1].valid == 0);
    assert(results[2].valid == 0);
}

/* test_validate_multiple_vkn_empty boş VKN listesi senaryosunu test eder. */
static void test_validate_multiple_vkn_empty(void) {
    ValidationResult results[1];
    validate_multiple_vkn(NULL, 0, results);
}

int main(void) {
    test_validate_tckn_valid();
    test_validate_tckn_normalize();
    test_validate_tckn_invalid_length();
    test_validate_tckn_leading_zero();
    test_validate_tckn_checksum();
    test_validate_tckn_same_digits();
    test_validate_tckn_multiple_errors();

    test_validate_vkn_valid();
    test_validate_vkn_normalize();
    test_validate_vkn_invalid_length();
    test_validate_vkn_checksum();
    test_validate_vkn_same_digits();

    test_validate_multiple_tckn();
    test_validate_multiple_tckn_empty();
    test_validate_multiple_vkn();
    test_validate_multiple_vkn_empty();

    printf("All tests passed.\n");
    return 0;
}
