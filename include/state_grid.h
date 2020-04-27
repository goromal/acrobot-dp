#pragma once
#include <math.h>
#include <string.h>
#include <Eigen/Core>
#include <fstream>
#include <iostream>

#define        NUM_TH1                       18
#define        NUM_TH2                       36
#define          DELTA          (10.0*M_PI/180.0)
#define          TH1_0           (5.0*M_PI/180.0)
#define          TH2_0           (5.0*M_PI/180.0)

////#define       NUM_THD1                       72
////#define       NUM_THD2                       72
////#define         THD1_0 (-2.0*M_PI+5.0*M_PI/180.0)
////#define         THD2_0 (-2.0*M_PI+5.0*M_PI/180.0)

//#define       NUM_THD1                      144
////#define       NUM_THD2                      144
//#define       NUM_THD2                      324
//#define         THD1_0 (-4.0*M_PI+5.0*M_PI/180.0)
////#define         THD2_0 (-4.0*M_PI+5.0*M_PI/180.0)
//#define         THD2_0 (-9.0*M_PI+5.0*M_PI/180.0)

using namespace Eigen;

enum {THETA1 = 0, THETA2 = 1, THETA1DOT = 2, THETA2DOT = 3};

double& state_grid_index(double *array, const int &i, const int &j, const int &k, const int &l)
{
    return array[i * NUM_TH2*NUM_THD1*NUM_THD2 + j * NUM_THD1*NUM_THD2 + k * NUM_THD2 + l];
}

double natural_neighbors_4dgrid(const int (&highs)[4], const int (&lows)[4], const double (&fracs)[4],
                                double* data, const double &kl_ob_val = 0.0) // kl_ob_val = value to use for out-of-bounds velocity indices
{
    static const int DIMENSION = 4;
    double interpolation = 0.0;
    for (int d_idx = 0; d_idx < (1 << DIMENSION); d_idx++) // 2^4 = 16 surrounding interpolation points
    {
        // Calculate boolean combination from d_idx
        bool switch1 = d_idx & (1 << 0);
        bool switch2 = d_idx & (1 << 1);
        bool switch3 = d_idx & (1 << 2);
        bool switch4 = d_idx & (1 << 3);

        // Determine which interpolation point location to select
        int i = switch1 ? highs[0] : lows[0];
        int j = switch2 ? highs[1] : lows[1];
        int k = switch3 ? highs[2] : lows[2];
        int l = switch4 ? highs[3] : lows[3];

        // Calculate weight for the interpolation point in the sum
        double X = switch1 ? fracs[0] : (1-fracs[0]);
        double Y = switch2 ? fracs[1] : (1-fracs[1]);
        double Z = switch3 ? fracs[2] : (1-fracs[2]);
        double W = switch4 ? fracs[3] : (1-fracs[3]);

        // Determine whether to use data value or out-of-bounds (only possible with velocity) value
        double interp_point;
        if ((k > NUM_THD1-1) || (k < 0) || (l > NUM_THD2-1) || (l < 0))
            interp_point = kl_ob_val;
        else
            interp_point = state_grid_index(data, i, j, k, l);

        interpolation += X * Y * Z * W * interp_point;
    }
    return interpolation;
}

class StateGrid
{
public:
    StateGrid()
    {
        for (int i = 0; i < NUM_TH1; i++)  x1[i] = TH1_0 + i*DELTA;
        for (int i = 0; i < NUM_TH2; i++)  x2[i] = TH2_0 + i*DELTA;
        for (int i = 0; i < NUM_THD1; i++) x3[i] = THD1_0 + i*DELTA;
        for (int i = 0; i < NUM_THD2; i++) x4[i] = THD2_0 + i*DELTA;
        memset(us, 0, sizeof(us));
    }
    ~StateGrid()
    {
        delete[] us;
    }
    Vector4d x_at(const int &i, const int &j, const int &k, const int &l)
    {
        return Vector4d(x1[i], x2[j], x3[k], x4[l]);
    }
    double u_at(const int &i, const int &j, const int &k, const int &l)
    {
        if (i < 0 || i >= NUM_TH1 || j < 0 || j >= NUM_TH2 || k < 0 || k >= NUM_THD1 ||
            l < 0 || l >= NUM_THD2)
            return 0.0;
        else
            return state_grid_index(us, i, j, k, l);
    }
    void set_u_at(const double &u, const int &i, const int &j, const int &k, const int &l)
    {
        state_grid_index(us, i, j, k, l) = u;
    }
    void get_interpolation_indices(const Vector4d &x_ref, int &il, int &iu, double &ifrac,
                                                          int &jl, int &ju, double &jfrac,
                                                          int &kl, int &ku, double &kfrac,
                                                          int &ll, int &lu, double &lfrac)
    {
        double x1 = x_ref(0);
        double x2 = x_ref(1);
        double x3 = x_ref(2);
        double x4 = x_ref(3);
        bool flipped = get_x1_bounds(x_ref(0), il, iu, ifrac);
        get_x2_bounds(flipped ? -x_ref(1) : x_ref(1), jl, ju, jfrac);
        get_x3_bounds(flipped ? -x_ref(2) : x_ref(2), kl, ku, kfrac);
        get_x4_bounds(flipped ? -x_ref(3) : x_ref(3), ll, lu, lfrac);
    }
    double get_interpolated_u(const Vector4d &x)
    {
        int i_l, i_u, j_l, j_u, k_l, k_u, l_l, l_u;
        double i_frac, j_frac, k_frac, l_frac;
        get_interpolation_indices(x, i_l, i_u, i_frac, j_l, j_u, j_frac,
                                     k_l, k_u, k_frac, l_l, l_u, l_frac);
        return natural_neighbors_4dgrid({i_u, j_u, k_u, l_u}, {i_l, j_l, k_l, l_l},
                                        {i_frac, j_frac, k_frac, l_frac}, us, 0.0);
    }
    void write_u_to_log(const std::string &logname)
    {
        // log all doubles as one long string of doubles
        std::ofstream file;
        file.open(logname);
        for (int i = 0; i < NUM_TH1; i++)
            for (int j = 0; j < NUM_TH2; j++)
                for (int k = 0; k < NUM_THD1; k++)
                    for (int l = 0; l < NUM_THD2; l++)
                        file.write((char *) &state_grid_index(us, i, j, k, l), sizeof(double));
        file.close();
    }
    void read_u_from_log(const std::string &logname)
    {
        // read all doubles as one long string
        int i = 0;
        int j = 0;
        int k = 0;
        int l = 0;
        std::ifstream file(logname);
        while (!file.fail() && i < NUM_TH1)
        {
            double read;
            file.read((char *) &read, sizeof(double));
            if (!file.fail()) state_grid_index(us, i, j, k, l) = read;
            l++;
            if (l == NUM_THD2)
            {
                l = 0;
                k++;
            }
            if (k == NUM_THD1)
            {
                k = 0;
                j++;
            }
            if (j == NUM_TH2)
            {
                j = 0;
                i++;
            }
        }
        file.close();
    }

private:
    void wrap_angle_0_to_2pi(double &theta)
    {
        while (theta < 0) theta += 2*M_PI;
        while (theta > 2*M_PI) theta -= 2*M_PI;
    }
    bool get_x1_bounds(const double &x1_c, int &lower, int &upper, double &frac)
    {
        bool flipped = false;
        double x1 = x1_c;
        wrap_angle_0_to_2pi(x1);

        // Confine to 0 -> pi
        if (x1 > M_PI)
        {
            x1 = 2*M_PI - x1;
            flipped = true;
        }

        double idx_precise = (x1 - TH1_0) / DELTA;
        int idx_floor = static_cast<int>(floor(idx_precise));
        int idx_ceil  = static_cast<int>(ceil(idx_precise));
        frac = idx_precise - static_cast<double>(idx_floor);

        if (idx_floor < 0)
            lower = 0; // symmetry
        else
            lower = idx_floor;
        if (idx_ceil > NUM_TH1-1)
            upper = NUM_TH1-1;
        else
            upper = idx_ceil;

        return flipped;
    }
    void get_x2_bounds(const double &x2_c, int &lower, int &upper, double &frac)
    {
        double x2 = x2_c;
        wrap_angle_0_to_2pi(x2);

        double idx_precise = (x2 - TH2_0) / DELTA;
        int idx_floor = static_cast<int>(floor(idx_precise));
        int idx_ceil  = static_cast<int>(ceil(idx_precise));
        frac = idx_precise - static_cast<double>(idx_floor);

        if (idx_floor < 0)
            lower = NUM_TH2-1;
        else
            lower = idx_floor;
        if (idx_ceil > NUM_TH2-1)
            upper = 0;
        else
            upper = idx_ceil;
    }
    void get_x3_bounds(const double &x3, int &lower, int &upper, double &frac)
    {
        double idx_precise = (x3 - THD1_0) / DELTA;
        lower = static_cast<int>(floor(idx_precise));
        upper  = static_cast<int>(ceil(idx_precise));
        frac = idx_precise - static_cast<double>(lower);
    }
    void get_x4_bounds(const double &x4, int &lower, int &upper, double &frac)
    {
        double idx_precise = (x4 - THD2_0) / DELTA;
        lower = static_cast<int>(floor(idx_precise));
        upper  = static_cast<int>(ceil(idx_precise));
        frac = idx_precise - static_cast<double>(lower);
    }

    double x1[NUM_TH1];
    double x2[NUM_TH2];
    double x3[NUM_THD1];
    double x4[NUM_THD2];
    double *us = new double[NUM_TH1*NUM_TH2*NUM_THD1*NUM_THD2];
};
