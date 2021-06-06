#include <synapse/memory/vmem/vmem.hpp>
#include <synapse/socket/socket.hpp>
#include <unordered_map>

namespace synapse {
namespace network {
namespace http    {

    class reader
    {
    public:
        using prefix_callback = std::function<int(uint8_t, std::string_view, std::string_view)>;
        using header_callback = std::function<int(std::string_view, std::string_view)>;

    public:
        reader(synapse::network::socket& r_sock);

    protected:
        synapse::network::socket& reader_socket;
        synapse::memory::memory * reader_buffer;
        std::unordered_map<std::string, >
    };

}
}
}

synapse::network::http::reader::reader(synapse::network::socket& r_sock)
    : reader_socket(r_sock),
      reader_buffer(new synapse::memory::vmem(4096))  { }

bool synapse::network::http::reader::receive_response()
{
    if(reader_buffer->memory_status() != synapse::memory::memory_state::normal) return false;

    if(!reader_socket.recv(*reader_buffer)) return false;
    else                                    return true ;
}

std::string_view 