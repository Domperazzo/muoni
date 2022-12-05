#include "muon_class.h"
#include <math.h>
#include <vector>

#define _USE_MATH_DEFINES

muone::muone(double ragg, double theta, double phi):
  raggio(ragg),
  ang_theta(theta),
  ang_phi(phi)
  {}
muone::~muone(){

}

double muone::coeff_x(){
  return tan(ang_theta)*cos(ang_phi);
}

double muone::coeff_y(){
  return tan(ang_theta)*sin(ang_phi);
}

double muone::rand_phi (double min, double max)
  {
    return min + (max - min) * rand () / static_cast<double> (RAND_MAX);
  }

double muone::rand_theta(double valore_centrale) {
  return 0.5*(valore_centrale + sin(valore_centrale)*cos(valore_centrale)) - 0.5*(-0.5*M_PI+sin(-0.5*M_PI)*cos(-0.5*M_PI));
}
