#pragma once

#include "DampEngine/Threads/Mutex.hpp"

#include <thread>
#include <semaphore.h>
#include <queue>

#define FULL_N 1000

template <typename T>
class Avenue
{
private:
    bool m_ConsumerRunning = false;
    sem_t empty, full;
    DampEngine::Mutex mutex, threadMutex;
    T &attr;
    std::queue<T> items;

    std::thread *consumer_thread = nullptr;

    static void runConsumer(Avenue *consumerObject)
    {
        consumerObject->consumer();
    }

public:
    Avenue(T &attr) : attr(attr)
    {
        sem_init(&empty, 1, FULL_N);
        sem_init(&full, 1, 0);
    }

    ~Avenue()
    {
        sem_destroy(&empty);
        sem_destroy(&full);
    }

    //Method called every time an absolute increment/decrement is desired
    void producer(T value)
    {
        sem_wait(&empty);
        mutex.Lock();
        //Queues an absolute increment/decrement
        items.push(value);

        mutex.Unlock();
        sem_post(&full);
    }

    //Method called once, (in a separate thread)
    void consumer()
    {
        while (m_ConsumerRunning)
        {
            sem_wait(&full);
        mutex.Lock();

            //Eval one absolute increment/decrement (delta)
            attr += items.front();

            items.pop();

            if (attr < 0)
                attr = 0;

        mutex.Unlock();
            sem_post(&empty);
        }
    }

    void startConsumer()
    {
        threadMutex.Lock();
        m_ConsumerRunning = true;
        consumer_thread = new std::thread(runConsumer, this);
        threadMutex.Unlock();

    }

    void stopConsumer()
    {
        threadMutex.Lock();
        DE_ASSERT(consumer_thread != nullptr, "Trying to stop consumer twice (or never started)");
        m_ConsumerRunning = false;
        producer(1);
        // DE_DEBUG("IN: join @stopConsumer");
        consumer_thread->join();
        // DE_DEBUG("OUT: join @stopConsumer");
        delete consumer_thread;
        consumer_thread = nullptr;
        threadMutex.Unlock();

    }

    void up()
    {
        mutex.Lock();
    }

    void down()
    {
        mutex.Unlock();
    }
};