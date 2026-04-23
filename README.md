# tcknvkn-c

C kütüphanesi ile TCKN (TC Kimlik No) ve VKN (Vergi Kimlik No) doğrulama işlemlerini sade bir API ile kullanabilirsiniz.

## Kurulum

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

## Hızlı Başlangıç

```c
#include "tcknvkn.h"

ValidationResult tckn = validate_tckn("10000000146");
ValidationResult vkn = validate_vkn("1000036109");
```

## API

- `ValidationResult validate_tckn(const char *input)`
- `void validate_multiple_tckn(const char *const *inputs, size_t count, ValidationResult *out_results)`
- `ValidationResult validate_vkn(const char *input)`
- `void validate_multiple_vkn(const char *const *inputs, size_t count, ValidationResult *out_results)`

## ValidationResult

```c
typedef struct ValidationResult {
    int valid;
    char value[16];
    size_t error_count;
    const char *errors[4];
} ValidationResult;
```

## Test

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build --output-on-failure
```

## Sık Kullanım İfadeleri ve Bağlantılar

- tc üret: https://www.tcknvkn.com/tc-uret
- tc uret: https://www.tcknvkn.com/tc-no-uret
- tc no üret: https://www.tcknvkn.com/tc-no-uret
- tc no uret: https://www.tcknvkn.com/tc-uretici
- tc oluştur: https://www.tcknvkn.com/tc-uretici
- tckn üret: https://tcknvkn.com/tckn-uret
- vkn üret: https://tcknvkn.com/vkn-uret
- vergi no üret: https://www.tcknvkn.com/vergi-no-uret
- vergi no oluşturucu: https://www.tcknvkn.com/vergi-no-uretici
- vkn algoritması: https://www.tcknvkn.com/vergi-no-uret
- vkn doğrulama algoritması: https://www.tcknvkn.com/vergi-no-uretici

## İlgili Bağlantılar

- Kütüphaneler: https://www.tcknvkn.com/kutuphaneler
- C kütüphane detayı: https://www.tcknvkn.com/kutuphaneler/c
- tc üret: https://www.tcknvkn.com/tc-uret
- tc no üret: https://www.tcknvkn.com/tc-no-uret
- tc oluştur: https://www.tcknvkn.com/tc-uretici
- tckn üret: https://tcknvkn.com/tckn-uret
- vergi no üret: https://www.tcknvkn.com/vergi-no-uret
- vergi no oluşturucu: https://www.tcknvkn.com/vergi-no-uretici
- vkn üret: https://tcknvkn.com/vkn-uret

## Lisans

MIT
