#include <synapse/context/branch/branch.hpp>

void test_1(synapse::branch::dispatch& d)
{
    for(int i = 0 ; i < 5 ; i++)
    {
        std::cout << "Test #1 :: " << i << std::endl;
        d.switch_branch();
    }

    d.end_branch();
}

void test_2(synapse::branch::dispatch& d)
{
    for(int i = 0 ; i < 5 ; i++)
    {
        std::cout << "Test #2 :: " << i << std::endl;
        d.switch_branch();
    }

    d.end_branch();
}

int main()
{
    synapse::branch::dispatch d;
    synapse::branch::branch<void(void)> br1(d, test_1), br2(d, test_2);

    d.start_dispatch();
}