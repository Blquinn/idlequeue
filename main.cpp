#include <iostream>

#include "gtkmm.h"
#include "glibmm.h"
#include "window.h"

static void on_activate (Glib::RefPtr<Gtk::Application> app)
{
    Gtk::Window *window = app->get_active_window();

    if (!window)
    {
        window = new Window();
        app->add_window(*window);
    }

    window->present();
}

bool foo(void *user_data) {
    std::cout << "In idle" << std::endl;
    return false;
}

int main (int argc, char *argv[])
{
    auto app = Gtk::Application::create("org.gnome.Notes", Gio::APPLICATION_FLAGS_NONE);

//    auto func = [](void *data) -> bool {
//    };
    g_idle_add(G_SOURCE_FUNC(foo), nullptr);

    app->signal_activate().connect(sigc::bind(&on_activate, app));

    auto ret = app->run(argc, argv);
    return ret;
}
