//
// Created by ben on 9/9/20.
//

#ifndef IDLEQUEUE_IDLE_QUEUE_H
#define IDLEQUEUE_IDLE_QUEUE_H

#include "safe_queue.h"
#include "glibmm.h"
#include <functional>
#include <optional>
#include <utility>

typedef std::function<void()> q_fn;

class IdleQueue
{
public:
    IdleQueue()
        : m_safe_queue()
        , m_dispatcher()
    {
        m_dispatcher.connect(sigc::bind(&IdleQueue::processJobs, this));
    }

    void dispatch(q_fn func)
    {
        m_safe_queue.enqueue(std::move(func));
        m_dispatcher.emit();
    }
private:
    SafeQueue<q_fn> m_safe_queue;
    Glib::Dispatcher m_dispatcher;

    // This function will always take place on the main thread.
    void processJobs()
    {
        for (;;) // Eagerly process all jobs on queue.
        {
            const std::optional<q_fn> &job_opt = m_safe_queue.try_dequeue();
            if (!job_opt.has_value()) return;

            try
            {
                job_opt.value()();
            }
            catch (std::exception &e)
            {
                std::cout << "Got exception while processing job: " << e.what();
            }
        }
    }
};

#endif //IDLEQUEUE_IDLE_QUEUE_H
