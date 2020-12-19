#include <synapse/disk/disk_object.hpp>

#include <iostream>
#include <string>

namespace disk
{
    class directory : public disk::disk_object
    {
        public:
            directory(std::string _path);

            void         add_object   (disk_object* _d);
            void         delete_object(disk_object* _d);

            disk_object* operator[]   (std::string _name);

        private:
            

    }
}