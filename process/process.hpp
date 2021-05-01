#include <synapse/process/process_flag.hpp>

namespace synapse {
namespace process {

    class process
    {
    public:
        using handle_t = pid_t;

    public:
        process (std::string pr_name);
        ~process();
        
        bool     start_process(char** pr_param)     ;
        int      wait_process ()                    ;
    
    public:
        handle_t native_handle()                    { return process_handle; }

    private:
        std::string             process_name  ;

        handle_t                process_handle;
        synapse::process::state process_state ;
    };

}
}

synapse::process::process::process (std::string pr_name)
    : process_name (std::move(pr_name)),
      process_state(synapse::process::state::standby) { }

bool     synapse::process::process::start_process(char** pr_param)
{
    process_handle     = fork();
    if(process_handle == 0) // Child Process
        execv(process_name.c_str(), pr_param); // End of the Current Image's Context.

    // Parent Process
    int pr_state,
        w_state = waitpid(process_handle, &pr_state, WNOHANG); // Check State of the Process.
    
    if(w_state == -1 || WIFSIGNALED(pr_state) == true)
        return false;
    else
    {
        process_state = synapse::process::state::running;
        return          true ;
    }
}
int     synapse::process::process::wait_process ()
{
    int      process_res;
    handle_t process_handle = ::wait(process_handle, &process_res);

    if(process_handle == -1 || WIFSIGNALED(process_res))
        return -1;
    else
        return WEXITSTATUS(process_res);
}   