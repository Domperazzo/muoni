#include "muon_class.h"
#include <math.h>
#include <vector>

#define _USE_MATH_DEFINES

muone::muone(double theta, double phi):
  ang_theta(theta),
  ang_phi(phi)
  {}

muone::~muone(){
}

double muone::get_theta(){
  return ang_theta;
}

double muone::get_phi(){
  return ang_phi;
}

double muone::coeff_x(){
  return tan(ang_theta)*cos(ang_phi);
}

double muone::coeff_y(){
  return tan(ang_theta)*sin(ang_phi);
}
