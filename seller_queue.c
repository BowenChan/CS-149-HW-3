/*
 * seller_queue.c
 *
 * Created on: Feb 26, 2016
 * Author: Group 2
 */

#include "header.h"

struct node {
    int wait_timer;
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
    newNode->wait_timer = value;
    newNode->next = NULL;
    if (seller->next == NULL)
        seller->next = newNode;
    else
        a->next = newNode;
    return newNode;
}

/* Increment wait timer for each buyer 
 * Remove from queue if they have waited for 10 seconds
 */
int increment_wait_timer(struct seller_data *seller, int time, char* timestamp) {
    int i = 0;
    struct node* prev = NULL;
    struct node* cur_buyer = seller->next;
    while (cur_buyer != NULL) {
        cur_buyer->wait_timer += time;
        if (cur_buyer->wait_timer >= 10) {
            i++;
            printf("%s: A customer at %s was impatient and left the line\n", timestamp, seller->name);
            if (prev == NULL) {
                seller->next = cur_buyer->next;
                cur_buyer = seller->next;
                continue;
            } else {
                prev->next = cur_buyer->next;
                free(cur_buyer);
                cur_buyer = prev->next;
                continue;
            }
        }
        prev = cur_buyer;
        cur_buyer = cur_buyer->next;
    }
    return i;
}