cmake_minimum_required(VERSION 3.17)
project(idlequeue)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig)
find_package(Threads)

pkg_check_modules(GTKMM gtkmm-3.0)

link_directories(${GTKMM_LIBRARY_DIRS})

include_directories(${GTKMM_INCLUDE_DIRS})


add_executable(idlequeue
        main.cpp
        blocking_queue.h
        window.h
        )

target_link_libraries(idlequeue ${GTKMM_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

