#pragma once

#include <semaphore.h>

#include "DampEngine/Threads/Mutex.hpp"

namespace Application
{

    class Semaphore final
    {
        DampEngine::Mutex mutex;
        sem_t sem;

    public:
        Semaphore()
        {
            sem_init(&sem, 1, 0);
        }
        ~Semaphore()
        {
            sem_destroy(&sem);
        }

        inline void Wait()
        {
            mutex.Lock();
            sem_wait(&sem);
            mutex.Unlock();
        }
        inline void Post()
        {
            mutex.Lock();
            sem_post(&sem);
            mutex.Unlock();
        }
    };

} // namespace Application