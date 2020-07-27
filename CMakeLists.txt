cmake_minimum_required(VERSION 3.0.0)
project(project1 VERSION 0.1.0)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIB REQUIRED gtk+-3.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -lpthread -lm -ldl -s -export-dynamic")

add_executable(project1 src/main.c src/main_window.c src/playback.c)

target_link_libraries(project1 ${LIB_LIBRARIES})
target_include_directories(project1 PUBLIC ${LIB_INCLUDE_DIRS})
target_compile_options(project1 PUBLIC ${LIB_CFLAGS_OTHER})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
