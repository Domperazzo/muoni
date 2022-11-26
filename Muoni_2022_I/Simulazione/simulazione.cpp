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
#include <cstdlib>

#include "TGraph2D.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TStyle.h"

#define xmax 100
#define ymax 100
#define zmax 100

#define points_number 1000

#define time_increment 1.0

#define simulation_time 100

#define x1_riv1 53
#define y1_riv1 53
#define x2_riv1 53
#define y2_riv1 53
#define z_riv1 53

#define x1_riv2 33
#define y1_riv2 33
#define x2_riv2 33
#define y2_riv2 33
#define z_riv2 33


double rand_range (double min, double max)
  {
    return min + (max - min)* rand() / static_cast<double> (RAND_MAX);
  }


void controllo_rilevazione(double x1_riv, double y1_riv, double x2_riv, double y2_riv, double z_riv, double x_muon, double y_muon, double z_muon, int controllo){
  if (x1_riv < x_muon && x_muon < x2_riv) {
    if (y1_riv < y_muon && y_muon < y2_riv) {
      controllo = 1;
    }
  } else{
    controllo = 0;
  }
}





int main(int argc, char **argv) {

  TApplication theApp("theApp", &argc, argv);
  gStyle->SetOptFit(1112);

  std::vector<double> v_posx, v_posy, v_posz, v_theta, v_phi;

  double tempo_sim = 0.;

  std::vector<double> v_modulovel; //in frazioni di c
  for (int i = 0; i < points_number; i++) {
    v_posx.push_back(rand_range(0, xmax));
    v_posy.push_back(rand_range(0, ymax));
    v_posz.push_back(rand_range(0, zmax));
    v_theta.push_back(rand_range(0, 90));
    v_phi.push_back(rand_range(0, 360));
    v_modulovel.push_back(rand_range(0, 1));
  }

  TGraph2D g2d;
  for (int i = 0; i < points_number; i++) {
    g2d.SetPoint(i, v_posx.at(i), v_posy.at(i), v_posz.at(i));
    //std::cout << "x:" << v_posx.at(i)<< '\t'<< "y:" << v_posy.at(i)<< '\t'<< "z:" << v_posz.at(i)<< '\t';
  }
  TCanvas c3;
  gStyle->SetPalette(1);
  g2d.SetMarkerStyle(20);
  g2d.Draw("pcol");
  //g2d.Draw();
  //g2d.Draw("surf1");

  std::vector<double> v_pos_urto_x, v_pos_urto_y, v_pos_urto_z;
  std::vector<double> v_pos_urto2_x, v_pos_urto2_y, v_pos_urto2_z;

  TH2F * h_rivelati1  = new TH2F("h_rivelati1","h_rivelati1",1000,-500,500,100,-500,500);
  TH2F * h_rivelati2  = new TH2F("h_rivelati2","h_rivelati2",100,-500,500,100,-500,500);


  do {
    for (int i = 0; i < points_number; i++) {
      muone muon(v_posx.at(i), v_posy.at(i), v_posz.at(i), v_theta.at(i), v_phi.at(i), v_modulovel, tempo_sim);
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
    }

    tempo_sim = tempo_sim + time_increment;
  } while(tempo_sim < simulation_time);

  TCanvas c1;
  h_rivelati1->Draw();
  TCanvas c2;
  h_rivelati2->Draw();


  theApp.Run();

  return 0;
}
