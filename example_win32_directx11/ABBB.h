#ifndef ABBB_HPP
#define ABBB_HPP

#include <iostream>
#include <fstream>

using namespace std;

typedef struct IndexItem {
    int id;
    int rrn;
} item;


typedef int rrn;
typedef int key;

typedef struct node {
    int bal;
    int id;
    int rrn;
    struct node* root;
    struct node* left;
    struct node* right;
} node;

typedef node* avl;

class ABBB {
public:
    avl root = NULL;
    int height(avl N);
    int maximo(int a, int b);
    int getBalance(avl N);
    avl rotationRight(avl r);
    avl rotationLeft(avl r);
    avl insertAVL(node* r, node* novo, int* increased_height);
    avl insert(avl r, key id, rrn rrn);
    avl AVLmax(avl r);
    avl AVLmin(avl r);
    avl deleteNode(avl root, int id);
    avl minValueNode(avl r);
    avl balancing_left(avl r, int* decreased_height);
    avl balancing_right(avl r, int* decreased_height);
    avl search_remove(avl r, avl node_key, int* decreased_height);
    avl removeAVL(avl r, key id, int* decreased_height);
    avl remove(avl r, key id);
    void printInorder(avl node);
    int ABBB::searchId(avl r, key id);

};

#endif
