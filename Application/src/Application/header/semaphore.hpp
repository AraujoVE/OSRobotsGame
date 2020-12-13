// Custom semahpore class using mutex and conditional variable -> there's a <semaphore> lib for C++20
#include <mutex>
#include <condition_variable>
#include <iostream>
        
class Semaphore {
    private:
        std::mutex mtx;
        std::condition_variable conditionVar;
        int count = 0; // initialize as locked

public:
    Semaphore(int);
    
    void notify(int);
    void wait(int);
    bool tryWait(int);
};