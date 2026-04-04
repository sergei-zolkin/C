//
//
//

#include "queue.h"

Queue CreateQueue()
{
    Queue q = (Queue *)malloc(sizeof(Queue));
    if (q == NULL) {
        printf("Memory allocation failed!");
        exit(1);
    }

    MakeEmptyQueue(q);

    return q;
}

void MakeEmptyQueue(Queue q)
{
    q->front = (struct QueueNode *)malloc(sizeof(struct QueueNode));
    if (q->front == NULL) {
        printf("Memory allocation failed!");
        exit(1);
    }
    q->front->next = NULL;
    q->rear = q->front;
    q->size = 0;
}

void Enqueue(Flight* newFlight, Queue q)
{
    struct QueueNode *enqueueFlight = (struct QueueNode *)malloc(sizeof(struct QueueNode));
    if (enqueueFlight == NULL) {
        printf("Memory allocation failed!");
        exit(1);
    }

    enqueueFlight->flightInfo = newFlight;
    enqueueFlight->next = NULL;

    if (newFlight->flightType == 'E') enqueueFlight->priority = 3;
    else if (newFlight->flightType == 'D') enqueueFlight->priority = 2;
    else enqueueFlight->priority = 1;


    q->rear->next = enqueueFlight;
    q->rear = enqueueFlight;

    q->size += 1;
}

Flight* Dequeue(Queue q)
{
    if (IsEmptyQueue(q)) {
        printf("Can't dequeue from the empty queue!");
        exit(1);
    }

    struct QueueNode *dequeueFlight = q->front->next;
    Flight *flightInfo = dequeueFlight->flightInfo;

    q->front->next = dequeueFlight->next;
    if (dequeueFlight->next == NULL) {
        q->rear = q->front;
    }
    free(dequeueFlight);

    q->size -= 1;

    return flightInfo;
}

int IsEmptyQueue(Queue q)
{
    return QueueSize(q) == 0;
}

int QueueSize(Queue q)
{
    return q->size;
}

Flight* FrontOfQueue(Queue q)
{
    if (IsEmptyQueue(q)) {
        printf("Can't get the front of the empty queue!");
        exit(1);
    }

    return q->front->next->flightInfo;
}

Flight* RearOfQueue(Queue q)
{
    if (IsEmptyQueue(q)) {
        printf("Can't get the rear of the empty queue!");
        exit(1);
    }

    return q->rear->flightInfo;
}

void DisplayQueue(Queue q)
{
    struct QueueNode *tmp = q->front->next;
    printf("The content of the queue:\n");

    while (tmp != NULL) {
        printFlightInformation(tmp->flightInfo);
        tmp = tmp->next;
    }
}

void deleteQueue(Queue toDeleteList)
{
    struct QueueNode *tmp = toDeleteList->front;
    while (tmp != NULL) {
        struct QueueNode *deleteNode = tmp;
        tmp = tmp->next;
        free(deleteNode);
    }

    toDeleteList->front = NULL;
    toDeleteList->rear = NULL;
    toDeleteList->size = 0;
}
