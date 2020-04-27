#pragma once
#include <iostream>
#include <random>
#include <sstream>
#include "robot_dp.h"

class Simulator
{
public:
    Simulator() : max_time_(0.0), num_trials_(0), reng_(), // default random engine initialization for "deterministic randomness"
        x0_dist_(0, 2*M_PI), x1_dist_(0, 2*M_PI), x2_dist_(-M_PI, M_PI), x3_dist_(-M_PI, M_PI) {}
    ~Simulator()
    {
        delete sys_;
    }
    void initialize(const std::string &policy_logfile, const std::string &tracking_logfile_prefix,
                    DPRobot* sys, const double &max_time, const int &num_trials)
    {
        tracking_logfile_prefix_ = tracking_logfile_prefix;
        sys_ = sys;
        max_time_ = max_time;
        num_trials_ = num_trials;
        sys_->grid_.read_u_from_log(policy_logfile);
    }
    void run_sims()
    {
        // Run randomized trials and log results
        for (int idx = 0; idx < num_trials_; idx++)
        {
            std::cout << "TRIAL " << idx << std::endl;
            // output logfile
            std::stringstream ss;
            ss << tracking_logfile_prefix_ << "_trial" << idx << ".log";
            std::string ofname = ss.str();
            outfile_.open(ofname);

            // initial state
            double t = 0.0;
            Vector4d x_k = Vector4d(x0_dist_(reng_),
                                    x1_dist_(reng_),
                                    x2_dist_(reng_),
                                    x3_dist_(reng_));

            // simulate
            while (t <= max_time_)
            {
                // log time and state
                log(t);
                logVectors(x_k);

                // select appropriate control from policy
                double u = sys_->grid_.get_interpolated_u(x_k);

                // simulate system
                Vector4d x_kp1 = sys_->RK4(x_k, u, DT);
                x_k = x_kp1;
                t += DT;

                // break if things have gotten out of control
                if ((x_kp1.array() > 100.0).any())
                {
                    std::cout << "WARNING: break condition activated." << std::endl;
                    break;
                }
            }

            // close logfile
            outfile_.close();
        }
    }

private:
    template <typename... T>
    void log(T... data)
    {
        int dummy[sizeof...(data)] = { (outfile_.write((char*)&data, sizeof(T)), 1)... };
    }
    template <typename... T>
    void logVectors(T... data)
    {
        int dummy[sizeof...(data)] = { (outfile_.write((char*)data.data(), sizeof(typename T::Scalar)*data.rows()*data.cols()), 1)... };
    }

    std::string tracking_logfile_prefix_;
    DPRobot* sys_;
    double max_time_;
    int num_trials_;
    std::default_random_engine reng_;
    std::uniform_real_distribution<double> x0_dist_;
    std::uniform_real_distribution<double> x1_dist_;
    std::uniform_real_distribution<double> x2_dist_;
    std::uniform_real_distribution<double> x3_dist_;
    std::ofstream outfile_;

};
