#include <synapse/branch/coroutine/coroutine_node.hpp>

thread_local std::any                                    co_yield_argument;
thread_local synapse::branch::coroutine::coroutine_node  co_thread_node   ;
thread_local synapse::branch::coroutine::coroutine_node *co_current_node  ;