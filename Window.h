//
// Created by ben on 9/9/20.
//

#ifndef IDLEQUEUE_WINDOW_H
#define IDLEQUEUE_WINDOW_H

#include "gtkmm.h"
#include <iostream>
#include <thread>
#include "idle_queue.h"
#include "blocking_queue.h"
#include <functional>

class Window : public Gtk::Window
{
public:
    Window() : Gtk::Window()
        , m_box()
        , m_lbl("Press that there button.")
        , m_button("Press me.")
        , m_idle_queue()
        , m_blocking_queue()
        , m_n_presses(0)
        , m_work_thread(&Window::work, this)
    {
        m_box.add(m_lbl);
        m_box.add(m_button);
        m_button.signal_pressed().connect(sigc::bind(&Window::onPressed, this));
        add(m_box);
        show_all();
    }

private:
    Gtk::VBox m_box;
    Gtk::Label m_lbl;
    Gtk::Button m_button;
    IdleQueue m_idle_queue;
    BlockingQueue<std::function<int()>> m_blocking_queue;
    std::thread m_work_thread;
    int m_n_presses;

    void work()
    {
        for (;;)
        {
            const auto &job = m_blocking_queue.dequeue();
            if (job == nullptr) return;
            int n_presses = job();
            std::cout << "[" << std::this_thread::get_id() << "]" << "Button pressed " << n_presses << " time(s)." << std::endl;

            m_idle_queue.dispatch([this, n_presses]() {
                std::cout << "[" << std::this_thread::get_id() << "]" << "Button pressed " << n_presses << " time(s)." << std::endl;

                std::ostringstream stringStream;
                stringStream << "Button pressed " << n_presses << " times.";
                m_lbl.set_text(stringStream.str());
            });
        }
    }

    void onPressed()
    {
        m_n_presses++;
        std::cout << "[" << std::this_thread::get_id() << "]" << "Button pressed " << m_n_presses << " time(s)." << std::endl;
        auto presses = m_n_presses;
        m_blocking_queue.enqueue([presses]()->int{
            return presses;
        });
    }
};

#endif //IDLEQUEUE_WINDOW_H
