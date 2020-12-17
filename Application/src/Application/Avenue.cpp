#include "header/Avenue.hpp"

#include "DampEngine/Core/Macros.hpp"

Avenue::Avenue (int &attr): attr(attr) {
    sem_init(&empty, 1, FULL_N);
    sem_init(&full, 1, 0);
    pthread_mutex_init(&mutex, NULL);
}

Avenue::~Avenue () {
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
}

void Avenue::producer (int value) {
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    items.push(value);

    pthread_mutex_unlock(&mutex);
    sem_post(&full);
}

void Avenue::consumer () {
    DE_TRACE("(Avenue) Spawnei Consumer");
    while (true) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        attr += items.front();
        items.pop();

        if (attr < 0)
            attr = 0;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

void Avenue::up () {
    pthread_mutex_unlock(&mutex);
}

void Avenue::down () {
    pthread_mutex_lock(&mutex);
}

int Avenue::getValue () {
    int value;

    pthread_mutex_lock(&mutex);
    
    value = attr;
    
    pthread_mutex_unlock(&mutex);

    return value;
}