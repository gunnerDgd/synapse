#include <synapse/memory/memory.hpp>

#include <string_view>
#include <unordered_map>

namespace synapse {
namespace network {
namespace http    {

    class response_prefix
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

    class response
    {
    public:
        response(synapse::mem);

    protected:
        response_prefix                                        prefix ;
        std::unordered_map<std::string_view, std::string_view> context;
    };

}
}
}