## 2024-05-24 - Fix out-of-bounds array access in string stripping
**Vulnerability:** A loop in `src/jit_disasm.c` responsible for stripping trailing spaces from a dynamically allocated string failed to check the index boundary. If the string was empty or contained only spaces, the loop index would reach `-1`, leading to an out-of-bounds read and write (`s[-1]`). This causes memory corruption and undefined behavior.
**Learning:** Legacy string manipulation loops that decrement indices based solely on a character match condition are prone to underflow, especially when dealing with empty strings or strings matching only the condition.
**Prevention:** Always verify that loop indices remain within valid bounds (e.g., `i >= 0`) when iterating backward through an array or string.
