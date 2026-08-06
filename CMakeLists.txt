cmake_minimum_required(VERSION 3.14)
project(parley)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

include(FetchContent)


FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG 0.8.0 
)

FetchContent_MakeAvailable(yaml-cpp)

add_executable(fuzzer src/main.cpp src/parser.cpp src/engine.cpp harnesses/dummy_target.cpp)

target_include_directories(fuzzer PRIVATE include)

# Link the yaml-cpp library to our fuzzer
target_link_libraries(fuzzer PRIVATE yaml-cpp)