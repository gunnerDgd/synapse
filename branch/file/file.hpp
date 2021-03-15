#include <synapse/context/branch/switch.hpp>
#include <synapse/disk/file/file_flag.hpp>
#include <synapse/stream/stream.hpp>

namespace synapse
{
namespace branch_io
{
    class file : public stream::stream
    {
    public:
        file(std::string n, disk::open_mode o, disk::access_mode a);
        file() : stream::stream(io_mode::non_block)                { }

    public:
        void open (std::string n, disk::open_mode o, disk::access_mode a);
        void close();

    public:
        void attach_dispatcher(branch::dispatch& d) { file_dispatcher = &d; }
        void detach_dispatcher()                    { file_dispatcher = nullptr; }
    
    private:
        branch::dispatch* file_dispatcher;

    public:
        size_t read (uint8_t* c, size_t s) override;
        size_t write(uint8_t* c, size_t s) override;

    private:
        disk::handle_type file_handle;
        size_t            file_size  ;
#ifdef WIN32_MODE
        OVERLAPPED        file_overlapped;
#endif
    }
}
}