#include "lollipop_dp.h"

namespace dynamic_programming {

void DPLollipop::dynamics(Eigen::Vector4d &x_dot, Eigen::Vector4d &x, const double &u)
{
    double theta1     = x(1, 0);
    double theta2     = x(2, 0);
    double theta1_dot = x(3, 0);
    double theta2_dot = x(4, 0);

    // ++++
}

void DPLollipop::createStateGrid()
{
    // ++++
}

} // end namespace dynamic_programming
