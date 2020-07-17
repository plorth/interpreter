# Plorth interpreter

[![GitHub Actions](https://img.shields.io/endpoint.svg?url=https%3A%2F%2Factions-badge.atrox.dev%2Fplorth%2Finterpreter%2Fbadge&label=build&logo=none)](https://actions-badge.atrox.dev/plorth/interpreter/goto)

Interpreter for [Plorth programming language](https://plorth.org) written as
C++ library.

## Compilation

C++17 capable compiler and [CMake] are required to compile the interpreter.
Dependencies are handled by [cget], which also needs to be installed.

```bash
$ cget install
$ mkdir build
$ cd build
$ cmake ..
$ make
```

[CMake]: https://cmake.org
[cget]: https://github.com/pfultz2/cget
