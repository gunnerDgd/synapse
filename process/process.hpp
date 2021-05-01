#include <unistd.h>

#include <iostream>
#include <string>

namespace synapse {
namespace process {

    class process
    {
    public:
        using handle_t = int;

    public:
        process(std::string pr_name);

    private:
        std::string process_name  ;
        handle_t    process_handle;
    }

}
}