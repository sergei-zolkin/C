#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avltree.h"


struct AVLNode* CreateTree(void){
    return NULL;
}

struct AVLNode* MakeEmptyTree(struct AVLNode* t)
{
    if (t != NULL) {
        MakeEmptyTree(t->left);
        MakeEmptyTree(t->right);
        // free memory allocated for flight details
        struct Flight *flightData = t->data;
        while (flightData != NULL) {
            struct Flight *deleteData = flightData;
            flightData = flightData->next;
            free(deleteData);
        }

        free(t);
    }
    return t;
}

struct AVLNode* InsertFlight(struct AVLNode* t, struct Flight* flight){
    if (t == NULL) {
        t = (struct AVLNode *)malloc(sizeof(struct AVLNode));
        if (t == NULL) {
            printf("Memory allocation failed!");
            exit(1);
        }

        t->data = flight;
        t->left = t->right = NULL;
        t->height = 0;
    }
    else if (strcmp(flight->destination, t->data->destination) < 0) {
        t->left = InsertFlight(t->left, flight);
        /* if the tree is heavy on the left */
        if (AVLTreeHeight(t->left) - AVLTreeHeight(t->right) == 2) {
            /* Left Left */
            if (strcmp(flight->destination, t->left->data->destination) < 0) {
                t = SingleRotateWithLeft(t);
            }
            /* Left Right */
            else {
                t = DoubleRotateWithLeft(t);
            }
        }
    }
    else if (strcmp(flight->destination, t->data->destination) > 0) {
        t->right = InsertFlight(t->right, flight);
        /* if the tree is heavy on the right */
        if (AVLTreeHeight(t->right) - AVLTreeHeight(t->left) == 2) {
            /* Right Right */
            if (strcmp(flight->destination, t->right->data->destination) > 0) {
                t = SingleRotateWithRight(t);
            }
            /* Right Left */
            else {
                t = DoubleRotateWithRight(t);
            }
        }
    }
    /* the flight has the same destination as the current node */
    else {
        struct Flight *tmp = t->data;

        while (tmp->next != NULL) {
            tmp = tmp->next;
        }

        tmp->next = flight;
    }

    t->height = Max(AVLTreeHeight(t->left), AVLTreeHeight(t->right)) + 1;
    return t;
}

//right rotation
struct AVLNode*  SingleRotateWithRight(struct AVLNode*  k2)
{
    struct AVLNode *root = k2;

    struct AVLNode *newRoot = root->right;
    root->right = newRoot->left;
    newRoot->left = root;

    root->height = Max(AVLTreeHeight(root->left), AVLTreeHeight(root->right)) + 1;
    newRoot->height = Max(AVLTreeHeight(newRoot->right), root->height) + 1;

    return newRoot;
}

//left rotation
struct AVLNode*  SingleRotateWithLeft(struct AVLNode*  k1)
{
    struct AVLNode *root = k1;

    struct AVLNode *newRoot = root->left;
    root->left = newRoot->right;
    newRoot->right = root;

    root->height = Max(AVLTreeHeight(root->left), AVLTreeHeight(root->right)) + 1;
    newRoot->height = Max(AVLTreeHeight(newRoot->left), root->height) + 1;

    return newRoot;
}

//left and right rotation
struct AVLNode*  DoubleRotateWithLeft(struct AVLNode*  k3)
{
    k3->left = SingleRotateWithRight(k3->left);
    return SingleRotateWithLeft(k3);
}

// right and left rotation
struct AVLNode*  DoubleRotateWithRight(struct AVLNode* k3)
{
    k3->right = SingleRotateWithLeft(k3->right);
    return SingleRotateWithRight(k3);
}

int Max(int x, int y)
{
    if (x >= y) return x;
    return y;
}

int AVLTreeHeight(struct AVLNode* t)
{
    if (t == NULL) return -1;
    return t->height;
}

void DisplayTree(struct AVLNode* t)
{
    if (t != NULL) {
        DisplayTree(t->left);

        struct Flight *tmp = t->data;
        while (tmp != NULL) {
            PrintFlightDetails(tmp);
            tmp = tmp->next;
        }
        DisplayTree(t->right);
    }
}

void PrintFlightDetails(struct Flight* flight){
    printf("%s %s %c %s %d\n", flight->destination, flight->airline, flight->type, flight->date, flight->miles);
}

struct Flight* FindDestination(struct AVLNode *t, char *destination){
    struct AVLNode *tmp = t;

    while (tmp != NULL && strcmp(tmp->data->destination, destination) != 0) {
        if (strcmp(destination, tmp->data->destination) < 0) {
            tmp = tmp->left;
        }
        else {
            tmp = tmp->right;
        }
    }

    // return NULL if a flight with this destination is not in the list
    if (tmp == NULL) {
        return NULL;
    }
    return tmp->data;
}