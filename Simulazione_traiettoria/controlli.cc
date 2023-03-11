#include "controlli.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <ctime>



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
    double k=0;
    do{
        k=min + (max - min) * rand() /static_cast<double> (RAND_MAX);
    }while(k==2*M_PI);
    return k;
}

double f_cos2(double x){
    return 4*pow(cos(x), 2)/M_PI; //integrale 
}

double rand_theta(double f(double), double xMin, double xMax, double yMax)
{
  
    double x = 0.;
    double y = 0.;
    do
    {
        x = rand_range(xMin, xMax);
        y = rand_range(0, yMax);
    } while (y > f(x));
    return x;
}

double rand_range(double min, double max){
    
    return min + (max - min) *rand() /static_cast<double> (RAND_MAX);
}

double get_path (double L, double K_x, double K_y, double x0, double y0){
    double delta_x = x0 - L*K_x;
    double delta_y = y0 - L*K_y; 
    return sqrt(pow(x0 - delta_x, 2) + pow(y0 - delta_y, 2) + pow(L, 2) );
} 

bool   controllo_verticale( double x )
{
	if( x <=0 && x>= -0.572 ) return true;
	else return false;
}

