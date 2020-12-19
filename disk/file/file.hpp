#include <iostream>
#include <synapse/include/stream.hpp>
#include <synapse/disk/directory/directory.hpp>

namespace disk
{
    class file : public stream::stream, public disk::disk_object
    {
        public:
            file(std::string _name);

        private:
            std::string f_path;
    }
}