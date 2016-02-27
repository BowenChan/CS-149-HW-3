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

/* Check if this line is empty */
int is_empty(struct node *n) {
    if (n == NULL)
        return 1;
    return 0;
}

/* Remove first buyer in line */
struct node* remove_head(struct seller_data *seller) {
    if (seller->next == NULL) return NULL;
    struct node* new_head = seller->next->next;
    free(seller->next);
    seller->next = new_head;
    seller->cust_count--;
    return new_head;
}

/* Add a buyer to end of line */
struct node* add_tail(struct seller_data *seller, int value) {
    seller->cust_count++;
    struct node* a = seller->next;
    while (a != NULL && a->next != NULL) {
        a = a->next;
    }
    struct node* newNode = (struct node*) malloc(sizeof (struct node));
    newNode->data = value;
    newNode->next = NULL;
    if (seller->next == NULL)
        seller->next = newNode;
    else
        a->next = newNode;
    return newNode;
}