cmake_minimum_required(VERSION 3.14)

project(dha VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

find_package(Qt6 REQUIRED COMPONENTS Core Quick QuickControls2 Gui)
qt_standard_project_setup()

include_directories(".")

set(SRC_LIST DH.cpp main.cpp backend.cpp dhworker.cpp matrix4d.cpp)
set(HEADERS_LIST DH.h backend.h dhworker.h matrix4d.h)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

qt_add_executable(${PROJECT_NAME}
    ${SRC_LIST}
    ${HEADERS_LIST}
)

qt_add_qml_module(${PROJECT_NAME}
    URI dha_qml
    QML_FILES
        main.qml
        SOURCES programm.h programm.cpp
    RESOURCES
        icons/upload.svg
)

qt_add_resources(${PROJECT_NAME}
    qtquickcontrols2.conf
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::QuickControls2 Qt6::Gui)
