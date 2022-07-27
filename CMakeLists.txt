project(Chat)

cmake_minimum_required(VERSION 3.21)

if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
endif (CMAKE_BUILD_TYPE EQUAL "DEBUG")

add_subdirectory(ChatClient)
add_subdirectory(ChatServer)
add_subdirectory(ChatNetworking)