//
//
//

#ifndef FLIGHTSIM_FLIGHT_H
#define FLIGHTSIM_FLIGHT_H

#include <stdio.h>
#include <stdlib.h>

// A structure to reference the flights in the simulation.
typedef struct {
    char flightType;
    int readyTime;
    int serviceTime;
    int serviceStartTime;
    int runwayNumber;
    char airline[3];
    char destination[4];
}Flight;

//A function to return the address of a dynamically allocated random flight
Flight* createRandomFlight(int, int);

//A function to print a single flights information
void printFlightInformation(Flight*);

#endif
