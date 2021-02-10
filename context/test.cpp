#include <synapse/context/context.hpp>
#include <iostream>

int main()
{
    context::context ctx;

    std::cout << ctx.ctx_previous[frame::previous::index::previous_instruction] << std::endl;
}