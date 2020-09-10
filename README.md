# idlequeue

This project shows a method of creating a mechanism to call back
to the GTK UI thread from a worker thread in gtkmm.

Note that no attempt has been made to have high performance and
that any performance would ultimately optimizations would
likely be capped by the Gtk::Dispatcher mechanism.

### Goal

GTK# and other gtk bindings supply a method to callback to the
main thread, which gtkmm does not. GTK# has Application.Invoke().
This is a potential solution which seems roughly equivalent
to those other solutions.
We could implement this as a global queue in the Application and
have a simple method that can be called to create callbacks.

### Architecture

The system utilizes two main components to work.

1. It has a thread safe queue, which it uses to pass std::function's to the
main thread.
1. It uses a single Gtk::Dispatcher to signal the main thread to check the
work queue for pending jobs.

Note that normal threading rules apply and there's plenty of ways
to shoot yourself in the foot with this method.

The callbacks guarantee no thread safety other than the queuing of the work.
