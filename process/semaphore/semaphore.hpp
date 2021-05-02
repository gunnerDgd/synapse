#include <iostream>
#include <string>

#include <sys/types.h> 
#include <sys/sem.h> 
#include <sys/ipc.h> 
#include <unistd.h>

namespace synapse {
namespace process {

    class semaphore
    {
    public:
        using handle_t = int;
    public:
        semaphore (std::string sem_id);
        ~semaphore()                  { this->release(); }

    public:
        bool acquire();
        bool release();

    private:
        key_t    semaphore_key;
        handle_t semaphore_handle;
    };
}
}

synapse::process::semaphore::semaphore(std::string sem_id)
    : semaphore_key(std::hash<std::string>{}(sem_id))
{
    semaphore_handle    = ::semget(semaphore_key, 1, IPC_CREAT | 0666);
    if(semaphore_handle < 0)
        return;

    union sem_ctrl
    {
        int                 sc_val;
        struct semid_ds    *sc_buf;
        unsigned short int *sc_ary;
    }                       sem_init;
    sem_init.sc_val  = 1;
    int sem_init_res = ::semctl(semaphore_handle, 0, SETVAL, sem_init);
}

void synapse::process::semaphore::acquire()
{
    struct sembuf ac_ctrl = { 1, -1, SEM_UNDO };
    int ac_res            = ::semop(semaphore_handle, &ac_ctrl, 1);

    return (ac_res < 0) ? false : true;
}

void synapse::process::semaphore::release()
{
    struct sembuf rl_ctrl = { 1, 1, SEM_UNDO };
    int           rl_res  = ::semop(semaphore_handle, &rl_ctrl, 1);

    return (rl_res < 0) ? false : true;
}