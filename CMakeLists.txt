cmake_minimum_required(VERSION 3.30)
project(assignment1 C)

set(CMAKE_C_STANDARD 99)

add_executable(assignment1
        DataStructures/FlightScheduling/main.c
        DataStructures/FlightScheduling/flight.c
        DataStructures/FlightScheduling/flight.h
        DataStructures/FlightScheduling/queue.c
        DataStructures/FlightScheduling/queue.h
        DataStructures/FlightScheduling/list.c
        DataStructures/FlightScheduling/list.h
        DataStructures/FlightScheduling/flightscheduling.c
)