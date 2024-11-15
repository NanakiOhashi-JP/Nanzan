#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

char *in_str = NULL;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_input = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_display = PTHREAD_COND_INITIALIZER;

void *input() {
    while(1) {
        pthread_mutex_lock(&mutex);

        while (in_str != NULL) {
            pthread_cond_wait(&cond_input, &mutex);
        }

        in_str = (char *)malloc(sizeof(char) * 80);
        printf("Input string (or \"quit\"): ");
        scanf("%s", in_str);

        if (strcmp(in_str, "quit") == 0) {
            pthread_cond_signal(&cond_display);
            pthread_mutex_unlock(&mutex);
            break;
        }
        
        pthread_cond_signal(&cond_display);
        pthread_mutex_unlock(&mutex);
    }
}

void *display() {
    while(1) {
        pthread_mutex_lock(&mutex);

        while (in_str == NULL) {
            pthread_cond_wait(&cond_display, &mutex);
        }

        if (strcmp(in_str, "quit") == 0) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        printf("Inputted string is %s\n", in_str);

        free(in_str);
        in_str = NULL;
        pthread_cond_signal(&cond_input);

        pthread_mutex_unlock(&mutex);
    }
}

int main() {
    pthread_t handle1, handle2;

    pthread_create(&handle1, NULL, input, (void *) NULL);
    pthread_create(&handle2, NULL, display, (void *) NULL);

    pthread_join(handle1, NULL);
    pthread_join(handle2, NULL);

    return 0;
}

