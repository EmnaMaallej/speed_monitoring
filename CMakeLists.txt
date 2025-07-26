```cmake
cmake_minimum_required(VERSION 3.10)

# Project name
project(SomeIpApplication)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find vsomeip package
find_library(VSOMEIP_LIBRARY NAMES vsomeip3)
find_library(VSOMEIP_SD_LIBRARY NAMES vsomeip3-sd)
find_library(VSOMEIP_CFG_LIBRARY NAMES vsomeip3-cfg)

# Check if vsomeip libraries are found
if(NOT VSOMEIP_LIBRARY OR NOT VSOMEIP_SD_LIBRARY OR NOT VSOMEIP_CFG_LIBRARY)
    message(FATAL_ERROR "vsomeip libraries not found. Please install libvsomeip3-dev.")
endif()

# Include directories for vsomeip
include_directories(
    /usr/include/vsomeip
)

# Create executable for client1
add_executable(client1 client1.cpp)
target_link_libraries(client1
    ${VSOMEIP_LIBRARY}
    ${VSOMEIP_SD_LIBRARY}
    ${VSOMEIP_CFG_LIBRARY}
    pthread
)

# Create executable for client2
add_executable(client2 client2.cpp)
target_link_libraries(client2
    ${VSOMEIP_LIBRARY}
    ${VSOMEIP_SD_LIBRARY}
    ${VSOMEIP_CFG_LIBRARY}
    pthread
)

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
```
