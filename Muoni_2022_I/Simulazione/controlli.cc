#include "controlli.h"
#include <cmath>
#include <iostream>
#include <algorithm>

bool is_coincidence(double L, double K_x, double K_y, double x0, double y0){ //L = distanza tra gli scintillatori
    double delta_x = x0 - L*K_x; //K_x = tan(theta)*cos(phi)
    double delta_y = y0 - L*K_y; //K_y = tan(theta)*sin(phi)
    if(delta_x < larghezza/2 && delta_x > -larghezza/2 && delta_y < profondita/2 && delta_y > -profondita/2){
        return true;
    }
    else return false;
}