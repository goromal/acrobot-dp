#pragma once

#include "robot_dp.h"

#define UVAL 5.0 // 59%

class InvertedPendulumDP : public DPRobot
{
public:
    InvertedPendulumDP() : DPRobot() {}
    void dynamics(Vector4d &x_dot, const Vector4d &x, const double &u) override
    {
        const static double m = 1.0;
        const static double l = 1.0;
        const static double g = 10.0;

        x_dot(THETA1)    = x(THETA1DOT);
        x_dot(THETA2)    = 0.0;
        x_dot(THETA1DOT) = (u - m*g*l*sin(x(THETA1))) / (m*l*l);
        x_dot(THETA2DOT) = 0.0;
    }
    double isSuccess(const int &i, const int &j, const int &k, const int &l) override
    {
        if (i == 17 && (k == 17 || k == 18))
            return 1.0;
        else
            return 0.0;
    }
    void populateU() override
    {
        u[0] = -UVAL;
        u[1] =   0.0;
        u[2] =  UVAL;
    }
};
