# Binterpreter
A simple bytecode interpreter with a compiler frontend. Largely based off Bob Nystrom's clox in the book Crafting Interpreters, with a spattering of features inspired by modern runtimes.

## Prerequisites
- `gcc`
- `make`
- `cppcheck`

## Build
```bash
make
```
The build output can be found in `out/binterpreter`. The executable current runs as a REPL.

## Lint
```bash
make lint
```
## Features
- [WIP] **Generational garbage collection**: optimize for short-lived objects and reduce pause time.
    - Success criteria: Lower total GC time on allocation-heavy workloads; will need to look into existing literature to quantify
- [WIP] **Quickening**: reduce dispatch overhead by specializing hot opcodes. 
    - Success criteria: 10% speedup on arithmetic/property benchmarks
- [WIP] **Inline caching**: cache repeated lookup metadata at bytecode locations. 
    - Success criteria: lower lookup cost and fewer slow-path hits; will need to look into benchmarks further to quantify
- [WIP] **Microbenchmarking suite**: quantify optimizations
    - Success criteria: reproducible benchmark runs, cache baseline comparisons

### Running a source file
WIP

### Debugging
Compile with the following to:
- Print compiled bytecode chunk: `make CFLAGS="-Wall -Wextra -Isrc -g -fsanitize=address -DDEBUG_PRINT_CODE"`
- Trace VM execution: `make CFLAGS="-Wall -Wextra -Isrc -g -fsanitize=address -DDEBUG_TRACE_EXECUTION`
