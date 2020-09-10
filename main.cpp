#include <iostream>

//#include "controllers/notes_window.h"
//#include "sqlite3.h"
//#include "db_executor.h"
//#include "SQLiteCpp/Statement.h"
#include "gtkmm.h"
#include "glibmm.h"
#include "window.h"
#include "idle_queue.h"

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

int main (int argc, char *argv[])
{
    auto app = Gtk::Application::create("org.gnome.Notes", Gio::APPLICATION_FLAGS_NONE);

    app->signal_activate().connect(sigc::bind(&on_activate, app));

    auto ret = app->run(argc, argv);
    return ret;
}
