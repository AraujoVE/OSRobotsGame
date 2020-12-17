#pragma once

#include <pthread.h>
#include <semaphore.h>
#include <queue>

#define FULL_N 1000

class Avenue {
    private:
        sem_t empty, full;
        pthread_mutex_t mutex;
        int &attr;
        std::queue<int> items;

    public:
        Avenue(int &attr);
        ~Avenue();

        void producer (int value);
        void consumer ();

        void up();
        void down();

        int getValue ();
};