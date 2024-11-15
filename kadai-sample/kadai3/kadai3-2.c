#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

char *in_str1 = NULL;
char *in_str2 = NULL;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_input = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_display = PTHREAD_COND_INITIALIZER;

int terminater = 0; //if it is 1, it means "end it up"//

void *input() {
    while(1) {
        pthread_mutex_lock(&mutex);

        while (in_str1 != NULL || in_str2 != NULL) {
            pthread_cond_wait(&cond_input, &mutex);
        }

        in_str1 = (char *)malloc(sizeof(char) * 80);
        printf("Input 1 th string (or \"quit\"): ");
        scanf("%s", in_str1);

        if (strcmp(in_str1, "quit") == 0) {
            terminater = 1;
            pthread_cond_signal(&cond_display);
            pthread_mutex_unlock(&mutex);
            break;
        }


        in_str2 = (char *)malloc(sizeof(char) * 80);
        printf("Input 2 th string (or \"quit\"): ");
        scanf("%s", in_str2);

        if (strcmp(in_str2, "quit") == 0) {
            terminater = 1;
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

        while (in_str1 == NULL || in_str2 == NULL) {
            if (terminater) {
                pthread_mutex_unlock(&mutex);
                return NULL;
            }
            pthread_cond_wait(&cond_display, &mutex);
        }
        
        if (terminater) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        char *concat_str = (char *)malloc(strlen(in_str1) + strlen(in_str2) + 1);
        strcpy(concat_str, in_str1);
        strcat(concat_str, in_str2);
        printf("Concatinated string is %s\n", concat_str);

        free(in_str1);
        free(in_str2);
        free(concat_str);
        in_str1 = NULL;
        in_str2 = NULL;

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

