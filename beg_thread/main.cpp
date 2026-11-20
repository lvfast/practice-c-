#include<iostream>
#include<queue>
#include<thread>
#include<mutex>
#include<condition_variable>


template<typename T>
class ThreadSafeQueue
{
private:
    std::queue<T> queue;
    std::mutex mtx;
    std::condition_variable cv;

public:
    void push(T value)
    {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(std::move(value));
        cv.notify_one();
    }

    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !queue.empty();});
        int val = std::move(queue.front());
        queue.pop();
    }
};






std::queue<int> queue;
std::mutex mtx;
std::condition_variable cv;

void producer()
{
    for (int i = 0; i < 10000; ++i)
    {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(i);
        cv.notify_one();
    }
}

void consumer()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mtx);

        cv.wait(lock, [] {return !queue.empty(); });
        int val = queue.front();
        queue.pop();
        std::cout << "Processed: " << val << "\n";
        
    }
}

int main()
{
    std::thread t1(producer);
    std::thread t2(consumer);

    t1.join();
    t2.join();

    return 0;
}