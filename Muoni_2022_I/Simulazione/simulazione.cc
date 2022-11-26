#include "simulazione.h"
#include <math.h>
#include <vector>

muone::muone(double pos_x, double pos_y,double pos_z, double theta, double phi, std::vector<double> vel, double time):
  x(pos_x),
  y(pos_y),
  z(pos_z),
  ang_theta(theta),
  ang_phi(phi),
  v_vel(vel),
  tempo(time)
  {}


muone::~muone(){

}

double muone::modulo_vel(){
  return sqrt(pow(v_vel.at(0), 2) + pow(v_vel.at(1), 2) + pow(v_vel.at(2), 2));
}

double muone::get_vel_component(int component){
    if (component==1) {
      return modulo_vel()*sin(ang_theta)*cos(ang_phi);
    }

    if (component==2) {
      return modulo_vel()*sin(ang_theta)*sin(ang_phi);
    }

    if (component==3) {
      return modulo_vel()*cos(ang_theta);
    }
    return 0;

}


double muone::posizione_x(){
  return x+get_vel_component(1)*tempo;
}

double muone::posizione_y(){
  return y+get_vel_component(2)*tempo;
}

double muone::posizione_z(){
  return z+get_vel_component(3)*tempo;
}
