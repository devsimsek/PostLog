# PostLog

PostLog is a simple, extensible C++ logging library with support for console and PostgreSQL database sinks. It is
designed for easy integration into C++ projects that require structured logging and persistent log storage.

## Features

- Log to console or PostgreSQL database
- Pluggable sink architecture (add your own sinks)
- Simple API for logging messages with levels and timestamps
- Easy setup for database tables

## Requirements

- C++20 or newer
- [libpqxx](https://github.com/jtv/libpqxx) (PostgreSQL C++ client library)
- PostgreSQL server (for database logging)
- CMake (build system)
- Ninja (recommended for fast builds)

## Building

To build the project, use the provided script:

```sh
./build.sh
```

This script will:

- Remove any existing `cmake-build-debug` directory
- Generate the build system with CMake and Ninja
- Build the project
- Place all build artifacts in `cmake-build-debug/`

## Usage Example

See `tests.cpp` for a complete example. Basic usage:

```cpp
#include "PostLog.h"
#include <pqxx/pqxx>

int main() {
    pqxx::connection conn("dbname=PostLog user=PostLog");
    PostLog::DatabaseSink dbSink(&conn, "logs");
    dbSink.setup();
    dbSink.log({"Hello, world!", PostLog::LogLevel::Info, std::to_string(time(nullptr))});
}
```

## Project Structure

- `PostLog.h` / `PostLog.cpp`: Library source code
- `tests.cpp`: Example and test usage
- `CMakeLists.txt`: Build configuration
- `cmake-build-debug/`: Build artifacts (auto-generated)

## Author

[github.com/devsimsek](https://github.com/devsimsek)

## License

[Mit License](LICENSE)