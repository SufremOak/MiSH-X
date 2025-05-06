# Kindle library CMake configuration

# Add Kindle library sources
add_library(kindle STATIC
    openstd.c
)

# Include directories
target_include_directories(kindle PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link dependencies
target_link_libraries(kindle
    PRIVATE
    mishx
)

# Installation
install(TARGETS kindle
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
