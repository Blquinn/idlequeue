//
// Created by ben on 9/9/20.
//

#ifndef IDLEQUEUE_BLOCKING_QUEUE_H
#define IDLEQUEUE_BLOCKING_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

// A simple thread-safe queue.
// Synchronization is performed by a mutex.
template <class T>
class SafeQueue
{
public:
    SafeQueue()
            : m_queue()
            , m_mutex()
            , m_cond()
    {}

    // Add an element to the queue.
    void enqueue(T t)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(t);
        m_cond.notify_one();
    }

    // Get the "front"-element.
    // If the queue is empty, wait till a element is available.
    T dequeue()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while(m_queue.empty())
            m_cond.wait(lock);
        T val = m_queue.front();
        m_queue.pop();
        return val;
    }

    // Get the "front"-element.
    // If the queue is empty it will return a nullopt.
    std::optional<T> try_dequeue()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_queue.empty()) return std::nullopt;
        T val = m_queue.front();
        m_queue.pop();
        return val;
    }
private:
    std::queue<T> m_queue;
    mutable std::mutex m_mutex;
    std::condition_variable m_cond;
};

#endif //IDLEQUEUE_BLOCKING_QUEUE_H