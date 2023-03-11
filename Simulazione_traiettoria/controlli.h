#ifndef controlli_h
#define controlli_h



bool is_coincidence(double L, double K_x, double K_y, double x0, double y0);
double rand_phi(double min, double max);
double f_cos2(double x);
double rand_theta(double f(double), double xMin, double xMax, double yMax);
double rand_range(double min, double max);
double get_path (double L, double K_x, double K_y, double x0, double y0);
double x_coinc_coord(double L, double k_x, double x0);
double y_coinc_coord(double L, double k_y, double y0);

bool   controllo_verticale( double x );

#endif
