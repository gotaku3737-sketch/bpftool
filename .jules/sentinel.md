## 2024-05-24 - Fix out-of-bounds array access in string stripping
**Vulnerability:** A loop in `src/jit_disasm.c` responsible for stripping trailing spaces from a dynamically allocated string failed to check the index boundary. If the string was empty or contained only spaces, the loop index would reach `-1`, leading to an out-of-bounds read and write (`s[-1]`). This causes memory corruption and undefined behavior.
**Learning:** Legacy string manipulation loops that decrement indices based solely on a character match condition are prone to underflow, especially when dealing with empty strings or strings matching only the condition.
**Prevention:** Always verify that loop indices remain within valid bounds (e.g., `i >= 0`) when iterating backward through an array or string.

## 2024-05-24 - Fix Uninitialized VLA Format String
**Vulnerability:** In `src/xlated_dumper.c`, a VLA `char chomped_fmt[l];` was created using `strlen(fmt)` directly. If `fmt` was an empty string, a zero-length array was created, and it was passed uninitialized as a format string to `jsonw_vprintf_enquote()`, causing an uninitialized memory read and a severe format string vulnerability because it interpreted arbitrary stack memory.
**Learning:** Using `strlen()` to determine VLA size without guarding against `0` length can result in zero-length arrays that bypass initialization logic but are still passed to unsafe sink functions expecting valid C strings.
**Prevention:** Always ensure arrays (especially VLAs used for strings) have a minimum size of 1 for the null terminator and are explicitly null-terminated if default initialization logic is skipped.
