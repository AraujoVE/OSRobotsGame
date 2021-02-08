#pragma once

#include <mutex>

#define FULL_N 1000

template <typename T>
class Avenue
{
private:
    std::mutex m_ItemsMutex;
    T &m_ReferenceVariable;
    uint64_t m_ID;
    static uint64_t s_nextID;

    static void runConsumer(Avenue *consumerObject)
    {
    }

public:
    Avenue(T &m_ReferenceVariable) : m_ReferenceVariable(m_ReferenceVariable)
    {
        m_ID = s_nextID++;
    }

    ~Avenue()
    {
    }

    //Method called every time an absolute increment/decrement is desired
    void producer(T value)
    {
        std::lock_guard<std::mutex> itemGuard(m_ItemsMutex);
        m_ReferenceVariable += value;
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

template <typename T>
uint64_t Avenue<T>::s_nextID = 0;
