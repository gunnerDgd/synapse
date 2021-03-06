#include <synapse/memory/memory.hpp>

#include <string_view>
#include <unordered_map>

namespace synapse {
namespace network {
namespace http    {
namespace prefix  {

    class response
    {
    public:
        response_prefix(std::string prefix_str);
        

        const std::string& http_version  () { return prefix_http_version  ; }
        void               http_version  () { }
        const std::string& status_message() { return prefix_status_message; }
        const uint8_t&     status_code   () { return prefix_status_code   ; }

    private:
        std::string prefix_http_version  ,
                    prefix_status_message;
        uint8_t     prefix_status_code   ;
    };

    class request
    {
    public:
        request(std::string prefix_str);
    };
}
}
}
}

