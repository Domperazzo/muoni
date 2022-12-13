#ifndef controlli_h
#define controlli_h

#define larghezza 80 //in cm
#define profondita 30 //in cm

bool is_coincidence(double L, double K_x, double K_y, double x0, double y0);
double rand_phi(double min, double max);
double rand_theta(double valore_centrale);
double rand_range(double min, double max);
double get_path (double L, double K_x, double K_y, double x0, double y0);

#endif
