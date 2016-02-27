/*
 * seller_queue.c
 *
 * Created on: Feb 26, 2016
 * Author: Group 2
 */

#include "header.h"

struct node {
    int data;
    struct node *next;
};

void free_mem(struct node* a) {
    if (a == NULL)
        return;
    else {
        free_mem(a->next);
        free(a);
    }
    return;
}

int is_empty(struct node *n) {
    if (n == NULL)
        return 1;
    return 0;
}

struct node* remove_head(struct seller_data *seller) {
    if (seller->next == NULL) return NULL;
    struct node* new_head = seller->next->next;
    free(seller->next);
    seller->next = new_head;
    return new_head;
}

struct node* add_tail(struct seller_data *seller, int value) {
    struct node* a = seller->next;
    while (a != NULL) {
        a = a->next;
    }
    struct node* newNode = (struct node*) malloc(sizeof (struct node));
    newNode->data = value;
    newNode->next = NULL;
    if (seller->next == NULL)
        seller->next = newNode;
    else
        seller->next->next = newNode;
    return newNode;
}