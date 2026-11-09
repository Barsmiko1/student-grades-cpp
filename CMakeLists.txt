cmake_minimum_required(VERSION 3.10)
project(StudentGrades VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build an optimized program unless another build type is chosen, so the
# speed test measures the same thing as the Makefile build.
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(grades
    main.cpp
    Person.cpp
    Students.cpp
    FileIO.cpp
    Console.cpp
    SpeedTest.cpp
)

if(MSVC)
    target_compile_options(grades PRIVATE /W4)
else()
    target_compile_options(grades PRIVATE -Wall -Wextra)
endif()

# Windows needs this library for the peak memory measurement.
if(WIN32)
    target_link_libraries(grades PRIVATE psapi)
endif()
