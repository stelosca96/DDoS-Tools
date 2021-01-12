cmake_minimum_required(VERSION 3.16)
project(DDoS_Tools)
find_library(TINS_LIBRARY tins)
find_package(nlohmann_json 3.2.0 REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ltins -pthread")

add_executable(DDoS_Tools main.cpp SynFlood.cpp SynFlood.h DnsAmplification.cpp DnsAmplification.h)

target_link_libraries(DDoS_Tools "${TINS_LIBRARY}" nlohmann_json::nlohmann_json)
