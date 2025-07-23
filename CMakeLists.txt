cmake_minimum_required(VERSION 3.10)
project(SomeIPApp)

find_library(VSOMEIP_LIBRARY vsomeip3)
find_library(VSOMEIP_SD_LIBRARY vsomeip3-sd)

add_executable(client1 client1.cpp)
target_link_libraries(client1 ${VSOMEIP_LIBRARY} ${VSOMEIP_SD_LIBRARY} boost_system boost_thread)

add_executable(client2 client2.cpp)
target_link_libraries(client2 ${VSOMEIP_LIBRARY} ${VSOMEIP_SD_LIBRARY} boost_system boost_thread)

add_executable(server server.cpp)
target_link_libraries(server ${VSOMEIP_LIBRARY} ${VSOMEIP_SD_LIBRARY} boost_system boost_thread)
