//
// Created by ben on 9/9/20.
//

#ifndef IDLEQUEUE_SAFE_QUEUE_H
#define IDLEQUEUE_SAFE_QUEUE_H

//
// Created by ben on 9/8/20.
//

#ifndef BUILD_SAFE_QUEUE_H
#define BUILD_SAFE_QUEUE_H

#include <queue>
#include <mutex>
#include <optional>

// A thread-safe-queue.
template <class T>
class SafeQueue
{
public:
    SafeQueue()
        : m_queue()
        , m_mutex()
    {
    }

    // Add an element to the queue.
    void enqueue(T t)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(t);
    }

    // Get the "front"-element.
    // If the queue is empty, return.
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
};
#endif //BUILD_SAFE_QUEUE_H


#endif //IDLEQUEUE_SAFE_QUEUE_H
