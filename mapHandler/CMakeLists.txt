cmake_minimum_required(VERSION 3.7)
project(urchinMapHandler)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 17)

add_definitions(-ffast-math)
include_directories(src ../common/src ../3dEngine/src ../physicsEngine/src ../soundEngine/src ../AIEngine/src)

file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
add_library(urchinMapHandler SHARED ${SOURCE_FILES})
target_link_libraries(urchinMapHandler urchinCommon urchin3dEngine urchinPhysicsEngine urchinSoundEngine urchinAIEngine -static-libstdc++)