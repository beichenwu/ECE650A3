cmake_minimum_required(VERSION 2.8)
project(a3-ece650)

set (CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/run)

add_executable(a3-ece650 a3-ece650.cpp)
add_executable(a2-ece650 a2-ece650.cpp)
add_executable(rgen rgen.cpp)

install(PROGRAMS a1-ece650.py support_functions.py intersect.py DESTINATION bin)
install(TARGETS a2-ece650 a3-ece650 RUNTIME DESTINATION bin)


