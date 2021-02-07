#pragma once

#include "DampEngine/Threads/Mutex.hpp"

#include <thread>
#include <semaphore.h>
#include <queue>
#include <mutex>

#define FULL_N 1000

template <typename T>
class Avenue
{
private:
    std::mutex m_ItemsMutex;
    T &m_ReferenceVariable;

    static void runConsumer(Avenue *consumerObject)
    {
    }

public:
    Avenue(T &m_ReferenceVariable) : m_ReferenceVariable(m_ReferenceVariable)
    {
    }

    ~Avenue()
    {
    }

    //Method called every time an absolute increment/decrement is desired
    void producer(T value)
    {
        m_ItemsMutex.lock();

        m_ReferenceVariable += value;

        m_ItemsMutex.unlock();
    }

    //Method called once, (in a separate thread)
    void consumer()
    {

    }

    void startConsumer()
    {

    }

    void stopConsumer()
    {

    }

    void up()
    {
        m_ItemsMutex.lock();
    }

    void down()
    {
        m_ItemsMutex.unlock();
    }
};