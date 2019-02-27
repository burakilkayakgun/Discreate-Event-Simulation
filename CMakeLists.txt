cmake_minimum_required(VERSION 2.8)
project(project_2)

add_compile_options(-std=c++11)

add_executable(project_2
        DiscreateEventSimulation.pdf main.cpp Passenger.cpp Passenger.h Employee.cpp Employee.h Event.cpp Event.h Port.cpp Port.h)
