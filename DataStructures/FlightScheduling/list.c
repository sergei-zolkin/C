//
//
//

#include "list.h"

List CreateList()
{
    List l = (List *)malloc(sizeof(List));
    if (l == NULL) {
        printf("Memory allocation failed!");
        exit(1);
    }
    MakeEmptyList(l);

    return l;
}

void MakeEmptyList(List l)
{
    l->head = (struct ListNode *)malloc(sizeof(struct ListNode));
    if (l->head == NULL) {
        printf("Memory allocation failed!");
        exit(1);
    }
    l->head->next = NULL;
    l->tail = l->head;
    l->length = 0;
}

void InsertList(List l, int pos, Flight* data)
{
    if (pos == ListSize(l) + 1) {
        InsertToListEnd(l, data);
    }
    else if (pos == 1) {
        InsertToListHead(l, data);
    }
    else {
        struct ListNode *newFlight = (struct ListNode *)malloc(sizeof(struct ListNode));
        if (newFlight == NULL) {
            printf("Memory allocation failed!");
            exit(1);
        }

        newFlight->flightInformation = data;

        struct ListNode *tmp = l->head;
        for (int i = 1; i < pos; i++) {
            tmp = tmp->next;
        }

        newFlight->next = tmp->next;
        tmp->next = newFlight;

        l->length += 1;
    }
}

void InsertToListEnd(List l, Flight* flight){
    struct ListNode *newFlight = (struct ListNode *)malloc(sizeof(struct ListNode));
    if (newFlight == NULL) {
        printf("Memory allocation failed!");
        exit(1);
    }

    newFlight->flightInformation = flight;
    newFlight->next = NULL;

    l->tail->next = newFlight;
    l->tail = newFlight;

    l->length += 1;
}

void InsertToListHead(List l, Flight* flight){
    struct ListNode *newFlight = (struct ListNode *)malloc(sizeof(struct ListNode));
    if (newFlight == NULL) {
        printf("Memory allocation failed!");
        exit(1);
    }

    newFlight->flightInformation = flight;
    newFlight->next = l->head->next;
    l->head->next = newFlight;

    l->length += 1;
}

void InsertListOrderedByReadyTime(List l, Flight* newFlight){
    int insertPosition = 1;

    struct ListNode *tmp = l->head->next;
    while (tmp != NULL) {
        if (tmp->flightInformation->readyTime >= newFlight->readyTime) break;
        insertPosition++;
        tmp = tmp->next;
    }

    InsertList(l, insertPosition, newFlight);
}

Flight* DeleteFirstNode(List l){
    if (IsEmptyList(l)) {
        printf("Can not delete the first node from the empty list!");
        exit(1);
    }

    struct ListNode *deleteNode = l->head->next;

    Flight *flightInfo = deleteNode->flightInformation;

    l->head->next = deleteNode->next;
    free(deleteNode);

    l->length -= 1;
    return flightInfo;
}

void DeleteFromList(List l, Flight* v)
{
    if (IsEmptyList(l)) {
        printf("Can't delete from the empty list!");
        exit(1);
    }

    struct ListNode *tmp = l->head;

    while (tmp->next != NULL && tmp->next->flightInformation != v) {
        tmp = tmp->next;
    }

    if (tmp->next == NULL) {
        printFlightInformation(v);
        printf("Is not in the list!");
        exit(1);
    }

    struct ListNode *deleteNode = tmp->next;
    tmp->next = deleteNode->next;

    if (deleteNode->next == NULL) {
        l->tail = tmp;
    }

    free(deleteNode);
    l->length -= 1;
}

int IsEmptyList(List l)
{
    return ListSize(l) == 0;
}

int ListSize(List l)
{
    return l->length;
}

Flight* HeadOfList(List l)
{
    if (IsEmptyList(l)) {
        printf("Can't get the head from the empty list!");
        exit(1);
    }

    return l->head->next->flightInformation;
}

Flight* TailOfList(List l)
{
    if (IsEmptyList(l)) {
        printf("Can't get the tail from the empty list!");
        exit(1);
    }

    return l->tail->flightInformation;
}

void DisplayList(List l)
{
    struct ListNode *tmp = l->head->next;
    printf("The content of the list:\n");

    while (tmp != NULL) {
        printFlightInformation(tmp->flightInformation);
        tmp = tmp->next;
    }
}

void deleteList(List toDeleteList)
{
    struct ListNode *tmp = toDeleteList->head->next;
    while (tmp != NULL) {
        struct ListNode *deleteNode = tmp;
        tmp = tmp->next;
        free(deleteNode);
    }

    toDeleteList->head = NULL;
    toDeleteList->tail = NULL;
    toDeleteList->length = 0;
}
