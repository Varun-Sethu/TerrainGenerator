cmake_minimum_required( VERSION 2.8 FATAL_ERROR )
set( CMAKE_VERBOSE_MAKEFILE ON )
set (DEBUG FALSE)


project(Terrain)
enable_language(CXX)
set (CMAKE_CXX_STANDARD 17)

# If debug is on then mark the application with -ggdb so we can debug it in GDB
if(DEBUG)
    set(CMAKE_CXX_FLAGS "-ggdb")
endif()


get_filename_component(APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${APP_PATH}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${APP_PATH}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${APP_PATH}/bin)

include_directories("${APP_PATH}/include")

set(SRC_FILES 
    "main.cpp"
    "include/map.h"
    "include/perlin.h"
    "include/biomes.h"
)


add_executable(terrain ${SRC_FILES})
target_link_libraries(terrain -lsfml-graphics -lsfml-window -lsfml-system)


