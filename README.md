# HPX Collectives & Parallel Computing Demo

Minimal demos built against HPX 2.0.0 on macOS (Apple Silicon).

## Programs

- **hello_hpx.cpp** — Basic HPX locality startup
- **collective_demo.cpp** — Distributed all_reduce across localities with correctness verification
- **matrix_multiplication_demo.cpp** — Parallel matrix multiplication using `hpx::for_loop` with `hpx::execution::par`, verified against sequential reference

## Build
```bash
mkdir build && cd build
cmake -DHPX_DIR=/path/to/hpx/build/lib/cmake/HPX ..
make
```

## Run
```bash
./hello_hpx
./collective_demo
./matrix_multiplication_demo --size=512
```

## Author

Anshuman Agrawal — [github.com/iemAnshuman](https://github.com/iemAnshuman)

GSoC 2026 proposal: Hierarchical Collectives for HPX (STE||AR Group)
