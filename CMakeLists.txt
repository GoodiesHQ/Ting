cmake_minimum_required(VERSION 3.9)
project(Ting C)

set(CMAKE_C_STANDARD 99)
include_directories(inc)

add_definitions(-DTING_CAPTURE_IP_ONLY)

add_executable(Ting src/main.c inc/ting.h inc/ting/endian.h src/endian.c inc/ting/packet.h inc/ting/gre.h src/gre.c inc/ting/dns.h src/dns.c)