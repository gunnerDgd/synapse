#include <synapse/context/dispatcher/dispatcher.hpp>

void test_1(frame::dispatcher::dispatcher_entity* ent);
void test_2(frame::dispatcher::dispatcher_entity* ent);

int main()
{
    frame::dispatcher fr_disp;
    frame::dispatcher::dispatcher_entity* fr_main = new frame::dispatcher::dispatcher_entity;

    fr_main->current_dispatcher = &fr_disp;
    fr_main->current_frame      = new frame::frame;

    fr_disp.add_frame(test_1, fr_main, 64 * 1024);
}

void test_1(frame::dispatcher::dispatcher_entity* ent)
{
    std::cout << "Hello Test 1 #1\n";

    ent->current_dispatcher->add_frame(test_2, ent, 64 * 1024);
    std::cout << "Hello Test 1 #2\n";
    ent->current_dispatcher->switch_frame(test_2, ent);
}

void test_2(frame::dispatcher::dispatcher_entity* ent)
{
    std::cout << "Hello Test 2 #1\n";
    ent->current_dispatcher->switch_frame(test_1, ent);
    std::cout << "Hello Test 2 #2\n";
}