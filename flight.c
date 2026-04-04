//
//
//

#include "flight.h"

//This function will create a dynamically allocated flight with random values and return its address.
Flight * createRandomFlight(int maxReadyTime, int maxServiceTime){
    Flight *flightInfo = (Flight *)malloc(sizeof(Flight));
    if (flightInfo == NULL) {
        printf("Memory allocation failed!");
        exit(1);
    }

    flightInfo->runwayNumber = 0;
    flightInfo->serviceStartTime = 0;

    flightInfo->readyTime = rand() % maxReadyTime + 1;
    flightInfo->serviceTime = rand() % maxServiceTime + 1;

    char flightTypes[3] = "EID";
    flightInfo->flightType = flightTypes[rand() % 3];

    FILE *file = fopen("Airline_Destination_File.txt", "r");
    if (file == NULL) {
        printf("Unable to access the file!");
        exit(1);
    }

    char header[255];
    fgets(header, sizeof(header), file);

    char airlines[38][3];
    char destinations[38][4];
    for (int i = 0; i < 38; i++) {
        fscanf(file, "%2s %3s\n", airlines[i], destinations[i]);
    }

    int airlineNumber = rand() % 38;
    int destinationNumber = rand() % 38;

    for (int i = 0; i < 3; i++) {
        flightInfo->airline[i] = airlines[airlineNumber][i];
    }
    for (int j = 0; j < 4; j++) {
        flightInfo->destination[j] = destinations[destinationNumber][j];
    }
    return flightInfo;
}

//This function gets a flights address and prints its values.
void printFlightInformation(Flight* flight){
    if(flight != NULL){
        printf("%c %2d %2d %2d %2d %3s %4s\n", flight->flightType, flight->readyTime, flight->serviceTime, flight->serviceStartTime, flight->runwayNumber, flight->airline, flight->destination);
    }
}
