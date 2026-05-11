# Binterpreter
A bytecode interpreter with a compiler frontend, built in C with a Rust-based garbage collector. Based on Bob Nystrom's clox from Crafting Interpreters, extended with a split C/Rust memory model designed to avoid the concurrency pitfalls of GIL-dependent runtimes.

See the [design document](docs/design.md) for architecture and rationale, and [features](docs/features.md) for notes on key language design decisions.

## Prerequisites
- `gcc`
- `make`
- `cppcheck` (for linting)

## Build
```bash
make
```
The binary is output to `out/binterpreter`.

## Usage

### REPL
```bash
./out/binterpreter
```
Starts an interactive prompt. Enter Lox statements one at a time:
```
> print 1 + 2;
3
> var name = "world";
> print "hello " + name;
hello world
```

### Running a source file
```bash
./out/binterpreter path/to/script.lox
```

## Test
```bash
make test
```
Builds a separate binary (`out/binterpreter-test`) with debug tracing disabled, then runs all `.lox` test files under `tests/`. Each test file contains `// expect:` comments that are compared against the interpreter's actual output.

Tests are organized by feature:
```
tests/
├── closures/          # upvalue capture & closed-over variables
├── control_flow/      # if/else, while, for
├── expressions/       # arithmetic, comparison, strings
├── functions/         # declarations & return values
└── variables/         # globals, locals, shadowing
```

## Features
- **Single-pass compiler**: Pratt parser that scans tokens and emits bytecode directly, with no intermediate AST.
- **Stack-based VM**: Executes variable-width bytecode with a value stack, call frames, and closures with upvalues.
- [WIP] **Automatic memory management**: A generational garbage collector to handle object lifecycle management, replacing the current teardown-only model.
- [WIP] **Actor-model concurrency**: Independent VMs communicating via deep-copy message passing to enable multi-core execution without shared memory or a GIL.
- [WIP] **Rust-native extensions**: Native language extensions written in Rust with thread safety enforced by `Send`/`Sync` traits.

## Debugging
Debug flags can be enabled by overriding `CFLAGS`:
```bash
# Print disassembled bytecode after compilation
make CFLAGS="-Wall -Wextra -Isrc -g -fsanitize=address -DDEBUG_PRINT_CODE"

# Trace every VM instruction during execution
make CFLAGS="-Wall -Wextra -Isrc -g -fsanitize=address -DDEBUG_TRACE_EXECUTION"
```
Both flags are enabled by default in the standard build and suppressed in the test build via `-DNDEBUG_TRACE`.

## Lint
```bash
make lint
```

## Acknowledgements
Based on Bob Nystrom's [Crafting Interpreters](https://craftinginterpreters.com/) (2021), which served as the reference implementation for the bytecode VM and compiler frontend.