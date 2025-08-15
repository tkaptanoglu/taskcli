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

add_executable(taskcli_test_unit_person
    tests/unit/person_unit_test.cpp
    src/person.cpp
    src/task.cpp
)

add_executable(taskcli_test_unit_person_manager
    tests/unit/person_manager_unit_test.cpp
    src/person_manager.cpp
    src/person.cpp
    src/task.cpp
)

add_executable(taskcli_test_unit_task
    tests/unit/task_unit_test.cpp
    src/task.cpp
    src/person.cpp
)
