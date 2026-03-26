#include <hpx/hpx_init.hpp>
#include <hpx/algorithm.hpp>
#include <hpx/numeric.hpp>
#include <iostream>
#include <vector>
#include <chrono>

// Simple row-major matrix using a flat vector
struct Matrix {
    std::size_t rows, cols;
    std::vector<double> data;

    Matrix(std::size_t r, std::size_t c, double val = 0.0)
      : rows(r), cols(c), data(r * c, val) {}

    double& operator()(std::size_t i, std::size_t j) { return data[i * cols + j]; }
    double  operator()(std::size_t i, std::size_t j) const { return data[i * cols + j]; }
};

// Parallel matrix multiplication using hpx::for_loop with par policy
// C = A * B where A is (M x K), B is (K x N), C is (M x N)
Matrix parallel_matmul(const Matrix& A, const Matrix& B)
{
    const std::size_t M = A.rows;
    const std::size_t K = A.cols;
    const std::size_t N = B.cols;

    Matrix C(M, N, 0.0);

    // Parallelize over rows of C
    hpx::experimental::for_loop(
        hpx::execution::par, 
        std::size_t(0), M,
        [&](std::size_t i) {
            for (std::size_t j = 0; j < N; ++j) {
                double sum = 0.0;
                for (std::size_t k = 0; k < K; ++k) {
                    sum += A(i, k) * B(k, j);
                }
                C(i, j) = sum;
            }
        });

    return C;
}

// Sequential reference for correctness verification
Matrix sequential_matmul(const Matrix& A, const Matrix& B)
{
    const std::size_t M = A.rows;
    const std::size_t K = A.cols;
    const std::size_t N = B.cols;

    Matrix C(M, N, 0.0);
    for (std::size_t i = 0; i < M; ++i)
        for (std::size_t j = 0; j < N; ++j)
            for (std::size_t k = 0; k < K; ++k)
                C(i, j) += A(i, k) * B(k, j);
    return C;
}

bool verify(const Matrix& C1, const Matrix& C2, double tol = 1e-9)
{
    if (C1.rows != C2.rows || C1.cols != C2.cols) return false;
    for (std::size_t i = 0; i < C1.data.size(); ++i) {
        if (std::abs(C1.data[i] - C2.data[i]) > tol) return false;
    }
    return true;
}

int hpx_main(hpx::program_options::variables_map& vm)
{
    const std::size_t N = vm["size"].as<std::size_t>();

    // Initialize matrices with deterministic values
    Matrix A(N, N), B(N, N);
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            A(i, j) = static_cast<double>(i + j) / N;
            B(i, j) = static_cast<double>(i * j + 1) / N;
        }
    }

    // Parallel multiplication
    auto t1 = std::chrono::high_resolution_clock::now();
    Matrix C_par = parallel_matmul(A, B);
    auto t2 = std::chrono::high_resolution_clock::now();
    double par_ms = std::chrono::duration<double, std::milli>(t2 - t1).count();

    // Sequential multiplication for verification
    auto t3 = std::chrono::high_resolution_clock::now();
    Matrix C_seq = sequential_matmul(A, B);
    auto t4 = std::chrono::high_resolution_clock::now();
    double seq_ms = std::chrono::duration<double, std::milli>(t4 - t3).count();

    // Verify correctness
    bool correct = verify(C_par, C_seq);

    std::cout << "Matrix size: " << N << " x " << N << "\n"
              << "Sequential:  " << seq_ms << " ms\n"
              << "Parallel:    " << par_ms << " ms\n"
              << "Speedup:     " << seq_ms / par_ms << "x\n"
              << "Correctness: " << (correct ? "PASS" : "FAIL") << "\n";

    return hpx::finalize();
}

int main(int argc, char* argv[])
{
    hpx::program_options::options_description desc("Options");
    desc.add_options()
        ("size", hpx::program_options::value<std::size_t>()->default_value(512),
         "Matrix dimension (N x N)");

    hpx::init_params init_args;
    init_args.desc_cmdline = desc;

    return hpx::init(argc, argv, init_args);
}
