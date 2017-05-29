cmake_minimum_required(VERSION 3.4.1)

project (Framework)

# Add engine
add_subdirectory(../engine "${CMAKE_CURRENT_BINARY_DIR}/engine_build")
include_directories(../engine/source)

include_directories( ../libs/pugixml/src )
include_directories(../libs/freetype/include)

file(GLOB FRAMEWORK_SRC "source/*.cpp")
add_executable(framework ${FRAMEWORK_SRC})

target_link_libraries(framework shadow-lib)