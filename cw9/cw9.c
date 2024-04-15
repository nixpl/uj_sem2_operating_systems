#define _REENTRANT
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

enum ERRORS 
{
    ARGC_ERROR = 1,
    ATOI_ERROR,
    TOO_MANY_THREADS,
    PTHREAD_MUTEX_INIT_EEROR,
    PTHREAD_CREATE_ERROR,
    PTHREAD_MUTEX_LOCK_ERROR,
    PTHREAD_MUTEX_UNLOCK_ERROR,
    PTHREAD_JOIN_ERROR,
    PTHREAD_MUTEX_DESTROY_ERROR,

};

int global_counter = 0;

struct arg_struct
{
    int critic;
    int thread;
    int threads_num;
};

pthread_mutex_t mutex;

void rand_sleep()
{
    srand(time(0));
    sleep((rand()%3)+1);
}

void my_atoi(char* str, int* result)
{
    int buff = atoi(str);

    if(buff == 0)
    {
        printf("[%s] nie jest liczba typu int lub jest to zero\n", str);
        perror("atoi error");
        exit(ATOI_ERROR);
    }       
    else
        *result = buff;
}

void go_to_xy(unsigned int x, unsigned int y)
{
  printf("\033[%d;%dH\033[2K", y, x);
}

void* start_fun(void* arguments_ptr)
{
    struct arg_struct* arguments = arguments_ptr;

    int thread_num = arguments->thread;

    int threads_num = arguments->threads_num;

    int critic = arguments->critic;

    for(int i = 0; i < critic; i++)
    {
        rand_sleep();

        go_to_xy(1, 1 + threads_num + 3 + thread_num);
        printf("[%d watek]Sekcja prywatna nr: %d, (globalny: %d)\n", thread_num, i+1, global_counter);

        rand_sleep();

        if(pthread_mutex_lock(&mutex) != 0)
        {
            printf("pthread_mutex_lock error: %s\n", strerror(errno));
            exit(PTHREAD_MUTEX_LOCK_ERROR);
        }       

        int buff = global_counter + 1;
        rand_sleep();
        global_counter = buff;

        go_to_xy(47, 1 + threads_num + 3 + thread_num);
        printf("[%d watek]Sekcja publiczna nr: %d (globalny: %d)\n", thread_num, i+1, global_counter);

        if(pthread_mutex_unlock(&mutex) != 0)
        {
            printf("pthread_mutex_unlock error: %s\n", strerror(errno));
            exit(PTHREAD_MUTEX_UNLOCK_ERROR);
        }   
    }
    return 0;
}

void create_threads(int threads_num, struct arg_struct* arguments, int crit_section_num, pthread_t* thread_id_array)
{
    for(int i = 0; i < threads_num; i++)
    {
        arguments[i].thread = i + 1;
        arguments[i].critic = crit_section_num;
        arguments[i].threads_num = threads_num;
        if(pthread_create(&thread_id_array[i], NULL, start_fun, &arguments[i]) != 0)
        {
            printf("pthread_create error: %s\n", strerror(errno));
            exit(PTHREAD_CREATE_ERROR);
        }
        printf("Stworzono watek %d [id: %lu]\n", i+1, (unsigned long)thread_id_array[i]);
    }
}

void wait_threads(int threads_num, int crit_section_num, pthread_t* thread_id_array)
{
    for(int i = 0; i < threads_num; i++)
        if(pthread_join(thread_id_array[i], NULL) != 0)
        {
            printf("pthread_join error: %s\n", strerror(errno));
            exit(PTHREAD_JOIN_ERROR);
        }
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Podales %d argumentow zamiast 2\n", argc-1);
        printf("spodziewano sie: <threads_num> <crit_section_num>\n");
        exit(ARGC_ERROR);
    }

    int threads_num, crit_section_num;
    my_atoi(argv[1], &threads_num);
    my_atoi(argv[2], &crit_section_num);

    pthread_t thread_id_array[threads_num];
    struct arg_struct arguments[threads_num];

    if(pthread_mutex_init(&mutex, NULL) != 0)
    {
        printf("pthread_mutex_init error: %s\n", strerror(errno));
        exit(PTHREAD_MUTEX_INIT_EEROR);
    }
    printf("Adres utworzonego muteksu: %p\n", (void*)&mutex);

    create_threads(threads_num, arguments, crit_section_num, thread_id_array);

    wait_threads(threads_num, crit_section_num, thread_id_array);

    go_to_xy(1, 3 + threads_num + 2 + threads_num + 1);
    printf("Wartosc licznika: %d, oczekiwana wartosc %d\n", global_counter, threads_num * crit_section_num);

    if (pthread_mutex_destroy(&mutex) != 0)
    {
        printf("pthread_mutex_destroy error: %s\n", strerror(errno));
        exit(PTHREAD_MUTEX_DESTROY_ERROR);
    }
    return 0;
}