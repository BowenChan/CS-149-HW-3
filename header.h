/*
 * header.h
 *
 * Created on: Feb 26, 2016
 * Author: Group 2
 */

#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef enum {
    H,
    M,
    L
} seller_type;

struct seller_data {
    int num_sold;
    pthread_mutex_t mutex;
    char *name;
    seller_type type;
    struct node *next;
};

struct node* add_tail(struct seller_data *seller, int value);
struct node* remove_head(struct seller_data *seller);
int is_empty(struct node *n);
int assign_seat(struct seller_data *seller);
void print_seating();

#endif
