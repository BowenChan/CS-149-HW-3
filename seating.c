/*
 * seating.c
 *
 * Created on: Feb 26, 2016
 * Author: Group 2
 */

#include "header.h"
#include <string.h>

static char *seating[10][10] = {
    {"----", "----", "----", "----", "----", "----", "----", "----", "----", "----"},
    {"----", "----", "----", "----", "----", "----", "----", "----", "----", "----"},
    {"----", "----", "----", "----", "----", "----", "----", "----", "----", "----"},
    {"----", "----", "----", "----", "----", "----", "----", "----", "----", "----"},
    {"----", "----", "----", "----", "----", "----", "----", "----", "----", "----"},
    {"----", "----", "----", "----", "----", "----", "----", "----", "----", "----"},
    {"----", "----", "----", "----", "----", "----", "----", "----", "----", "----"},
    {"----", "----", "----", "----", "----", "----", "----", "----", "----", "----"},
    {"----", "----", "----", "----", "----", "----", "----", "----", "----", "----"},
    {"----", "----", "----", "----", "----", "----", "----", "----", "----", "----"}
};

/* Assign seating based on availability */
int assign_seat(struct seller_data *seller) {
    int i, j;
    if (seller->type == H) {
        for (i = 0; i < 10; i++) {
            for (j = 0; j < 10; j++) {
                if (strcmp(seating[i][j], "----") == 0) {
                    char* str = (char *) malloc(5);
                    sprintf(str, "%s%03d", seller->name, seller->num_sold + 1);
                    seating[i][j] = str;
                    seller->num_sold++;
                    return 1;
                }
            }
        }
    } else if (seller->type == L) {
        for (i = 9; i >= 0; i--) {
            for (j = 0; j < 10; j++) {
                if (strcmp(seating[i][j], "----") == 0) {
                    char* str = (char *) malloc(5);
                    sprintf(str, "%s%02d", seller->name, seller->num_sold + 1);
                    seating[i][j] = str;
                    seller->num_sold++;
                    return 1;
                }
            }
        }
    } else {
        int order[10] = {4, 5, 3, 6, 2, 7, 1, 8, 0, 9};
        for (i = 0; i < 10; i++) {
            for (j = 0; j < 10; j++) {
                if (strcmp(seating[order[i]][j], "----") == 0) {
                    char* str = (char *) malloc(5);
                    sprintf(str, "%s%02d", seller->name, seller->num_sold + 1);
                    seating[order[i]][j] = str;
                    seller->num_sold++;
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* Print seating chart */
void print_seating() {
    printf("\nSeating Chart:\n");
    int i, j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            printf("%s ", seating[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}