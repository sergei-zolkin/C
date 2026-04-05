#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avltree.h"

struct AVLNode* loadFlights(char *fileName);
struct AVLNode* insertFlight(struct AVLNode* t, char *destination, char *airline, char type, char *date, int miles);
void infoFlights(struct AVLNode* t, char *destination);
void displayFlights(struct AVLNode* t);
struct Flight* findFurthestDestination(struct AVLNode* t);
void FurthestDestinations(struct AVLNode* t, struct Flight* highestMilesFlight);

int main (int argc, char **argv){

    struct AVLNode* myTree = NULL;
    int menuOption, flag = 1;

    if(argc == 1){
        printf("Please provide a file name: ");
        return 0;
    }

    myTree = loadFlights(argv[1]);

    printf("Welcome to data analysis @ Istanbul Airport\n");

    while (flag){
        printf("\n-------Menu-------\n"
               "(1)Insert flight\n"
               "(2)Display the full index of flights\n"
               "(3)Display the details of the flight\n"
               "(4)Display the furthest destination\n"
               "(5)Exit\n"
               "Enter your option:");
        scanf("%d", &menuOption);

        switch (menuOption) {

            case 1:{
                char destination[50], airline[50], type, date[11];
                int miles;

                printf("Please enter flight details:\n");
                printf("Destination: ");
                scanf(" %s", destination);
                printf("Airline: ");
                scanf(" %s", airline);
                printf("Type: ");
                scanf(" %c", &type);
                printf("Date: ");
                scanf(" %s", date);
                printf("Miles: ");
                scanf("%d", &miles);

                myTree = insertFlight(myTree, destination, airline, type, date, miles);
                printf("%s %s has been added successfully.\n", airline, destination);
                break;
            }
            case 2:{
                displayFlights(myTree);
                break;
            }
            case 3:{
                char destination[50];
                printf("Destination: ");
                scanf(" %s", destination);
                infoFlights(myTree,destination);
                break;
            }
            case 4:{
                printf("Detailed information of the furthest destination:\n");
                FurthestDestinations(myTree, findFurthestDestination(myTree));
                break;
            }
            case 5: {
                flag = 0;
                break;
            }
            default:{
                printf("Option %d can't be recognized.\n", menuOption);
            }
        }
    }

    myTree = MakeEmptyTree(myTree);
    return 0;
}

struct AVLNode* insertFlight(struct AVLNode* t, char *destination, char *airline, char type, char *date, int miles) {
    struct Flight *newFlight = (struct Flight *)malloc(sizeof(struct Flight));
    if (newFlight == NULL) {
        printf("Memory allocation failed!");
        exit(1);
    }

    strcpy(newFlight->destination, destination);
    strcpy(newFlight->airline, airline);
    strcpy(newFlight->date, date);

    newFlight->miles = miles;
    newFlight->type = type;

    newFlight->next = NULL;

    t = InsertFlight(t, newFlight);
    return t;
}

struct AVLNode* loadFlights(char *fileName){
    FILE *file = fopen(fileName, "r");

    if (file == NULL) {
        printf("Unable to access the file!");
        exit(1);
    }


    struct AVLNode *t = CreateTree();

    char destination[50], airline[50], type, date[11];
    int miles;

    while (fscanf(file, "%3[^;];%2[^;];%c;%11[^;];%d\n", destination, airline, &type, date, &miles) == 5) {
        t = insertFlight(t, destination, airline, type, date, miles);
    }

    fclose(file);
    return t;
}

void displayFlights(struct AVLNode* t){
    DisplayTree(t);
}

/* The time complexity of this function is O(log n), because the FindDestination
 * function traverses the tree by comparing destinations of the flights, which is
 * what the tree itself is based on. There are no obvious ways to improve the
 * complexity of this algorithm, as it already uses the standard method of
 * traversing the tree. */
void infoFlights(struct AVLNode* t, char *destination){
    struct Flight *destinationFlight = FindDestination(t, destination);

    if (destinationFlight == NULL) {
        printf("There is no available flight whose destination is %s\n", destination);
    }
    while (destinationFlight != NULL) {
        PrintFlightDetails(destinationFlight);
        destinationFlight = destinationFlight->next;
    }
}

struct Flight* findFurthestDestination(struct AVLNode* t){
    if (t == NULL) return NULL;

    struct Flight *furthestFlight = t->data;
    struct Flight *temp = furthestFlight->next;
    // traverse through all flights in one node and update the furthest flight if you find a further flight
    while (temp != NULL) {
        if (temp->miles > furthestFlight->miles) {
            furthestFlight = temp;
        }
        temp = temp->next;
    }


    struct Flight *leftFurthestFlight = findFurthestDestination(t->left);
    struct Flight *rightFurthestFlight = findFurthestDestination(t->right);

    if (leftFurthestFlight != NULL && (leftFurthestFlight->miles > furthestFlight->miles)) {
        furthestFlight = leftFurthestFlight;
    }
    if (rightFurthestFlight != NULL && (rightFurthestFlight->miles > furthestFlight->miles)) {
        furthestFlight = rightFurthestFlight;
    }

    return furthestFlight;
}

/* compared to InfoFlights function, FurthestDestinations is considerably slower,
 * having the time complexity O(N), due to the fact that it visits every node
 * in the tree in order to find flights with the highest mileage, because the tree
 * is based on destination and not on the miles. The only way I could think of that
 * would make the program more efficient is to build a new tree based on miles, not
 * on destinations and use this new tree to find furthest destinations. */
void FurthestDestinations(struct AVLNode* t, struct Flight* highestMilesFlight){
    if (t == NULL) return;

    // traverse through all flights in one node and check whether it's the furthest or not
    struct Flight *currentFlight = t->data;
    while (currentFlight != NULL) {
        if (currentFlight->miles == highestMilesFlight->miles) {
            PrintFlightDetails(currentFlight);
        }
        currentFlight = currentFlight->next;
    }

    FurthestDestinations(t->left, highestMilesFlight);
    FurthestDestinations(t->right, highestMilesFlight);
}
