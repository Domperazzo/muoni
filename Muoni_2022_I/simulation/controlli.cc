#include "controlli.h"
#include <cmath>
#include <iostream>
#include <algorithm>

#define larghezza 80.0 //in cm
#define profondita 30.0 //in cm

bool is_coincidence(double L, double K_x, double K_y, double x0, double y0){ //L = distanza tra gli scintillatori
    double delta_x = x0 - L*K_x; //K_x = tan(theta)*cos(phi)
    double delta_y = y0 - L*K_y; //K_y = tan(theta)*sin(phi)
    if(delta_x < larghezza/2 && delta_x > -larghezza/2 && delta_y < profondita/2 && delta_y > -profondita/2){
        return true;
    }
    return false;
}

double x_coinc_coord(double L, double k_x, double x0){
    return x0 - L*k_x; //K_x = tan(theta)*cos(phi)
}

double y_coinc_coord(double L, double k_y, double y0){
    return y0 - L*k_y; //K_x = tan(theta)*cos(phi)
}

double rand_phi(double min, double max){
    return min + (max - min) * rand() /static_cast<double> (RAND_MAX);
}

double rand_theta(double valore_centrale){
    return 0.5*(valore_centrale + sin(valore_centrale)*cos(valore_centrale)) -0.5*(-0.5*M_PI+sin(-0.5*M_PI)*cos(-0.5*M_PI));
}

double rand_range(double min, double max){
    return min + (max - min) *rand() /static_cast<double> (RAND_MAX);
}

double get_path (double L, double K_x, double K_y, double x0, double y0){
    double delta_x = x0 - L*K_x;
    double delta_y = y0 - L*K_y; 
    return sqrt(pow(x0 - delta_x, 2) + pow(y0 - delta_y, 2) + pow(L, 2) );
} 
