#include "event.hpp"

template          <size_t N>
synchronous::event<N>::event()
{
    std::srand(time());
    while(event_context != -1)
        event_context = semget(std::rand() % 1024,
                               1, IPC_CREAT | IPC_PRIVATE);

    sem_ctrl.val = 0;
    semctl(event_context, 0, SETVAL, &sem_ctrl);
}

template          <size_t N>
void synchronous::event<N>::await()
{
    struct sembuf event_await = { 0, -1, SEM_UNDO };
    semop(event_context, &event_await, 1);
}

template          <size_t N>
void synchronous::event<N>::alert()
{
    struct sembuf event_await = { 0, N, SEM_UNDO };
    semop(event_context, &event_await, 1);
}