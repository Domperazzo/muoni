#include "muon_class.h"
#include <math.h>
#include <vector>

muone::muone(double ragg, double theta, double phi):
  raggio(ragg),
  ang_theta(theta),
  ang_phi(phi)
  {}
muone::~muone(){

}


double muone::get_vel_component(int component){
    if (component==1) {
      return vel*sin(ang_theta)*cos(ang_phi);
    }

    if (component==2) {
      return vel*sin(ang_theta)*sin(ang_phi);
    }

    if (component==3) {
      return vel*cos(ang_theta);
    }
    return 0;

}
