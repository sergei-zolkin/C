//
//
//

#ifndef list
#define list

#include "flight.h"

// Linked list implementation will be responsible for Flight* allocations and deallocation

// A structure to represent a list node with item type Flight*
struct ListNode
{
    Flight *flightInformation;
    struct ListNode *next;
};

// A structure to keep a linked list record
struct ListRecord
{
    struct ListNode *head;
    struct ListNode *tail;
    int length;
};

// Typedef for list record pointer
typedef struct ListRecord *List;

/* A function to create a list, it allocates an address in
 * memory for ListRecord and creates an empty linked list with dummy
 * node by using MakeEmptyList function.*/
List CreateList(void);

// Makes a list empty with dummy node.
void MakeEmptyList(List);

// Insert a node to a specific position of the linked list
void InsertList(List, int, Flight*);

// Insert to the end of linked list
void InsertToListEnd(List, Flight*);

//Insert to the beginning of the linked list
void InsertToListHead(List, Flight*);

//Insert to linked list by ready time order (Ascending, smallest ready time at Head)
void InsertListOrderedByReadyTime(List l, Flight*);

//To delete first node from the linked list and return the address of deleted nodes item Flight
Flight* DeleteFirstNode(List);

//To Delete a flight from the list
void DeleteFromList(List, Flight*);

//To get the length of the linked list
int ListSize(List);

//To get the Flights address at the head of the linked list, without removing the node!
Flight* HeadOfList(List);

//To get the Flight address at the tail of the linked list, without removing the node!
Flight* TailOfList(List);

//To check of list is empty, 1 for empty, 0 for not
int IsEmptyList(List);

//To print the content of the linked list
void DisplayList(List);

//To free all the nodes and delete the linked list
void deleteList(List);


#endif //
