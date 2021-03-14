#include <synapse/context/switch.hpp>
#include <unordered_map>
#include <random>

#include <thread>
#include <chrono>

namespace synapse
{
namespace branch
{
    class branch_base  : public frame::frame
    { 
    public:
        branch_base()                            : frame(64 * 1024) {} 
        virtual void execute(frame& prev) = 0; 
    };
    
    using branch_id    = uint32_t;
    using branch_queue = std::unordered_map<branch_id, branch_base*>;

    class dispatch
    {
    public:
        void start_dispatch() { disp_loop(); }

    public:
        branch_id start_branch(branch_base& br_base);
        
        void      end_branch  (branch_id    br_id);
        void      end_branch  ()                  ;

        void      switch_branch()                 { frame::switch_frame(*disp_current->second, disp_loop_frame); }

    private:
        frame::frame           disp_loop_frame;
        branch_queue::iterator disp_current;
        
        branch_queue  disp_rq;
        void          disp_loop();
    };
}
} using namespace synapse;

branch::branch_id branch::dispatch::start_branch(branch_base& br_base)
{
    std::random_device br_gen;
    branch_id          br_id;

    while         (disp_rq.find(br_id = br_gen()) != disp_rq.end());
    disp_rq.insert(std::make_pair(br_id, &br_base));

    return br_id;
}

void      branch::dispatch::end_branch  (branch_id    br_id)
{
    if(br_id == disp_current->first) end_branch();
    disp_rq.erase(br_id);
}

void      branch::dispatch::end_branch  ()
{
    frame::frame* prev_fr = disp_current->second;
    disp_rq.erase(disp_current);

    frame::switch_frame(*prev_fr, disp_loop_frame);
}

void      branch::dispatch::disp_loop ()
{
    while(true)
    {
        while(disp_rq.empty())
            std::this_thread::sleep_for(std::chrono::nanoseconds(10));
        
        disp_current        = disp_rq.begin();
        while(disp_current != disp_rq.end())
        {
            disp_current->second->execute(disp_loop_frame);
            if(!disp_rq.empty()) disp_current++;
            else                 break;
        }
    }
}