#pragma once

#include <pthread.h>
#include <semaphore.h>
#include <queue>

#define FULL_N 1000

class Avenue {
    private:
        bool m_ConsumerRunning = false;
        sem_t empty, full;
        pthread_mutex_t mutex;
        int &attr;
        std::queue<int> items;

        pthread_t consumer_thread;

    public:
        Avenue(int &attr);
        ~Avenue();

        void producer(int value);
        void consumer();

        void startConsumer();
        void stopConsumer();

        void up();
        void down();

};