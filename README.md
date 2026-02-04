# The STB-style chaotic non-standard stdlib extension for C

- Usage:
  ```bash
    wget https://raw.githubusercontent.com/Shabani005/chaos.h/refs/heads/master/chaos.h
  ```

- Build Examples:
```bash
  cc -o builder builder.c
  ./builder
```

  ```c
    #define CHAOS_IMPLEMENTATION
    #include "chaos.h"
  ```
---
- Provides:
  - String utils
  - File utils
  - "Build System" utils
  - misc utils
  - Arenas
  - Hash tables
---  
- Philosophy:
   - C is an old language but does not have to feel that way, this library attempts to port features from modern languages.
