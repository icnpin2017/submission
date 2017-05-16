cmake_minimum_required(VERSION 3.1)

if(CMAKE_CXX_COMPILER MATCHES "clang\\+\\+")
    set(CMAKE_CXX_FLAGS "-Weverything -Wno-c++98-compat -Wno-padded -Wno-sign-conversion ${CMAKE_CXX_FLAGS}")
else()
    set(CMAKE_CXX_FLAGS "-Wall ${CMAKE_CXX_FLAGS}")
endif()

set(CMAKE_CXX_FLAGS "-std=c++14 -Werror ${CMAKE_CXX_FLAGS}")

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS "-ggdb ${CMAKE_CXX_FLAGS}")
else()
    set(CMAKE_CXX_FLAGS "-O2 ${CMAKE_CXX_FLAGS}")
endif() 

find_package(LEMON REQUIRED)

include_directories(${LEMON_INCLUDE_DIRS})

add_executable(rec_mst mst.cpp)
add_executable(rec_spt spt.cpp)
add_executable(rec_mstt mstt.cpp)
add_executable(camh camh.cpp)
add_executable(eval eval.cpp)
add_executable(mcamh mcamh.cpp)
add_executable(mcamhle mcamh.cpp)
add_executable(stupid stupid.cpp)
add_executable(msttmax msttmax.cpp)
add_executable(msttthm10 msttthm10.cpp)

target_compile_definitions(mcamhle PRIVATE "-DMCAMLE")
