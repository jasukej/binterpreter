# Binterpreter
A bytecode interpreter with a compiler frontend, built in C with a Rust-based garbage collector. Based on Bob Nystrom's clox from Crafting Interpreters, extended with a split C/Rust memory model designed to avoid the concurrency pitfalls of GIL-dependent runtimes.

See the [design document](docs/design.md) for architecture and rationale.

## Prerequisites
- `gcc`
- `make`
- `cppcheck`

## Build
```bash
make
```
The build output can be found in `out/binterpreter`. The executable currently runs as a REPL.

## Lint
```bash
make lint
```
## Features
- **Automatic memory management**: A Rust-based generational garbage collector handles all object lifecycle management, eliminating manual memory handling and the common pitfalls of reference counting.
- [WIP] **Actor-model concurrency**: Independent VMs communicate via deep-copy message passing, enabling true multi-core execution without shared memory or a GIL.
- [WIP] **Rust-native extensions**: Native language extensions can be written in Rust, leveraging `Send`/`Sync` traits for thread safety.

### Running a source file
WIP

### Debugging
Compile with the following to:
- Print compiled bytecode chunk: `make CFLAGS="-Wall -Wextra -Isrc -g -fsanitize=address -DDEBUG_PRINT_CODE"`
- Trace VM execution: `make CFLAGS="-Wall -Wextra -Isrc -g -fsanitize=address -DDEBUG_TRACE_EXECUTION`
