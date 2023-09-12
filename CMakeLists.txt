cmake_minimum_required(VERSION 3.5)

project(QuadtreeVis LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.3.1/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Widgets Core)
qt_standard_project_setup()

file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.h
    src/*.ui

)


add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt6::Widgets


)

target_include_directories(${PROJECT_NAME} PUBLIC



)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
