cmake_minimum_required(VERSION 3.16)
project(cv_task_2)
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

find_package(
    OpenCV
    REQUIRED
)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(
    cv_task_2
    main.cpp
)
target_link_libraries(
    cv_task_2
    ${OpenCV_LIBS}
)
