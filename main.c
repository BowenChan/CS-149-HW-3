/*
 * main.c
 *
 * Created on: Feb 26, 2016
 * Author: Group 2
 */

#include "header.h"
#include <time.h>
#include <unistd.h>

#define NUM_SELLERS 10

struct consumer_data {
    char *name;
    struct seller_data *seller;
};

struct seller_data thread_data_array[NUM_SELLERS];
time_t start;
static pthread_mutex_t mutex_seating = PTHREAD_MUTEX_INITIALIZER;

int time_elapsed() {
    time_t end = time(NULL);
    return end - start;
}

char *time_elapsed_string() {
    int time = time_elapsed();
    int seconds = time % 60;
    int minutes = (time / 60) % 60;
    char* str = (char *) malloc(5);
    sprintf(str, "%d:%02d", minutes, seconds);
    return str;
}

void *ProcessSellers(void *threadarg) {
    struct seller_data *my_data;
    my_data = (struct seller_data *) threadarg;
    while (time_elapsed() <= 60) {
        if (is_empty(my_data->next)) {
            sleep(1);
        } else {
            pthread_mutex_lock(&mutex_seating);
            int res = assign_seat(my_data);
            char *str = time_elapsed_string();
            if (res == 1) {
                printf("%s: Customer was assigned a seat from %s\n", str, my_data->name);
            }
            remove_head(my_data);
            pthread_mutex_unlock(&mutex_seating);
            if (my_data->type == H)
                sleep(rand() % 2 + 1);
            else if (my_data->type == M)
                sleep(rand() % 3 + 2);
            else
                sleep(rand() % 4 + 4);
            free(str);
            char *str2 = time_elapsed_string();
            if (res == 1)
                printf("%s: Customer has completed ticket purchase at %s\n", str, my_data->name);
            free(str2);
        }
    }
}

void *QueueConsumer(void *threadarg) {
    struct seller_data *my_data;
    my_data = (struct seller_data *) threadarg;
    sleep(rand() % 60);
    pthread_mutex_lock(&my_data->mutex);
    add_tail(my_data, 1);
    pthread_mutex_unlock(&my_data->mutex);
    char *str = time_elapsed_string();
    printf("%s: A customer has arrived at seller %s\n", str, my_data->name);
    free(str);
}

int main(int argc, char** argv) {
    int tickets_per_seller = 5;
    if (argc == 2) {
        tickets_per_seller = atoi(argv[1]);
    }

    char *sellers[NUM_SELLERS] = {"H", "M1", "M2", "M3", "L1", "L2", "L3", "L4", "L5", "L6"};
    pthread_t threads[NUM_SELLERS];
    pthread_t buy_threads[NUM_SELLERS * tickets_per_seller];
    int rc[NUM_SELLERS];
    int i;
    start = time(NULL);
    for (i = 0; i < NUM_SELLERS; i++) {
        thread_data_array[i].name = sellers[i];
        switch (sellers[i][0]) {
            case 'H':
                thread_data_array[i].type = H;
                break;
            case 'M':
                thread_data_array[i].type = M;
                break;
            case 'L':
                thread_data_array[i].type = L;
                break;
        }
        pthread_mutex_init(&thread_data_array[i].mutex, NULL);
        pthread_create(&threads[i], NULL, ProcessSellers, (void *) &thread_data_array[i]);
    }

    //struct consumer_data buyer_data_array[NUM_SELLERS][tickets_per_seller];
    time_t t;
    srand((unsigned) time(&t));
    int j;
    int k = 0;
    for (i = 0; i < NUM_SELLERS; i++) {
        for (j = 0; j < tickets_per_seller; j++) {
            //buyer_data_array[i][j] = &thread_data_array[i];
            pthread_create(&buy_threads[k++], NULL, QueueConsumer, (void *) &thread_data_array[i]);
        }
    }

    for (j = 0; j < sizeof (buy_threads) / sizeof (buy_threads[0]); j++) {
        pthread_join(buy_threads[j], NULL);
    }
    for (j = 0; j < sizeof (threads) / sizeof (threads[0]); j++) {
        pthread_join(threads[j], NULL);
    }
    sleep(1);
    print_seating();
    return 0;
}
