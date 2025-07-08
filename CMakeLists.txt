cmake_minimum_required(VERSION 3.31)
project(PostLog)

set(CMAKE_CXX_STANDARD 20)

# Get libpq paths from Homebrew's pg_config
execute_process(COMMAND pg_config --includedir OUTPUT_VARIABLE PG_INCLUDE_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND pg_config --libdir OUTPUT_VARIABLE PG_LIB_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
include_directories(${PG_INCLUDE_DIR})
link_directories(${PG_LIB_DIR})

# Set Homebrew include and lib paths (Apple Silicon default)
include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

add_library(PostLog STATIC PostLog.cpp)

add_executable(PostLogTests tests.cpp)

target_link_libraries(PostLog PRIVATE pqxx pq)
target_link_libraries(PostLogTests PostLog)