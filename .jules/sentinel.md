## 2024-05-18 - [Defense in Depth] Bounded String Operations
**Vulnerability:** Use of unbounded string operations like `sprintf`, `strcpy`, and `strcat`. These patterns were found extensively across core modules (`src/prog.c`, `src/gen.c`, `src/feature.c`, `src/tracelog.c`, `src/kernel/bpf/disasm.c`, `src/main.c`).
**Learning:** Even in contexts where strings might be known to fit their buffers initially, future changes or unexpected input can lead to buffer overflows. The sheer number of unbounded functions indicated a systemic vulnerability pattern that needed a defense-in-depth approach.
**Prevention:** Always use bounded equivalents: `snprintf` instead of `sprintf` or `strcpy`, and careful bounds checking for concatenations (`snprintf(dest + len, size - len, ...)` instead of `strcat`). This ensures that even if inputs change, the buffers will not overflow.
## 2026-05-15 - [Sentinel Fix: Add length bounds to sscanf in xlated_dumper.c]
**Vulnerability:** Found unbounded %s format specifiers in sscanf when parsing /proc/kallsyms in src/xlated_dumper.c.
**Learning:** Legacy C code often forgets to bound %s in format string parsers like sscanf, leaving it open to buffer overflows when parsing even trusted files like /proc/kallsyms.
**Prevention:** Always add explicit length limits like %255s that map to the actual size of the destination buffer when using family string formatting functions.
