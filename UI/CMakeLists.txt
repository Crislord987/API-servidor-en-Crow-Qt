cmake_minimum_required(VERSION 3.16)
project(UI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Buscar Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

# Habilitar MOC automático
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Crear ejecutable
add_executable(UI
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# Enlazar librerías Qt6
target_link_libraries(UI
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
)
