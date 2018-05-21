###############################################################
######                                                   ######
######			Author: Manuel P. J. (aka. manuoso)      ######
######                                                   ######
###############################################################

cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
project(clientUDP)

# Compiler specific options
 if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++11 -Wl,--no-as-needed)
 endif()

# Loading boost
if(WIN32)
	set(Boost_USE_STATIC_LIBS ON)
endif()
find_package(Boost REQUIRED COMPONENTS system date_time regex)

# Collect all sources
file(GLOB_RECURSE SRC_FILES "src/*.cpp" "include/*.h" "include/*.inl" )

# Add Executable
add_executable(${PROJECT_NAME} ${SRC_FILES})
include_directories(PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} pthread)

target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${Boost_LIBRARIES})
