#include <hpx/hpx_init.hpp>
#include <hpx/modules/collectives.hpp>
#include <iostream>

constexpr char const* all_reduce_name = "/demo/all_reduce";

int hpx_main()
{
    auto id = hpx::get_locality_id();
    auto num = hpx::get_num_localities(hpx::launch::sync);

    std::uint32_t local_value = id + 1;

    auto comm = hpx::collectives::create_communicator(
        all_reduce_name,
        hpx::collectives::num_sites_arg(num),
        hpx::collectives::this_site_arg(id));

    auto result = hpx::collectives::all_reduce(
        comm, local_value, std::plus<std::uint32_t>{}).get();

    std::uint32_t expected = num * (num + 1) / 2;

    std::cout << "Locality " << id
              << ": local=" << local_value
              << " reduced=" << result
              << " expected=" << expected
              << " [" << (result == expected ? "PASS" : "FAIL") << "]"
              << std::endl;

    return hpx::finalize();
}

int main(int argc, char* argv[])
{
    return hpx::init(argc, argv);
}
