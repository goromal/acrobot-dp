#include <iostream>
#include <chrono>
#include "inverted_pendulum.h"

int main(int argc, char *argv[])
{
    const int max_iter = 100; // 50 for 10 seconds
    const double epsilon = 0.01;
    const std::string logname = "inverted_pendulum_inputs_p01eps.log";
//    const double epsilon = 1.0;
//    const std::string logname = "inverted_pendulum_inputs_1eps.log";
//    const double epsilon = 10.0;
//    const std::string logname = "inverted_pendulum_inputs_10eps.log";
    InvertedPendulumDP DP;

    double max_diff = 1000;
    auto start_time = std::chrono::high_resolution_clock::now();
    DP.reset();
    int iter = 0;
    while (abs(max_diff) > epsilon && iter < max_iter)
    {
        std::cout << "ITER #" << iter << ":\n";
        max_diff = DP.iterate();
        auto current_time = std::chrono::high_resolution_clock::now();
        std::cout << "\tmax_diff = " << max_diff
                  << "\n\ttime = " << std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count() << " s" << std::endl;
        iter++;
    }
    std::cout << "Logging inputs..." << std::endl;
    DP.grid_.write_u_to_log(logname);
    std::cout << "Done." << std::endl;

    return 0;
}
