#include "header/Avenue.hpp"

#include "mypch.hpp"

template <>
Avenue<int>::Avenue (int &attr): attr(attr) {
    sem_init(&empty, 1, FULL_N);
    sem_init(&full, 1, 0);
    pthread_mutex_init(&mutex, NULL);
}

template <>
Avenue<int>::~Avenue () {
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
}

template <>
void Avenue<int>::producer (int value) {
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    items.push(value);

    pthread_mutex_unlock(&mutex);
    sem_post(&full);
}

template <>
void Avenue<int>::consumer () {
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
template <>
void Avenue<int>::up () {
    pthread_mutex_unlock(&mutex);
}

template <>
void Avenue<int>::down () {
    pthread_mutex_lock(&mutex);
}

template <>
void Avenue<std::nullptr_t>::up () {
    pthread_mutex_unlock(&mutex);
}

template <>
void Avenue<std::nullptr_t>::down () {
    pthread_mutex_lock(&mutex);
}