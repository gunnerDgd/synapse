#include "dispatcher.hpp"

thread_local context::dispatcher context_dispatcher;
thread_local uint64_t            context_instruction_pointer;