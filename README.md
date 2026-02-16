# kshell

A simple Unix shell written in C.

## Features

- Custom prompt displaying username, hostname, and current working directory
- Built-in commands:
  - `cd` - change directory
- Executes external commands via `execvp`
- Command history using readline
- Colored terminal output

## Build

```bash
mkdir build && cd build
cmake ..
make
```

## Usage

```bash
./kshell
```

The shell will display a prompt in the format `user@hostname/path> ` and accept commands.

## Project Structure

```
kshell/
├── CMakeLists.txt    # Build configuration
├── include/          # Header files
│   ├── commands.h
│   └── functions.h
└── src/              # Source files
    ├── main.c        # Entry point and command processing
    ├── commands.c    # Built-in command implementations
    └── functions.c   # Utility functions
```

## Dependencies

- CMake
- Clang (default compiler)
- readline library
