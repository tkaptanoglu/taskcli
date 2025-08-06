cmake_minimum_required(VERSION 3.15)
project(taskcli)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_SOURCE_DIR}/include)

# Add all your source files here
add_executable(taskcli
    main.cpp
    src/task.cpp
    src/task_manager.cpp
    src/person.cpp
    src/person_manager.cpp
)
