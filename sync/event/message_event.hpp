#include <synapse/sync/event/event.hpp>

namespace synapse     {
namespace synchronize {

    template <typename T>
    class message_event : private synapse::synchronize::event
    {
    public:
        void signal_message(T&  sig_msg);
        void signal_message(T&& sig_msg);
        T&   wait_message  ();

    private:
        T                  signal_message;
    };
}
}

template <typename T>
void synapse::synchronize::message_event<T>::signal_message(T& sig_msg)
{
    signal_message = sig_msg;
    this           ->set();
}

template <typename T>
void synapse::synchronize::message_event<T>::signal_message(T&& sig_msg)
{
    signal_message = sig_msg;
    this           ->set();
}

template <typename T>
T& synapse::synchronize::message_event<T>::wait_message  ()
{
    this ->wait();
    return signal_message;
}