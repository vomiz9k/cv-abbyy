cmake_minimum_required(VERSION 3.16)
project(cv_task_1)
find_package(
    OpenCV
    REQUIRED
)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(
    cv_task_1
    main.cpp
)
target_link_libraries(
    cv_task_1
    ${OpenCV_LIBS}
)
