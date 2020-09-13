//
// Created by ben on 9/9/20.
//

#ifndef IDLEQUEUE_WINDOW_H
#define IDLEQUEUE_WINDOW_H

#include "iostream"
#include "thread"
#include "chrono"
#include "functional"
#include "random"

#include "gtkmm.h"
#include "glibmm.h"

#include "blocking_queue.h"


class Window : public Gtk::Window
{
public:
    Window() : Gtk::Window()
        , m_box()
        , m_lbl()
        , m_button("Start Jobs")
        , m_blocking_queue()
        , m_progress_bar()
        , m_text_scroll()
        , m_work_thread(&Window::work, this)
    {
        set_default_size(400, 300);
        m_box.set_homogeneous(false);
        m_box.pack_start(m_progress_bar, false, false, 0);
        m_lbl.set_editable(false);
        m_lbl.signal_size_allocate().connect(sigc::mem_fun(this, &Window::lblSizeAllocate));
        m_text_scroll.add(m_lbl);
        m_text_scroll.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
        m_box.pack_end(m_button, false, false, 0);
        m_box.pack_end(m_text_scroll, true, true, 0);
        m_button.signal_pressed().connect(sigc::bind(&Window::onPressed, this));
        add(m_box);
        show_all();
    }

    ~Window() override
    {
        m_blocking_queue.enqueue(nullptr);
        m_work_thread.join();
    }

private:
    Gtk::VBox m_box;
    Gtk::ProgressBar m_progress_bar;
    Gtk::ScrolledWindow m_text_scroll;
    Gtk::TextView m_lbl;
    Gtk::Button m_button;
    SafeQueue<std::function<int()>> m_blocking_queue;
    std::thread m_work_thread;
    int m_total_work;
    int m_current_work;

    void lblSizeAllocate(Gdk::Rectangle rect)
    {
        auto adj = m_text_scroll.get_vadjustment();
        adj->set_value(adj->get_upper() - adj->get_page_size());
    }

    bool workCallback(void *data) {
        std::cout << "Got work callback in thread " << std::this_thread::get_id() << std::endl;
        return false;
    }

    void work()
    {
        for (;;)
        {
            const auto &job = m_blocking_queue.dequeue();
            if (job == nullptr) return;
            int jobNumber = job();

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
                
            Glib::signal_idle().connect_once([this, jobNumber](){
                m_current_work += 1;
                updateProgress(jobNumber);
                if (m_current_work == m_total_work)
                    m_button.set_sensitive(true);
            });
        }
    }

    void updateProgress(int jobNumber)
    {
        double prog = (double) m_current_work / (double) m_total_work;

        auto buf = m_lbl.get_buffer();
        auto it = buf->end();

        std::ostringstream msg;
        if (m_current_work != 0) msg << std::endl;
        msg << "Job " << jobNumber << " Done.";

        buf->insert(it, msg.str());

        m_progress_bar.set_text(msg.str());
        m_progress_bar.set_fraction(prog);
    }

    void onPressed()
    {
        m_button.set_sensitive(false);
        m_lbl.get_buffer()->set_text("");
        m_total_work = 100;
        m_current_work = 0;

        // Randomly enqueue the job that is to be completed.
        std::vector<int> jobs;
        jobs.reserve(m_total_work);
        for (int i = 0; i < m_total_work; i++) jobs.push_back(i);
        std::shuffle(jobs.begin(), jobs.end(), std::random_device());

        for (auto &&i : jobs)
        {
            m_blocking_queue.enqueue([i]()->int{
                return i;
            });
        }
    }
};

#endif //IDLEQUEUE_WINDOW_H
