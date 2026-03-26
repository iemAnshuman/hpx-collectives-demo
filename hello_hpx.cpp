#include <hpx/hpx_init.hpp>
#include <iostream>

int hpx_main()
{
    std::cout << "Hello from HPX locality "
              << hpx::get_locality_id()
              << std::endl;

    return hpx::finalize();
}

int main(int argc, char* argv[])
{
    return hpx::init(argc, argv);
}
