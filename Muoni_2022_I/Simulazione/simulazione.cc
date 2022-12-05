#include "simulazione.h"
#include <math.h>
#include <vector>

muone::muone(double pos_x, double pos_y,double pos_z, double theta, double phi, double velocity, double time):
  x(pos_x),
  y(pos_y),
  z(pos_z),
  ang_theta(theta),
  ang_phi(phi),
  vel(velocity),
  tempo(time)
  {}

muone::muone(std::vector<double> v_posx, std::vector<double> v_posy, std::vector<double> v_posz,
      std::vector<double> v_theta, std::vector<double> v_phi, std::vector<double> v_velocity, double time):
    v_x(v_posx),
    v_y(v_posy),
    v_z(v_posz),
    v_ang_theta(v_theta),
    v_ang_phi()v_phi,
    v_vel(v_velocity),
    tempo(time)
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


double muone::posizione_x(){
  return x+get_vel_component(1)*tempo;
}

double muone::posizione_y(){
  return y+get_vel_component(2)*tempo;
}

double muone::posizione_z(){
  return z+get_vel_component(3)*tempo;
}

double muone::get_pos_x(int numero){
  return v_x.at(numero);
}

double muone::get_pos_y(int numero){
  return v_y.at(numero);
}

double muone::get_pos_z(int numero){
  return v_z.at(numero);
}

double muone::get_theta(int numero){
  return v_ang_theta.at(numero);
}

double muone::get_phi(int numero){
  return v_ang_phi.at(numero);
}
double muone::get_vel(int numero){
  return v_vel.at(numero);
}

double muon::update_time(double time, double time_increment){
  tempo = time + time_increment;
}
/*
  do {
      muone muon(v_posx.at(8), v_posy.at(8), v_posz.at(8), v_theta.at(8), v_phi.at(8), v_modulovel, tempo_sim);
      int controllo = 0;
      int controllo2 = 0;
      controllo_rilevazione(x1_riv1, y1_riv1, x2_riv1, y2_riv1, z_riv1, muon.posizione_x(), muon.posizione_y(), muon.posizione_z(), controllo);
      controllo_rilevazione(x1_riv2, y1_riv2, x2_riv2, y2_riv2, z_riv2, muon.posizione_x(), muon.posizione_y(), muon.posizione_z(), controllo2);
      if (controllo == 1) {
        h_rivelati1->Fill(muon.posizione_x(),muon.posizione_y());
      }

      if (controllo2 == 1) {
        h_rivelati2->Fill(muon.posizione_x(),muon.posizione_y());
      }

      v_motox.push_back(muon.posizione_x());
      v_motoy.push_back(muon.posizione_y());
      v_motoz.push_back(muon.posizione_z());

    tempo_sim = tempo_sim + time_increment;
  } while(tempo_sim < simulation_time);

  TGraph2D g2d_moto;
  for (int i = 0; i < v_motox.size(); i++) {
    g2d_moto.SetPoint(i, v_motox.at(i), v_motoy.at(i), v_motoz.at(i));
  }
  TCanvas c4;
  g2d_moto.SetMarkerStyle(20);
  g2d_moto.Draw("pcol");
*/
