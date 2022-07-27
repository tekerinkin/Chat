cmake_minimum_required(VERSION 3.21)
project(ChatServer)

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
endif (CMAKE_BUILD_TYPE EQUAL "DEBUG")

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ChatNetworking
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        ChatNetworking
        )