#include <iostream>
#include <queue>

#include <any>
#include <synapse/context/switch.hpp>

namespace branch
{
    class branch_launcher { public: virtual void execute() = 0; };
    class branch_queue
    {
    public:
        using branch_t = size_t;

    public:
        void add_branch   (branch_launcher& br);
        void delete_branch();

    public:
        void next_branch();

    private:
        void                     exec_loop();
        std::queue<frame::frame> exec_queue;
        std::any                 exec_param;
    };

    template <typename _out, typename... _in>
    class branch : public branch_launcher
    {
    public:
        void execute() 
    };
}