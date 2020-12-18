#pragma once

#include <pthread.h>
#include <semaphore.h>
#include <queue>

#define FULL_N 1000

template<typename T>
class Avenue {
    private:
        sem_t empty, full;
        pthread_mutex_t mutex;
        T &attr;
        std::queue<int> items;

    public:
        Avenue(T &attr);
        ~Avenue();

        void producer (T value);
        void consumer ();

        void up();
        void down();

        T getValue ();
};