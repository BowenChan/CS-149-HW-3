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
#define SECS_TO_RUN 60

struct consumer_data {
    char *name;
    struct seller_data *seller;
};

struct seller_data thread_data_array[NUM_SELLERS];
static time_t start;
static pthread_mutex_t mutex_seating = PTHREAD_MUTEX_INITIALIZER;
static int h_sold, m_sold, l_sold;
static int h_leave, m_leave, l_leave;
static int h_closing, m_closing, l_closing;
static int h_closed, m_closed, l_closed;
static int h_left, m_left, l_left;

/* Get time elapsed */
int time_elapsed() {
    time_t end = time(NULL);
    return end - start;
}

/* Get time elapsed as a string. 
 * i.e 0:30 for 30 seconds 
 */
char *time_elapsed_string() {
    int time = time_elapsed();
    int seconds = time % 60;
    int minutes = (time / 60) % 60;
    char* str = (char *) malloc(5);
    sprintf(str, "%d:%02d", minutes, seconds);
    return str;
}

/* Method for sellers to run */
void *ProcessSellers(void *threadarg) {
    struct seller_data *my_data;
    my_data = (struct seller_data *) threadarg;
    while (time_elapsed() <= SECS_TO_RUN) { // Only run if time isn't up
        if (is_empty(my_data->next)) {
            sleep(1); // Sleep for 1 second if no one in line
        } else {
            pthread_mutex_lock(&mutex_seating);
            int res = assign_seat(my_data); // Assign seating
            pthread_mutex_unlock(&mutex_seating);
            char *str = time_elapsed_string();
            if (res == 1) {
                int completion_time = 0;
                if (my_data->type == H) {
                    completion_time = rand() % 2 + 1; // Sleep for 1 - 2
                } else if (my_data->type == M) {
                    completion_time = rand() % 3 + 2; // Sleep for 2 - 4
                } else {
                    completion_time = rand() % 4 + 4; // Sleep for 4 - 7
                }
                pthread_mutex_lock(&my_data->mutex);
                remove_head(my_data); // Remove first person from the line
                // Increment wait timer on all people in line and get amount of buyers who left
                int cust_left = increment_wait_timer(my_data, completion_time, str);
                printf("%s: A customer was assigned a seat at %s\n", str, my_data->name);
                pthread_mutex_unlock(&my_data->mutex);
                // Increments ticket sold
                if (my_data->type == H) {
                    h_sold++;
                    h_left += cust_left;
                } else if (my_data->type == M) {
                    m_sold++;
                    m_left += cust_left;
                } else {
                    l_sold++;
                    l_left += cust_left;
                }
                // Sleep for random time to generate minutes to complete sale
                sleep(completion_time);
                free(str);
                char *str2 = time_elapsed_string();
                printf("%s: A customer has completed their ticket purchase at %s\n", str2, my_data->name);
                free(str2);
            } else {
                pthread_mutex_lock(&my_data->mutex);
                printf("%s: Remaining %d customer(s) at %s are being told to leave since tickets are sold out\n", str, my_data->cust_count, my_data->name);
                free(str);
                // Remove and count everyone from the line
                while (my_data->next != NULL) {
                    remove_head(my_data);
                    if (my_data->type == H)
                        h_leave++;
                    else if (my_data->type == M)
                        m_leave++;
                    else
                        l_leave++;
                }
                my_data->closed = 1; // Set seller to closed
                pthread_mutex_unlock(&my_data->mutex);
                pthread_exit(NULL); // Exit seller thread
            }
        }
    }
    pthread_mutex_lock(&my_data->mutex);
    my_data->closed = 1; // Set seller to closed
    int rem = my_data->cust_count;
    if (rem != 0) {
        char *str = time_elapsed_string();
        printf("%s: Remaining %d customer(s) at %s are being told to leave since the selling time is up\n", str, my_data->cust_count, my_data->name);
        free(str);
        // Remove and count everyone from the line
        while (my_data->next != NULL) {
            remove_head(my_data);
            if (my_data->type == H)
                h_closing++;
            else if (my_data->type == M)
                m_closing++;
            else
                l_closing++;
        }
    }
    pthread_mutex_unlock(&my_data->mutex);
}

/* Consumer method
 * Sleeps for random amount of time and adds to seller line after
 */
void *QueueConsumer(void *threadarg) {
    // Get thread args that contain designated seller info
    struct seller_data *my_data;
    my_data = (struct seller_data *) threadarg;
    sleep(rand() % SECS_TO_RUN); // Sleep from 0 - 59 seconds
    pthread_mutex_lock(&my_data->mutex);
    char *str = time_elapsed_string();
    if (my_data->closed != 1) {
        add_tail(my_data, 0); // Add to designated seller line
        printf("%s: A customer has arrived at seller %s\n", str, my_data->name);
    } else {
        // Increment customers who came when seller was closed
        if (my_data->type == H)
            h_closed++;
        else if (my_data->type == M)
            m_closed++;
        else
            l_closed++;
        printf("%s: A customer has arrived but the seller was already closed %s\n", str, my_data->name);
    }
    free(str);
    pthread_mutex_unlock(&my_data->mutex);
}

int main(int argc, char** argv) {
    int tickets_per_seller = 5;
    if (argc == 2) { // Check if an arg was entered
        tickets_per_seller = atoi(argv[1]); // Set tickets per seller to arg
    }

    char *sellers[NUM_SELLERS] = {"H", "M1", "M2", "M3", "L1", "L2", "L3", "L4", "L5", "L6"};
    pthread_t seller_threads[NUM_SELLERS];
    pthread_t buy_threads[NUM_SELLERS * tickets_per_seller];
    int i;
    start = time(NULL); // Record start time
    // Create all seller threads
    for (i = 0; i < NUM_SELLERS; i++) {
        thread_data_array[i].name = sellers[i]; // Set seller name
        // Set seller type
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
        pthread_create(&seller_threads[i], NULL, ProcessSellers, (void *) &thread_data_array[i]);
    }

    time_t t;
    srand((unsigned) time(&t)); // Generate RNG seed
    int j;
    int k = 0;
    // Create buyer threads
    for (i = 0; i < NUM_SELLERS; i++) {
        for (j = 0; j < tickets_per_seller; j++) {
            pthread_create(&buy_threads[k++], NULL, QueueConsumer, (void *) &thread_data_array[i]);
        }
    }
    // Wait for buyer threads to finish
    for (j = 0; j < sizeof (buy_threads) / sizeof (buy_threads[0]); j++) {
        pthread_join(buy_threads[j], NULL);
    }
    // Wait for seller threads to finish
    for (j = 0; j < sizeof (seller_threads) / sizeof (seller_threads[0]); j++) {
        pthread_join(seller_threads[j], NULL);
    }
    sleep(1);
    print_seating();
    printf("H customers who got seats: %d\n", h_sold);
    printf("M customers who got seats: %d\n", m_sold);
    printf("L customers who got seats: %d\n", l_sold);
    printf("H customers who were told to leave because of ticket sellout: %d\n", h_leave);
    printf("M customers who were told to leave because of ticket sellout: %d\n", m_leave);
    printf("L customers who were told to leave because of ticket sellout: %d\n", l_leave);
    printf("H customers who were told to leave because the selling time was up: %d\n", h_closing);
    printf("M customers who were told to leave because the selling time was up: %d\n", m_closing);
    printf("L customers who were told to leave because the selling time was up: %d\n", l_closing);
    printf("H customers who came when seller was closed: %d\n", h_closed);
    printf("M customers who came when seller was closed: %d\n", m_closed);
    printf("L customers who came when seller was closed: %d\n", l_closed);
    printf("H customers who were impatient and left: %d\n", h_left);
    printf("M customers who were impatient and left: %d\n", m_left);
    printf("L customers who were impatient and left: %d\n", l_left);
    return 0;
}
