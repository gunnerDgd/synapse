#include <synapse/synapse.hpp>
#include <synapse/branch/branch.hpp>

#include <unordered_map>
#include <thread>
#include <any>

namespace  synapse   {
namespace  branch    {
namespace  coroutine {

    struct coroutine_node
    {
        coroutine_node                               *co_parent = nullptr;
        std::unordered_map<uint64_t, coroutine_node*> co_child           ;

        synapse::branch::branch                       co_branch          ;
    };

    extern thread_local std::any                                    co_yield_argument;
    extern thread_local synapse::branch::coroutine::coroutine_node  co_thread_node   ;
    extern thread_local synapse::branch::coroutine::coroutine_node *co_current_node  ;
}
}
}