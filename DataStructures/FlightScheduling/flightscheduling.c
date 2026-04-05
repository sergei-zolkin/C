#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "list.h"
#include "queue.h"

// To parse input from program arguments
void parseInput(char **, int *, int *, int *, int *);

// To initialise, create and populate the flight list with random flight data
List createFlightList(int, int, int);

//To initialise queue of the arrived flights and runway availability data
int* initialiseSimulator(Queue*, int);

//To get the priority of a flight
int priority(Flight *);

//Adds the arrived flight into the priority queue
void newFlight(Flight*, Queue);

//To calculate the simulated clock
void nextEvent(Flight *, int*, Queue);

//Randomly assign an available runway to the flight
void serveFlight(Flight*, int*, int);

//To check whether the Runway is full
int fullRunway(int*, int);

//Print the statistical data of the simulation run
void reportStatistics(List, int, int, int);

//Clear dynamically allocated memories
void exitFromTheSimulation(List, Queue, int *);

int main(int argc, char *argv[])
{
    srand(time(0));

    if (argc != 5) {
        printf("Not enough command line arguments!");
        exit(1);
    }

    int noOfFlights = 0, noOfRunways = 0, maxReadyTime = 0, maxServiceTime = 0;


    parseInput(argv, &noOfFlights, &noOfRunways, &maxReadyTime, &maxServiceTime);
    List flightList = createFlightList(noOfFlights, maxReadyTime, maxServiceTime);

    // the final list that will store all the data
    List flightDataList = CreateList();
    // queue that stores all flights which have been assigned a runway and are in the proccess of taking off
    Queue servedFlights = CreateQueue();
    // queue that stores flight based on their ready time and priority
    Queue flightQueue;

    int *runways = initialiseSimulator(&flightQueue, noOfRunways);

    // keeps the count of served flights
    int servedNum = 0;
    int clock = 0;

    while (servedNum < noOfFlights) {

        if (IsEmptyQueue(flightQueue)) {
            newFlight(DeleteFirstNode(flightList), flightQueue);
        }

        Flight *flightReadyToBeServed = Dequeue(flightQueue);

        if (!IsEmptyQueue(servedFlights)) {
            // move the clock to the next event
            nextEvent(flightReadyToBeServed, &clock, servedFlights);
        }

        clock = flightReadyToBeServed->readyTime;

        if (fullRunway(runways, noOfRunways)) {
            // get the flight details of the flight which has been the first to get on a runway
            Flight* servedFlight = Dequeue(servedFlights);
            /* if next flight is ready, but there are no runways to serve it ->
            -> start of service will be the time when the runway is cleared
            else start of service will be the time when the flight is ready */
            if (flightReadyToBeServed->readyTime < (servedFlight->serviceStartTime + servedFlight->serviceTime)) {
                flightReadyToBeServed->serviceStartTime = servedFlight->serviceStartTime + servedFlight->serviceTime;
            }
            else {
                flightReadyToBeServed->serviceStartTime = flightReadyToBeServed->readyTime;
            }
            // clear the runway
            runways[servedFlight->runwayNumber - 1] = 1;
            // store the flight data
            InsertToListEnd(flightDataList, servedFlight);
        }
        else {
            flightReadyToBeServed->serviceStartTime = flightReadyToBeServed->readyTime;
        }


        serveFlight(flightReadyToBeServed, runways, noOfRunways);

        Enqueue(flightReadyToBeServed, servedFlights);

        servedNum++;

        // while there is a flight in the list that is ready before this flight takes off, enqueue it based on its priority
        while (!IsEmptyList(flightList) && HeadOfList(flightList)->readyTime < (flightReadyToBeServed->serviceStartTime + flightReadyToBeServed->serviceTime)) {
            newFlight(DeleteFirstNode(flightList), flightQueue);
        }

    }
    // make sure to store all flights in the data list in order to process them later
    while (!IsEmptyQueue(servedFlights)) {
        Flight *servedFlight = Dequeue(servedFlights);
        clock = servedFlight->serviceStartTime + servedFlight->serviceTime;
        InsertToListEnd(flightDataList, servedFlight);
    }

    reportStatistics(flightDataList, noOfRunways, noOfFlights, clock);
    deleteList(flightDataList);
    deleteQueue(servedFlights);
    exitFromTheSimulation(flightList, flightQueue, runways);
    return 0;
}

void parseInput(char *programArguments[], int *noOfFlights, int *noOfRunways, int *maxReadyTime, int *maxServiceTime){
    *noOfFlights = atoi(programArguments[1]);
    *noOfRunways = atoi(programArguments[2]);
    *maxReadyTime = atoi(programArguments[3]);
    *maxServiceTime = atoi(programArguments[4]);
}

List createFlightList(int numberOfFlights, int maxReadyTime, int maxServiceTime){
    List flightList = CreateList();

    for (int i = 0; i < numberOfFlights; i++) {
        Flight *flightInfo = createRandomFlight(maxReadyTime, maxServiceTime);
        InsertListOrderedByReadyTime(flightList, flightInfo);
    }

    return flightList;
}

int* initialiseSimulator(Queue* flightQueue, int noOfRunways){
    *flightQueue = CreateQueue();
    int *runways = (int *)malloc(sizeof(int) * noOfRunways);
    if (runways == NULL) {
        printf("Memory allocation failed!");
        exit(1);
    }

    for (int i = 0; i < noOfRunways; i++) {
        runways[i] = 1;
    }

    return runways;
}

// get the priority based on type of flight
int priority(Flight* flightData) {
    if (flightData->flightType == 'E') return 3;
    if (flightData->flightType == 'I') return 2;
    return 1;
}

void newFlight(Flight* flightData, Queue flightQueue){
    int enqueued = 0;

    for (int i = 0; i < QueueSize(flightQueue); i++) {

        Flight* flightInTheQueue = Dequeue(flightQueue);

        if (!enqueued && priority(flightData) > priority(flightInTheQueue)) {
            Enqueue(flightData, flightQueue);
            enqueued = 1;
        }

        Enqueue(flightInTheQueue, flightQueue);
    }

    if (!enqueued) {
        Enqueue(flightData, flightQueue);
    }
}

void nextEvent(Flight* flightData, int *clock, Queue servedFlights) {
    // check if any of the flights have been served before the new flight is ready, and update the clock accordingly
    for (int i = 0; i < QueueSize(servedFlights); i++) {
        Flight *prevFlight = Dequeue(servedFlights);

        int completeService = prevFlight->serviceStartTime + prevFlight->serviceTime;
        if (completeService > *clock && completeService < flightData->readyTime) {
            *clock = completeService;
        }

        Enqueue(prevFlight, servedFlights);
    }
}


int fullRunway(int *runways, int noOfRunways) {
    for (int i = 0; i < noOfRunways; i++) {
        if (runways[i] == 1) {
            return 0;
        }
    }

    return 1;
}

// int *runways is the runway array
void serveFlight(Flight* flightData, int* runways, int runwayNumber){
    int flag = 1;
    int runway = 0;
    int availableRunways = 0;
    int availableIndex = 0;

    for (int i = 0; i < runwayNumber; i++) {
        if (runways[i] == 1) {
            availableRunways++;
            availableIndex = i;
        }
    }

    if (availableRunways > 1) {
        while (flag) {
            runway = rand() % runwayNumber;

            if (runways[runway] == 1) {
                runways[runway] = 0;
                flag = 0;
            }
        }
    }
    else if (availableRunways == 1) {
        runway = availableIndex;
        runways[runway] = 0;
    }

    flightData->runwayNumber = runway + 1;
}

void reportStatistics(List flightsDataList, int noOfRunways, int numberOfFlights, int clockTime){
    int emergency = 0, international = 0, domestic = 0, totalWaitingTime = 0, maxWaitingTime = 0;
    char popularAirline[3];
    int numOfPopularAirline = 0;
    char popularDestination[4];
    int numOfPopularDestination = 0;
    int runways[noOfRunways] = {};

    struct ListNode *tmp = flightsDataList->head->next;

    while (tmp != NULL) {
        Flight *flightInfo = tmp->flightInformation;
        if (flightInfo->flightType == 'E') emergency++;
        else if (flightInfo->flightType == 'I') international++;
        else if (flightInfo->flightType == 'D') domestic++;

        runways[flightInfo->runwayNumber - 1] += 1;

        int currentWaitingTime = flightInfo->serviceStartTime - flightInfo->readyTime;
        struct ListNode *tmp2 = flightsDataList->head->next;
        int airlineCount = 0;
        int destinationCount = 0;
        while (tmp2 != NULL) {
            Flight *currFlightInfo = tmp2->flightInformation;

            if (currFlightInfo->airline[0] == flightInfo->airline[0]
                && currFlightInfo->airline[1] == flightInfo->airline[1]) {
                airlineCount++;
            }

            if (currFlightInfo->destination[0] == flightInfo->destination[0]
                && currFlightInfo->destination[1] == flightInfo->destination[1]
                && currFlightInfo->destination[2] == flightInfo->destination[2]) {
                destinationCount++;
            }
            tmp2 = tmp2->next;
        }

        if (airlineCount > numOfPopularAirline) {
            numOfPopularAirline = airlineCount;
            popularAirline[0] = flightInfo->airline[0];
            popularAirline[1] = flightInfo->airline[1];
            popularAirline[2] = '\0';
        }

        if (destinationCount > numOfPopularDestination) {
            numOfPopularDestination = destinationCount;
            popularDestination[0] = flightInfo->destination[0];
            popularDestination[1] = flightInfo->destination[1];
            popularDestination[2] = flightInfo->destination[2];
            popularDestination[3] = '\0';
        }

        totalWaitingTime += currentWaitingTime;
        if (currentWaitingTime > maxWaitingTime) {
            maxWaitingTime = currentWaitingTime;
        }
        tmp = tmp->next;
    }


    printf("****************Report****************\n"
           "The number of runways: %d\n"
           "The number of flights: %d\n"
           "Number of flights for each flight type:\n"
           "      Emergency: %d\n"
           "      International: %d\n"
           "      Domestic: %d\n"
           "Number of flights for each runway:\n", noOfRunways, numberOfFlights, emergency, international, domestic);
    for (int i = 0; i < noOfRunways; i++) {
        printf("      Runway %d: %d\n", i + 1, runways[i]);
    }
    printf("Completion time: %d\n"
           "Average time spent in the queue: %.1lf\n"
           "Maximum waiting time: %d\n"
           "Popular airline: %s\n"
           "Popular Destination: %s\n", clockTime, (float)totalWaitingTime / (float)numberOfFlights,
           maxWaitingTime, popularAirline, popularDestination);
}

void exitFromTheSimulation(List flightList, Queue flightQueue, int *flightData){
    deleteList(flightList);
    deleteQueue(flightQueue);
    free(flightData);
}