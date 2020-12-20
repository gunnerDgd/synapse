#include <iostream>

namespace disk
{
    enum class type
    {
        directory,
        file
    };

    class disk_object
    {
        public:
            disk_object(disk::type _type) : object_type(_type) {}
            disk::type                      object_type;
    };
}