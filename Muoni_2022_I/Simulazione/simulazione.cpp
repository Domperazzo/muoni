/*
  c++ -o simulazione simulazione.cpp simulazione.cc `root-config --glibs --cflags`
*/

#include "simulazione.h"


#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>

#include "TH1F.h"

#define xmax 100;
#define ymax 100;
#define zmax 100;
#define points_number 1000;
#define time_increment 1.0;
#define simulation_time 100;
#define x_riv1 33;
#define y_riv1 33;
#define z_riv1 33;
#define x_riv2 53;
#define y_riv2 53;
#define z_riv2 53;


double rand_range (double min, double max)
  {
    srand(time(NULL));
    return min + (max - min) * rand() / static_cast<double> (RAND_MAX);
  }

int beccato_rivelatore(double coordinata_riv, double coordinata_muon){
  if (coordinata_riv == coordinata_muon) {
    return 1;
  } else{
    return 0;
  }
}

int main(int argc, char const *argv[]) {

  std::vector<double> v_posx, v_posy, v_posz, v_theta, v_phi;
  double v_posizione_rivelatore1[x_riv1, y_riv1, z_riv1], v_posizione_rivelatore2[x_riv2, y_riv2, z_riv2];

  double tempo_sim = 0.;


  for (int i = 0; i < points_number; i++) {
    v_posx.push_back(rand_range(0, xmax));
    v_posy.push_back(rand_range(0, ymax));
    v_posz.push_back(rand_range(0, zmax));
    v_theta.push_back(rand_range(0, 360));
    v_phi.push_back(rand_range(0, 360));
  }

  std::vector<double> v_pos_urto_x, v_pos_urto_y, v_pos_urto_z;
  std::vector<double> v_pos_urto2_x, v_pos_urto2_y, v_pos_urto2_z;

  do {
    for (int i = 0; i < points_number; i++) {
      muone muon(v_posx.at(i), v_posy.at(i), v_posz.at(i), v_theta.at(i), v_phi.at(i), v_velocity, tempo_sim);
      if (beccato_rivelatore(x_riv1 ,muon.posizione_x()) == 1) {
        v_pos_urto_x.push_back(muon.posizione_x());
      } else{
        continue;
      }
      if (beccato_rivelatore(y_riv1 ,muon.posizione_y()) == 1) {
        v_pos_urto_y.push_back(muon.posizione_y());
      } else{
        continue;
      }
      if (beccato_rivelatore(z_riv1 ,muon.posizione_z()) == 1) {
        v_pos_urto_z.push_back(muon.posizione_z());
      } else{
        continue;
      }

      if (beccato_rivelatore(x_riv2 ,muon.posizione_x()) == 1) {
        v_pos_urto2_x.push_back(muon.posizione_x());
      } else{
        continue;
      }
      if (beccato_rivelatore(y_riv2 ,muon.posizione_y()) == 1) {
        v_pos_urto2_y.push_back(muon.posizione_y());
      } else{
        continue;
      }
      if (beccato_rivelatore(z_riv2 ,muon.posizione_z()) == 1) {
        v_pos_urto2_z.push_back(muon.posizione_z());
      } else{
        continue;
      }
      //Questo controllo può essere fatto da una funzione, ci penso dopo
    }
    tempo_sim = tempo_sim + time_increment;
  } while(tempo_sim < simulation_time);

  std::cout << "numero eventi rivelati S1:"<< v_pos_urto_x.size()+v_pos_urto_y.size()+v_pos_urto_z.size() << '\n';
  std::cout << "numero eventi rivelati S2:"<< v_pos_urto2_x.size()+v_pos_urto2_y.size()+v_pos_urto2_z.size() << '\n';



  return 0;
}
