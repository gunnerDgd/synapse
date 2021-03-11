#include <synapse/branch/branch.hpp>

void test(int a, int b)
{
    std::cout << a << " " << b << std::endl;
}

int main()
{
    branch::branch_queue bq;
    branch::branch       br(bq, test, 1, 2);

    std::thread         bth(&branch::branch_queue::execute_branch, &bq);
    bth.join();
}