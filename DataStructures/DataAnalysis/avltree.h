#ifndef UNTITLED14_AVLTREE_H
#define UNTITLED14_AVLTREE_H

// data structure definition
struct Flight {
    char destination[50],airline[50],type,date[11];
    int miles;
    struct Flight* next;
};

struct AVLNode {
    struct Flight* data;
    struct AVLNode *left, *right;
    int height;
};

// avl tree functionalities
struct AVLNode* CreateTree(void);//Creates an empty tree
struct AVLNode* MakeEmptyTree(struct AVLNode* t);//Iterates over all the nodes inside avl tree and frees the nodes to empty the tree.
struct AVLNode* InsertFlight(struct AVLNode* t, struct Flight* flight);//Inserts new flight.
struct AVLNode* SingleRotateWithRight(struct AVLNode* t);
struct AVLNode* DoubleRotateWithRight(struct AVLNode* t);
struct AVLNode* SingleRotateWithLeft(struct AVLNode* t);
struct AVLNode* DoubleRotateWithLeft(struct AVLNode* t);
struct Flight* FindDestination(struct AVLNode *t, char *destination);//Searches to find a flight with provided destination, function returns the address of flight structure.
void DisplayTree(struct AVLNode* t);//Traverses the tree to print flights by alphabetical order.
void PrintFlightDetails(struct Flight* flight);//Gets a flight structure and prints its data.
int Max(int x, int y);//Compares two integer and returns greater value.
int AVLTreeHeight(struct AVLNode* t);//Returns the height of the current node.

#endif // UNTITLED14_AVLTREE_H
