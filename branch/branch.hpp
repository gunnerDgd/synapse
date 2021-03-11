#include <iostream>
#include <chrono>
#include <thread>
#include <list>

#include <any>
#include <synapse/context/switch.hpp>

namespace branch
{
    enum  branch_optype
    {
        terminate,
        normal
    };

    class branch_launcher : public frame::frame
    { 
    public: 
        virtual void execute(frame&) = 0; 
    };
    
    class branch_queue
    {
    public:
        branch_queue       () {}
        void execute_branch() { exec_loop(); }

    public:
        void add_branch   (branch_launcher& br) { exec_queue.push_back(br); }
        void delete_branch()                    { loop_optype         = branch_optype::terminate;
                                                  frame::switch_frame(*exec_current, loop_frame); }
        void next_branch  ()                    { frame::switch_frame(*exec_current, loop_frame); }

    private:
        void                    exec_loop();
        frame::frame            loop_frame ;
        branch_optype           loop_optype = branch_optype::normal; 

    private:
        std::list<branch_launcher> exec_queue            ;
        frame::frame*              exec_current = nullptr;
    };

    template <typename T, typename... Arg>
    class branch : public branch_launcher
    {
    public:
        branch        (branch_queue& bq, T&& bfp, Arg&&... barg)
            : br_queue(bq),
              br_args (std::make_tuple(std::move(barg...))),
              br_fp   (bfp)                                { }

        void execute(frame& prev) override // prev = Execution Loop's Frame.
        {
            if(br_isExecuted)
                switch_frame(prev, *this);
            else
                switch_frame(prev, *this, br_fp, br_args);
        }
        
    private:
        std::tuple<Arg...> br_args;
        T                  br_fp  ;
        
        bool               br_isExecuted = false;
        branch_queue&      br_queue;
    };
}

void branch::branch_queue::exec_loop()
{
    while(true)
    {
        if (exec_queue.empty())
            std::this_thread::sleep_for(1us);
        for(auto exec_it = exec_queue.begin() ; exec_it != exec_queue.end() ; exec_it++)
        {
            (*exec_it).execute(*exec_current);

            if(loop_optype == branch_optype::terminate) {
                loop_optype = branch_optype::normal;
                exec_queue.erase(exec_it);
            }
        }
    }}