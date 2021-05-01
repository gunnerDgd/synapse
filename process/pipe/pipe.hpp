#include <synapse/synapse.hpp>
#include <synapse/device/file.hpp>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace synapse {
namespace process {

    class pipe : public synapse::io::io
    {
    public:
        using handle_t = int;

    public:
        pipe(std::string                  p_name,
             synapse::device::open_mode   p_open = synapse::device::open_mode::create,
             synapse::device::access_mode p_ac   = synapse::device::access_mode::all);

    private:
        handle_t pipe_handle;
    };

}
}

synapse::process::pipe::pipe(std::string                  p_name, 
                             synapse::device::open_mode   p_open,
                             synapse::device::access_mode p_ac)
{
    
}