#pragma once

#include <pthread.h>

namespace DampEngine
{
    class Mutex final
    {
    public:
        Mutex()
        {
            pthread_mutex_init(&m_Mutex, NULL);
        }

        ~Mutex()
        {
            pthread_mutex_destroy(&m_Mutex);
        }

        void Lock()
        {
            pthread_mutex_lock(&m_Mutex);
        }

        void Unlock()
        {
            pthread_mutex_unlock(&m_Mutex);
        }

    private:
        pthread_mutex_t m_Mutex;
    };
} // namespace DampEngine
