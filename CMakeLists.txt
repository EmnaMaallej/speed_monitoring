cmake_minimum_required(VERSION 3.10)
project(speed_monitoring)

set(CMAKE_CXX_STANDARD 11)
find_package(PkgConfig REQUIRED)
pkg_check_modules(VSOMEIP REQUIRED vsomeip)

include_directories(include ${VSOMEIP_INCLUDE_DIRS})
file(GLOB SOURCES "src/*.cpp")

add_executable(speed_monitoring main.cpp ${SOURCES})
target_link_libraries(speed_monitoring ${VSOMEIP_LIBRARIES})
target_include_directories(speed_monitoring PRIVATE ${VSOMEIP_INCLUDE_DIRS})
