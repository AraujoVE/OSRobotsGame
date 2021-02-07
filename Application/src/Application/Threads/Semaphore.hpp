#pragma once

#include <semaphore.h>

#include "DampEngine/Threads/Mutex.hpp"

namespace Application
{

    class Semaphore final
    {
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
            sem_wait(&sem);
        }
        inline void Post()
        {
            sem_post(&sem);
        }
    };

} // namespace Application