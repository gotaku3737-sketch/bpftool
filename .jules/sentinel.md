## 2026-09-04 - [Fix strncat size_t underflow in detect_common_prefix]
**Vulnerability:** A theoretical `size_t` underflow in `strncat(msg, ref, sizeof(msg) - strlen(msg) - 1)` could lead to a stack buffer overflow. Although `snprintf` guarantees null-termination, making this unlikely to occur under normal execution, resolving it is a good defensive programming practice to prevent unexpected boundary vulnerabilities.
**Learning:** Using `sizeof(buf) - strlen(buf) - 1` without ensuring that `strlen(buf) < sizeof(buf)` can result in a massive positive integer due to `size_t` being unsigned.
**Prevention:** Guard size calculations for string concatenation using a ternary check like `len < sizeof(msg) ? sizeof(msg) - len - 1 : 0`, and ensure variables referencing length are updated between sequential operations.
## 2026-09-09 - [Fix snprintf size_t underflow]
**Vulnerability:** A theoretical size_t underflow could occur in snprintf when calculating the remaining buffer size as (MAX_LEN - strlen(buf)). If strlen(buf) >= MAX_LEN, the result is a huge positive number due to unsigned arithmetic wrapping, passing a massive size to snprintf and potentially causing a stack buffer overflow.
**Learning:** Subtractions on size_t types without bounds-checking prior to the operation can introduce subtle but critical buffer overflow vulnerabilities, especially when interacting with string formatting APIs.
**Prevention:** When determining the remaining space in a fixed-size buffer, always ensure the current length is strictly less than the maximum capacity, preferably using a ternary operator (len < MAX ? MAX - len : 0).
## 2026-09-10 - [Fix snprintf size_t underflow in do_batch]
**Vulnerability:** A theoretical size_t underflow could occur in snprintf when calculating the remaining buffer size as (sizeof(buf) - strlen(buf)). If strlen(buf) >= sizeof(buf), the result is a huge positive number due to unsigned arithmetic wrapping, passing a massive size to snprintf and potentially causing a stack buffer overflow.
**Learning:** Subtractions on size_t types without bounds-checking prior to the operation can introduce subtle but critical buffer overflow vulnerabilities, especially when interacting with string formatting APIs like snprintf.
**Prevention:** When determining the remaining space in a fixed-size buffer, always ensure the current length is strictly less than the maximum capacity, preferably using a ternary operator (len < MAX ? MAX - len : 0).
## 2024-05-24 - [size_t underflows in snprintf]
**Vulnerability:** Potential buffer overflow due to `size_t` underflows in `snprintf` remaining buffer space calculation (`size - pos`). If `pos` is larger than `size`, the result becomes a huge positive number.
**Learning:** `snprintf` expects a `size_t` for the buffer size. Always guard subtractions that calculate remaining buffer capacity, e.g., `pos < size ? size - pos : 0`, and ensure pointer arithmetic doesn't go out of bounds `buf + (pos < size ? pos : 0)`.
**Prevention:** Audit all `snprintf` usages specifically for expressions like `sizeof(buf) - len` or `size - pos` and replace them with clamped bounds checks.
## 2026-09-15 - [Fix improper strncat usage in code generation]
**Vulnerability:** The code used `var_ident[0] = '\0'; strncat(var_ident, var_name, sizeof(var_ident) - 1);` as a string copy pattern. While functionally safe because length was 0, using strncat for copying is an anti-pattern that can lead to subtle bugs or buffer overflows if the buffer isn't properly null-initialized or its previous length is misunderstood.
**Learning:** Misusing `strncat` as `strncpy` can be confusing and technically fragile depending on memory initialization. It's best to use intended functions for clarity and standard safety.
**Prevention:** Use `strncpy` coupled with manual null-termination `buf[sizeof(buf) - 1] = '\0'` for fixed-size buffer copying, rather than zeroing the first byte and appending.
## 2026-09-17 - [Fix length calculation size_t underflow in tracelog and feature]
**Vulnerability:** A theoretical `size_t` underflow in `PATH_MAX - strlen(pipe_name) - 1` or `sizeof(plain_desc) - strlen(plain_comment) - 1` could lead to buffer overflows if the string lengths are unexpectedly larger than the buffer size limits. Because `size_t` is unsigned, an underflow wraps around to a massive positive value.
**Learning:** Subtractions on size limits utilizing string lengths (like `strlen`) can underflow.
**Prevention:** Guard size calculations for bounds limiting logic using a ternary check like `strlen(str) < MAX ? MAX - strlen(str) - 1 : 0`, ensuring variables referencing length are validated against the bounds.
## 2024-05-25 - [Integer overflow in buffer growth size calculation]
**Vulnerability:** Exact equality bounds checks before exponentially growing a buffer size (e.g., `if (buf_size == UINT32_MAX)`) are unsafe. If `buf_size` is multiplied by 2 and exceeds `UINT32_MAX`, it can trigger integer overflow due to type limits or size conversions, bypassing the check entirely in future iterations. In functions passing sizes to `realloc`, this might wrap around to a small size or 0, leading to a double free or small buffer allocation followed by a heap buffer overflow.
**Learning:** Exact equality checks against max constants are ineffective when values grow exponentially. Buffer growth must cap at the maximum safe limit explicitly before the multiplication.
**Prevention:** Use `>=` combined with clamping to safely grow sizes: `if (buf_size >= MAX / 2) buf_size = MAX; else buf_size *= 2;`.
