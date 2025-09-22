#ifndef RANDOMIZER_H

#define RANDOMIZER_H

#include "physics.h"

typedef struct Node {
    Piece piece;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
    uint32_t size;
} LinkedList;

void init_bag();
Piece next_piece();
Piece peek_piece(int n);
void free_bag();

#endif // !RANDOMIZER_H
