cmake_minimum_required(VERSION 3.16)
project(Proiect_Practica)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Proiect_Practica
        main.cpp
        StudentRepository.cpp
)

if(MSVC)
    target_compile_options(Proiect_Practica PRIVATE /W4)
else()
    target_compile_options(Proiect_Practica PRIVATE -Wall -Wextra -Wpedantic)
endif()
