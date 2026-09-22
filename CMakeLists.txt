cmake_minimum_required(VERSION 3.16)
project(Proiect_Practica)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 QUIET COMPONENTS Widgets)
if(Qt6_FOUND)
    set(QT_LIBS Qt6::Widgets)
else()
    find_package(Qt5 REQUIRED COMPONENTS Widgets)
    set(QT_LIBS Qt5::Widgets)
endif()

add_executable(Proiect_Practica WIN32
        src/main.cpp
        src/repository/StudentRepository.cpp
        src/ui/MainWindow.cpp
        src/ui/StudentDialog.cpp
)

target_include_directories(Proiect_Practica PRIVATE
        src
        src/model
        src/repository
        src/ui
)

target_link_libraries(Proiect_Practica PRIVATE ${QT_LIBS})

if(MSVC)
    target_compile_options(Proiect_Practica PRIVATE /W4)
else()
    target_compile_options(Proiect_Practica PRIVATE -Wall -Wextra -Wpedantic)
endif()
