cmake_minimum_required(VERSION 3.10)
project(MiSH-X C CXX)

# Set C/C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Add subdirectories for different components
add_subdirectory(lib)
add_subdirectory(win32)

# Main library target
add_library(mishx STATIC
    lib/core.c
)

# Platform-specific targets
add_library(win32 STATIC
    win32/neo32.h
)

# Tools target
add_executable(tool
    ibn/util.self.c
    ibn/gblob/main.py
)

# Link dependencies
target_link_libraries(tool
    mishx
    win32
)

# Installation
install(TARGETS mishx win32 tool
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
