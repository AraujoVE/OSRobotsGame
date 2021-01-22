#pragma once

#include <pthread.h>
#include <semaphore.h>
#include <queue>

#define FULL_N 1000

template <typename T>
class Avenue
{
private:
    bool m_ConsumerRunning = false;
    sem_t empty, full;
    pthread_mutex_t mutex;
    T &attr;
    std::queue<T> items;

    pthread_t consumer_thread;

    static void *runConsumer(void *consumerObject)
    {
        Avenue *avenue = (Avenue *)consumerObject;
        avenue->consumer();
        return NULL;
    }

public:
    Avenue(T &attr) : attr(attr)
    {
        sem_init(&empty, 1, FULL_N);
        sem_init(&full, 1, 0);
        pthread_mutex_init(&mutex, NULL);
    }

    ~Avenue()
    {
        sem_destroy(&empty);
        sem_destroy(&full);
        pthread_mutex_destroy(&mutex);
    }

    //Method called every time an absolute increment/decrement is desired
    void producer(T value)
    {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        //Queues an absolute increment/decrement
        items.push(value);

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }

    //Method called once, (in a separate thread)
    void consumer()
    {
        while (m_ConsumerRunning)
        {
            sem_wait(&full);
            pthread_mutex_lock(&mutex);

            //Eval one absolute increment/decrement (delta)
            attr += items.front();

            items.pop();

            if (attr < 0)
                attr = 0;

            pthread_mutex_unlock(&mutex);
            sem_post(&empty);
        }
    }

    void startConsumer()
    {
        m_ConsumerRunning = true;
        pthread_create(&consumer_thread, NULL, runConsumer, this);
    }

    void stopConsumer()
    {
        m_ConsumerRunning = false;
        producer(1);
        // DE_DEBUG("IN: join @stopConsumer");
        pthread_join(consumer_thread, NULL);
        // DE_DEBUG("OUT: join @stopConsumer");
    }

    void up()
    {
        pthread_mutex_unlock(&mutex);
    }

    void down()
    {
        pthread_mutex_lock(&mutex);
    }
};