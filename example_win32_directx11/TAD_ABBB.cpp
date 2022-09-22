#include "ABBB.h"

node* NewNode(key id, rrn rrn) {
    node* New;
    New = (node*)malloc(sizeof(node));
    New->bal = 1;
    New->id = id;
    New->rrn = rrn;
    New->left = NULL;
    New->right = NULL;
    New->root = NULL;
    return New;
}

int ABBB::height(avl N)
{
    if (N == NULL)
        return 0;
    return N->bal;
}

int ABBB::maximo(int a, int b)
{
    return (a > b) ? a : b;
}

int ABBB::getBalance(avl N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

avl ABBB::rotationRight(avl r) {

    avl x = r->left;
    avl T2 = x->right;

    // Perform rotation
    x->right = r;
    r->left = T2;

    // Update heights
    r->bal = max(height(r->left), height(r->right)) + 1;
    x->bal = max(height(x->left), height(x->right)) + 1;

    // Return new root
    return x;
}

avl ABBB::rotationLeft(avl r) {
    avl y = r->right;
    avl T2 = y->left;

    // Perform rotation
    y->left = r;
    r->right = T2;

    //  Update heights
    r->bal = max(height(r->left), height(r->right)) + 1;
    y->bal = max(height(y->left), height(y->right)) + 1;

    // Return new root
    return y;
}

avl ABBB::insert(avl r, int id, int rrn) {
    if (r == NULL)
        return(NewNode(id, rrn));

    if (id < r->id)
        r->left = insert(r->left, id, rrn);
    else if (id > r->id)
        r->right = insert(r->right, id, rrn);
    else // Equal keys not allowed
        return r;

    /* 2. Update height of this ancestor node */
    r->bal = 1 + max(height(r->left), height(r->right));

    /* 3. Get the balance factor of this ancestor
          node to check whether this node became
          unbalanced */
    int balance = getBalance(r);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && id < r->left->id)
        return rotationRight(r);

    // Right Right Case
    if (balance < -1 && id > r->right->id)
        return rotationLeft(r);

    // Left Right Case
    if (balance > 1 && id > r->left->id)
    {
        r->left = rotationLeft(r->left);
        return rotationRight(r);
    }

    // Right Left Case
    if (balance < -1 && id < r->right->id)
    {
        r->right = rotationRight(r->right);
        return rotationLeft(r);
    }

    /* return the (unchanged) node pointer */
    return r;
}


avl ABBB::AVLmax(avl r) {
    if (r->right == NULL) return r;
    return AVLmax(r);
}

avl ABBB::minValueNode(avl r) {
    avl current = r;

    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;

    return current;
}

int ABBB::searchId(avl r, key id)
{
    if (r == NULL)
        return -1;
    else if (r->id == id)
        return r->rrn;
    else if (id < r->id)
        return searchId(r->left, id);
    else if (id > r->id)
        return searchId(r->right, id);
}

avl ABBB::balancing_left(avl r, int* decreased_height) {
    if (r->bal == -1) { //estou removendo do lado esquerdo, isso implica  que o meu fator de balanceamento fica 0
        r->bal = 0;
        r->left->bal = 0;
    }
    else if (r->bal == 0) {
        r->bal = 1;
        *decreased_height = 0;
    }
    else { //altura foi altera e desbalanceou
    //em qual caso estou? (vou fazer rotação simples ou dupla?)
        if (r->right->bal >= 0) {
            r = rotationLeft(r);
            if (r->bal == 0) {
                r->left->bal = 1;
                r->bal = -1;
                *decreased_height = 0;
            }
            else { //r->bal == 1;
                r->left->bal = 0;
                r->bal = 0;
            }
        }
        else {
            r->right = rotationRight(r);
            r = rotationLeft(r);
            if (r->bal == -1) {
                r->left->bal = 0;
                r->right->bal = 1;
            }
            else if (r->bal == 1) {
                r->left->bal = -1;
                r->right->bal = 0;
            }
            else {
                r->left->bal = 0;
                r->right->bal = 0;
            }
            r->bal = 0;
        }
    }
    return r;
}


avl ABBB::balancing_right(avl r, int* decreased_height) {
    if (r->bal == 1) { //estou removendo do lado direito, isso implica  que o meu fator de balanceamento fica 0
        r->bal = 0;
        r->left->bal = 0;
    }
    else if (r->bal == 0) {
        r->bal = -1;
        *decreased_height = 0;
    }
    else { //altura foi altera e desbalanceou
    //em qual caso estou? (vou fazer rotação simples ou dupla?)
        if (r->left->bal <= 0) {
            r = rotationRight(r);
            if (r->bal == 0) {
                r->right->bal = -1;
                r->bal = 1;
                *decreased_height = 0;
            }
            else { //r->bal == -1;
                r->right->bal = 0;
                r->bal = 0;
            }
        }
        //CASOS ROTAÇÃO DUPLA
        else {
            r->left = rotationLeft(r);
            r = rotationRight(r);
            if (r->bal == -1) {
                r->left->bal = 0;
                r->right->bal = 1;
            }
            else if (r->bal == 1) {
                r->left->bal = 0;
                r->right->bal = 1;;
            }
            else {
                r->left->bal = 0;
                r->right->bal = 0;
            }
            r->bal = 0;
        }
    }
    return r;
}


avl ABBB::search_remove(avl r, avl node_key, int* decreased_height) {
    node* aux;
    if (r->right != NULL) {
        r->right = search_remove(r, node_key, decreased_height);
        if (*decreased_height == 1) {
            r = balancing_right(r, decreased_height);
        }
    }
    else {
        node_key->id = r->id;
        aux = r;
        r = r->left;
        free(aux);
        *decreased_height = 1;
    }
    return r;
}


avl ABBB::deleteNode(avl root, int id)
{
    // STEP 1: PERFORM STANDARD BST DELETE

    if (root == NULL)
        return root;

    // If the key to be deleted is smaller than the
    // root's key, then it lies in left subtree
    if (id < root->id)
        root->left = deleteNode(root->left, id);

    // If the key to be deleted is greater than the
    // root's key, then it lies in right subtree
    else if (id > root->id)
        root->right = deleteNode(root->right, id);

    // if key is same as root's key, then This is
    // the node to be deleted
    else
    {
        // node with only one child or no child
        if ((root->left == NULL) || (root->right == NULL))
        {
            avl temp = root->left ? root->left : root->right;

            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // One child case
                root = temp; // Copy the contents of
                               // the non-empty child
            free(temp);
        }
        else
        {
            // node with two children: Get the inorder
            // successor (smallest in the right subtree)
            avl temp = minValueNode(root->right);

            // Copy the inorder successor's data to this node
            root->id = temp->id;

            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->id);
        }
    }

    // If the tree had only one node then return
    if (root == NULL)
        return root;

    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->bal = 1 + max(height(root->left), height(root->right));

    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
    // check whether this node became unbalanced)
    int balance = getBalance(root);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rotationRight(root);

    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left = rotationLeft(root->left);
        return rotationRight(root);
    }

    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return rotationLeft(root);

    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rotationRight(root->right);
        return rotationRight(root);
    }

    return root;
}


void ABBB::printInorder(avl node)
{
    if (node == NULL)
        return;

    /* first recur on left child */
    printInorder(node->left);

    /* then print the data of node */
    cout << node->id << " ";

    /* now recur on right child */
    printInorder(node->right);
}
