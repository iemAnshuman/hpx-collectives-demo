# HPX Collectives Demo

Minimal demos built against HPX 2.0.0 on macOS (Apple Silicon).

- `hello_hpx.cpp` — Basic HPX locality startup
- `collective_demo.cpp` — all_reduce across localities with correctness verification

## Build
```bash
mkdir build && cd build
cmake -DHPX_DIR=/path/to/hpx/build/lib/cmake/HPX ..
make
```
