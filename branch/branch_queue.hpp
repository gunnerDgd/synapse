#include "branch.hpp"
#include <unordered_map>

namespace synapse
{
namespace branch
{
    using branch_id    = uint64_t;
    using branch_queue = std::unordered_map<uint64_t, branch_base*>;
    
    class dispatch
    {
    public:
        branch_id add_branch   (branch_base& br_base);
        void      delete_branch(branch_id    br_id)  { disp_rq.erase(br_id); }
        void      switch_branch()                    { frame::switch_frame(*disp_current, disp_loop_frame); }

    private:
        frame::frame* disp_current,
                      disp_loop_frame;
        
        branch_queue  disp_rq;

    private:
        void      disp_loop();
    };
}
}
branch_id synapse::branch::dispatch::add_branch(branch_base& br_base)
{
    auto   rq_it  = disp_rq.begin();
    for( ; rq_it != disp_rq.end() ; rq_it++)
        if(rq_it.second == nullptr)
        {
            rq_it.second = &br_base;
            return rq_it.first;
        }
    
    --rq_it;
    branch_id br_newid = rq_it.first;

    disp_rq.insert(std::make_pair(br_newid, &br_base));
}

void      synapse::branch::dispatch::disp_loop ()
{
    while(true)
    {
        for(auto& rq_it : disp_rq)
            rq_it.execute(disp_loop_frame);
    }
}