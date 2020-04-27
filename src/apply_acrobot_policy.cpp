#include <iostream>
#include "simulator.h"
#include "acrobot.h"

int main(int argc, char* argv[])
{
    const std::string policy_logfile = "acrobot_inputs_p01eps.log";
    const std::string outfile_prefix = "acrobot_tracking_p01eps";
    const double max_time = 30.0;
    const int num_trials = 400;
    AcrobotDP* sys = new AcrobotDP();

    Simulator sim;
    sim.initialize(policy_logfile, outfile_prefix, sys, max_time, num_trials);
    std::cout << "Running sims..." << std::endl;
    sim.run_sims();
    std::cout << "...done." << std::endl;

    return 0;
}
