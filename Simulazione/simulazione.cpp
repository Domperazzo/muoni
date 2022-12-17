/*
  c++ -o simulazione simulazione.cpp simulazione.cc `root-config --glibs --cflags`
*/

#include "simulazione.h"
#include "list.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <cstdlib>

#include "TGraph2D.h"
#include "TGraph.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TStyle.h"

#define xmax 4000
#define ymax 5000
#define zmax 10000

#define z_generazione 8000

#define points_number 10000

#define time_increment 1.0

#define simulation_time 100

#define x1_riv1 300
#define y1_riv1 400
#define x2_riv1 700
#define y2_riv1 600
#define z_riv1 600

#define x1_riv2 300
#define y1_riv2 400
#define x2_riv2 700
#define y2_riv2 600
#define z_riv2 300

#define _USE_MATH_DEFINES

double fgaus (double x, double mean, double sigma)
  {
    double esponente = (-0.5 * (x - mean) * (x - mean) / (sigma * sigma));
    double termine_proporzionalita = 1 / (sqrt(2 * M_PI) * sigma);
    return termine_proporzionalita * exp(esponente);
  }

double rand_range (double min, double max)
  {
    return min + (max - min) * rand () / static_cast<double> (RAND_MAX);
  }

double rand_TAC_gaus (double mean, double sigma)
  {
    double x = 0. ;
    double y = 0. ;
    do {
      x = rand_range (mean - 4 * sigma, mean + 4 * sigma);
      y = rand_range (0, fgaus (mean, mean, sigma));
    } while (y > fgaus (x, mean, sigma));
    return x;
  }


bool controllo1_passaggio(muone muon_1, muone muon_2, double z_riv){
  if (muon_2.posizione_z()< z_riv && muon_1.posizione_z() > z_riv) {
    return true;
  } else{
    return false;
  }
}


bool controllo_passaggio(list lista, double z_riv, ){

}

/*

      Devo risistemare i vettori in modo che sia r=[x1, y1, z1, x2, y2, z2,...]

      Il vettore rinominalo v_pos


*/



int main(int argc, char **argv) {

  TApplication theApp("theApp", &argc, argv);
  gStyle->SetOptFit(1112);

  std::vector<double> v_posx, v_posy, v_posz, v_theta, v_phi, v_modulovel;

  double tempo_sim = 0.;

  for (int i = 0; i < points_number; i++) {
    v_posx.push_back(rand_range(0, xmax));
    v_posy.push_back(rand_range(0, ymax));
    v_posz.push_back(rand_range(0, zmax));
    v_theta.push_back(rand_TAC_gaus(30, 5));
    v_phi.push_back(rand_TAC_gaus(30, 5));
    v_modulovel.push_back(rand_range(1, 100));
  }


  TGraph2D g2d;
  for (int i = 0; i < points_number; i++) {
    g2d.SetPoint(i, v_posx.at(i), v_posy.at(i), v_posz.at(i));
    //std::cout << "x:" << v_posx.at(i)<< '\t'<< "y:" << v_posy.at(i)<< '\t'<< "z:" << v_posz.at(i)<< '\n';

  }
  TCanvas c3;
  gStyle->SetPalette(1);
  g2d.SetMarkerStyle(20);
  g2d.Draw("pcol");
  c3.Print("Condizione iniziale.pdf", "pdf");

  list(points_number, list);
  for (int i = 0; i < points_number; i=i+3) {
    list_push(list, v_pos.at(i));
  }



  //----------------------------------------------------------------//
  //    Ci sarebbe da fare una funzione che valuta le due posizioni
  //    a t e t+dt e vede se è passato attraverso il rilevatore



  //  h_rivelati1->Draw();
//    h_x.Draw();

  //----------------------------------------------------------------//
  theApp.Run();

  return 0;
}
