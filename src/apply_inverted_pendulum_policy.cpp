#include <iostream>
#include "simulator.h"
#include "inverted_pendulum.h"

int main(int argc, char* argv[])
{
    const std::string policy_logfile = "inverted_pendulum_inputs_p01eps.log";
    const std::string outfile_prefix = "inverted_pendulum_tracking_p01eps";
    const double max_time = 10.0;
    const int num_trials = 400;
    DPRobot* sys = new InvertedPendulumDP();

    Simulator sim;
    sim.initialize(policy_logfile, outfile_prefix, sys, max_time, num_trials);
    std::cout << "Running sims..." << std::endl;
    sim.run_sims();
    std::cout << "...done." << std::endl;

    return 0;
}
