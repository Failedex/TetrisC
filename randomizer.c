#include "randomizer.h"
#include <stdlib.h>

static LinkedList bag;

static void shuffle(Piece *array, size_t n)
{
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

static void bag_append() {
    Piece pieces[] = {I, J, L, O, S, T, Z};
    shuffle(pieces, 7);
    
    for (size_t i = 0; i<7; i++) {
        Node *new = malloc(sizeof(Node));
        new->piece = pieces[i];
        new->next = NULL;

        if (bag.head == NULL) {
            bag.head = new;
            bag.tail = new;
        } else {
            bag.tail->next = new;
            bag.tail = new;
        }
    }

    bag.size += 7;
}

void init_bag() 
{
    bag.head = NULL;
    bag.tail = NULL;
    bag.size = 0;
    bag_append();
    bag_append();
}

Piece next_piece() 
{
    Piece ret = bag.head->piece;
    Node* tmp = bag.head;
    bag.head = tmp->next;
    free(tmp);
    bag.size -= 1;
    if (bag.size <= 7) {
        bag_append();
    }
    return ret;
}

Piece peek_piece(int n) 
{
    if (bag.size < n) {
        return N;
    }
    Node* cur = bag.head;
    for (int i = 0; i<n; i++) {
        cur = cur->next;
    }
    return cur->piece;
}

void free_bag() 
{
    Node* cur = bag.head;
    while (cur != NULL) {
        Node* next = cur->next;
        free(cur);
        cur = next;
    }
    bag.head = NULL;
    bag.tail = NULL;
    bag.size = 0;
}
