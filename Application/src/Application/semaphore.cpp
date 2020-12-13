#include "header/semaphore.hpp"

Semaphore::Semaphore(int count_ = 0) : count(count_) {}

// ============ NOTIFIES ANOTHER THREAD THAT IT CAN STOP WAITING ============
void Semaphore::notify(int threadId) { // threadId only used during logging/debug
    // unique_lock/lock_guard manages a mutex object with unique ownership
    std::lock_guard<std::mutex> lock(mtx);
    
    count++;
    std::cout << "[Semaphore] Thread " << threadId <<  " notify" << std::endl;
    
    //notify the waiting thread
    conditionVar.notify_one();
}

// ============ SOMEONE IS ACCESSING CRITICAL REGION -> THREAD MUST WAIT ============
void Semaphore::wait(int threadId) { // threadId only used during logging/debug
    std::unique_lock<std::mutex> lock(mtx);

    while(count == 0) { // if count is zero, critial region can't be accessed
        std::cout << "[Semaphore] Thread " << threadId << " wait" << std::endl;
        
        // wait on the mutex until notify is called
        conditionVar.wait(lock);
        std::cout << "[Semaphore] Thread " << threadId << " run" << std::endl;
    }
    
    // When thread reaches here, it can do stuff in the critical region. After finishing, it must call notify()!!

    count--;
}