#pragma once

#include "robot_dp.h"

#define UVAL 5.0

class AcrobotDP : public DPRobot
{
public:
    AcrobotDP() : DPRobot() {}
    void dynamics(Vector4d &x_dot, const Vector4d &x, const double &u) override
    {
        // parameters
        const static double g = 9.8;
        const static double l1 = 1.0;
        const static double lc1 = 0.5;
        const static double m1 = 1.0;
        const static double I1 = m1 * l1 * l1 / 12.0;
        const static double l2 = 2.0;
        const static double lc2 = 1.0;
        const static double m2 = 1.0;
        const static double I2 = m2 * l2 * l2 / 12.0;

        // intermediate state values
        double d1 = m1*lc1*lc1 + m2*(l1*l1 + lc2*lc2 + 2*l1*lc2*cos(x(THETA2))) + I1 + I2;
        double d2 = m2*(lc2*lc2 + l1*lc2*cos(x(THETA2))) + I2;
        double phi2 = m2*lc2*g*cos(x(THETA1) + x(THETA2) - M_PI/2);
        double phi1 = -m2*l1*lc2*x(THETA2DOT)*x(THETA2DOT)*sin(x(THETA2)) -
                      2*m2*l1*lc2*x(THETA2DOT)*x(THETA1DOT)*sin(x(THETA2)) +
                      (m1*lc1 + m2*l1)*g*cos(x(THETA1)-M_PI/2) + phi2;

        // dynamics
        x_dot(THETA1)    = x(THETA1DOT);
        x_dot(THETA2)    = x(THETA2DOT);
        x_dot(THETA2DOT) = (u + phi1*d2/d1 - m2*l1*lc2*x(THETA1DOT)*x(THETA1DOT)*sin(x(THETA2)) - phi2)/(m2*lc2*lc2 + I2 - d2*d2/d1);
        x_dot(THETA1DOT) = -(d2*x_dot(THETA2DOT) + phi1)/d1;
    }
    double isSuccess(const int &i, const int &j, const int &k, const int &l) override
    {
        static const double h = 2.0; // goal height
        static const double l1 = 1.0;
        static const double l2 = 2.0;

        Vector4d x = grid_.x_at(i, j, k, l);
        if (-l1*cos(x(THETA1)) - l2*cos(x(THETA1) + x(THETA2)) >= h)
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
