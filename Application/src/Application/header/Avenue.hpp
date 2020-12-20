#pragma once

#include <pthread.h>
#include <semaphore.h>
#include <queue>

#define FULL_N 1000

class Avenue {
    using T = int;
    private:
        bool m_ConsumerRunning = false;
        sem_t empty, full;
        pthread_mutex_t mutex;
        T &attr;
        std::queue<T> items;

        pthread_t consumer_thread;

    public:
        Avenue(T &attr);
        ~Avenue();

        void producer(T value);
        void consumer();

        void startConsumer();
        void stopConsumer();

        void up();
        void down();

};