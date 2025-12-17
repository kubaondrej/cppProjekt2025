cmake_minimum_required(VERSION 3.16)

project(CPPProjekt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Quick
    QuickControls2
)

qt_standard_project_setup()

qt_add_executable(appCPPProjekt
    main.cpp

    mapmodel.h mapmodel.cpp
    unitmodel.h unitmodel.cpp
    resources.qrc


)

qt_add_resources(appCPPProjekt "app_resources"
    PREFIX "/"
    FILES
        Main.qml
)

target_link_libraries(appCPPProjekt PRIVATE Qt6::Quick Qt6::QuickControls2)
