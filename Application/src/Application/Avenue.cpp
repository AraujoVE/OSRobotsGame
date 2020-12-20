#include "header/Avenue.hpp"

#include "mypch.hpp"

void *runConsumer (void *consumerObject) {
    Avenue *avenue = (Avenue*) consumerObject;
    avenue->consumer();
    return NULL;
}

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

void Avenue::producer(int value) {
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    items.push(value);

    pthread_mutex_unlock(&mutex);
    sem_post(&full);
}

void Avenue::consumer() {
    DE_TRACE("(Avenue) Spawnei Consumer");
    while (m_ConsumerRunning) {
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

void Avenue::startConsumer() {
    m_ConsumerRunning = true;
    pthread_create(&consumer_thread, NULL, runConsumer, this);
}

void Avenue::stopConsumer() {
    m_ConsumerRunning = false;
    producer(666);
    DE_DEBUG("IN: join @Avenue::stopConsumer");
    pthread_join(consumer_thread, NULL);
    DE_DEBUG("OUT: join @Avenue::stopConsumer");
}


void Avenue::up () {
    pthread_mutex_unlock(&mutex);
}

void Avenue::down () {
    pthread_mutex_lock(&mutex);
}


