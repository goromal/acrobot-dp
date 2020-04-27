#pragma once
#include <Eigen/Core>
#include <math.h>
#include "state_grid.h"

#define             DT                     0.2
#define         GAMMAV                    0.90
#define SUCCESS_REWARD                  1000.0
#define BOUNDS_PENALTY                 -1000.0

class DPRobot // meant to be inherited from--only for "acrobot-like" robots!
{
public:
    DPRobot() : max_delta_(0.0), dt(DT), gamma_(GAMMAV) {}
    ~DPRobot()
    {
        delete[] Vs;
        delete[] rs;
    }
    void reset()
    {
        // Initialize Grids
        for (int i = 0; i < NUM_TH1; i++)
            for (int j = 0; j < NUM_TH2; j++)
                for (int k = 0; k < NUM_THD1; k++)
                    for (int l = 0; l < NUM_THD2; l++)
                    {
                        state_grid_index(rs, i, j, k, l) = SUCCESS_REWARD * isSuccess(i,j,k,l);
                        state_grid_index(Vs, i, j, k, l) = 0.0;
                    }
        populateU();
    }
    virtual void dynamics(Vector4d &x_dot, const Vector4d &x, const double &u) = 0;
    virtual double isSuccess(const int &i, const int &j, const int &k, const int &l) = 0; // define the success state, 1.0 = success, 0.0 = failure
    virtual void populateU() = 0;
    double iterate()
    {
        max_delta_ = 0.0;
        for (int i = 0; i < NUM_TH1; i++)
        {
            for (int j = 0; j < NUM_TH2; j++)
            {
                for (int k = 0; k < NUM_THD1; k++)
                {
                    for (int l = 0; l < NUM_THD2; l++)
                    {
                        Vector4d x = grid_.x_at(i,j,k,l);
                        double max_Vstar = -1.0e9;

                        for (int m = 0; m < 3; m++)
                        {
                            Vector4d x_kp1 = RK4(x, u[m], dt);
                            double Vstar = get_interpolated_score(x_kp1);
                            if (Vstar > max_Vstar)
                            {
                                max_Vstar = Vstar;
                                grid_.set_u_at(u[m],i,j,k,l);
                            }
                        }

                        double delta = max_Vstar - state_grid_index(Vs, i, j, k, l);
                        if (abs(delta) > abs(max_delta_))
                            max_delta_ = delta;

                        state_grid_index(Vs, i, j, k, l) = max_Vstar; // NOT += ^_^
                    }
                }
            }
        }
        return max_delta_;
    }
    Vector4d RK4(const Vector4d& x, const double &u, const double &dt)
    {
        dynamics(k1_, x, u);

        x2_ = x;
        x2_ += k1_ * (dt/2.0);
        dynamics(k2_, x2_, u);

        x3_ = x;
        x3_ += k2_ * (dt/2.0);
        dynamics(k3_, x3_, u);

        x4_ = x;
        x4_ += k3_ * dt;
        dynamics(k4_, x4_, u);

        return x + (k1_ + k2_*2.0 + k3_*2.0 + k4_) * (dt / 6.0);
    }
    StateGrid grid_;

protected:
    double u[3];

private:
    double get_interpolated_score(const Vector4d &x)
    {
        int i_l, i_u, j_l, j_u, k_l, k_u, l_l, l_u;
        double i_frac, j_frac, k_frac, l_frac;
        grid_.get_interpolation_indices(x, i_l, i_u, i_frac,
                                           j_l, j_u, j_frac,
                                           k_l, k_u, k_frac,
                                           l_l, l_u, l_frac);
        double r = natural_neighbors_4dgrid({i_u, j_u, k_u, l_u}, {i_l, j_l, k_l, l_l},
                                            {i_frac, j_frac, k_frac, l_frac}, rs, BOUNDS_PENALTY);
        double V = natural_neighbors_4dgrid({i_u, j_u, k_u, l_u}, {i_l, j_l, k_l, l_l},
                                            {i_frac, j_frac, k_frac, l_frac}, Vs, BOUNDS_PENALTY);
        return r + gamma_ * V; // Discounted Bellman Update (16.413 Lec 9.23.19, slide 40
    }

    Vector4d k1_;
    Vector4d k2_;
    Vector4d k3_;
    Vector4d k4_;
    Vector4d x2_;
    Vector4d x3_;
    Vector4d x4_;
    double max_delta_;
    double dt;
    double gamma_;
    double *rs = new double[NUM_TH1*NUM_TH2*NUM_THD1*NUM_THD2];
    double *Vs = new double[NUM_TH1*NUM_TH2*NUM_THD1*NUM_THD2];
};
