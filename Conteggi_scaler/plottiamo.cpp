/*
	c++ -o plottiamo plottiamo.cpp `root-config --glibs --cflags`
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TH1F.h"
#include "TF1.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include "TApplication.h"
#include "TMultiGraph.h"
#include "TStyle.h"

double rand_range (double min, double max)
  {
    return min + (max - min)* rand();
  }

void trova_plateu(double xMin, double xMax, std::vector<double> v_x, std::vector<double> v_y, std::vector<double> v_sigma){
  xMin = 0.;
  xMax = 0.;
  double chi2 = 0.;
  double ndf = 0.;
  do {

    double d_punto_inizio = rand_range(0, v_x.size());
    int i_punto_inizio = int (d_punto_inizio);
    if (i_punto_inizio > v_x.size()) {
      i_punto_inizio = v_x.size();
    }

    double d_punto_fine = rand_range(0, v_x.size());
    int i_punto_fine = int (d_punto_fine);
    
    if (i_punto_fine > v_x.size()) {
      i_punto_fine = v_x.size();
    }


    TGraphErrors grafico;
    for (int i = i_punto_inizio; i < i_punto_fine; i++) {
      grafico.SetPoint(i, v_x.at(i), v_y.at(i));
      grafico.SetPointError(i, 0., v_sigma.at(i));
    }

    TF1 modello ("modello", "[0]*x+[1]");
  	TFitResultPtr fit_result = grafico.Fit(&modello, "S");

    chi2 = modello.GetChisquare();
    ndf = modello.GetNDF();
    xMin = v_x.at(i_punto_inizio);
    xMax = v_x.at(i_punto_fine);
  } while(chi2/ndf > 1.);
}

int main(int argc, char **argv) {
  TApplication theApp("theApp", &argc, argv);
  gStyle->SetOptFit(1112);

  std::ifstream dati;
  dati.open("Dati/S1.txt", std::ios::in);
  std::vector<double> v_media, v_sigma;
  while (true) {
      double media, sigma;
      dati >> media >> sigma;
      if(dati.eof()==true) break;
      v_media.push_back(media);
      v_sigma.push_back(sigma);
  }
  dati.close();

  std::ifstream soglia;
  soglia.open("Dati/soglie.txt", std::ios::in);
  std::vector<double> v_soglie1, v_soglie2;
  while (true) {
      double sogliola1, sogliola2;
      soglia >> sogliola1 >> sogliola2 ;
      if(soglia.eof()==true) break;
      v_soglie1.push_back(sogliola1);
      v_soglie2.push_back(sogliola2);
  }
  soglia.close();


  TGraphErrors g1;
  for (int i = 0; i < v_media.size(); i++) {
    g1.SetPoint(i, v_soglie1.at(i), v_media.at(i));
  }
  for (int i = 0; i < v_sigma.size(); i++) {
    g1.SetPointError(i, 0., v_sigma.at(i));
  }

  g1.SetMarkerStyle(20);
  g1.SetMarkerSize(0.5);

  g1.SetTitle("Rate vs Soglia");
  g1.GetHistogram()-> GetXaxis()->SetTitle("V_{soglia}");
  g1.GetHistogram()-> GetYaxis()->SetTitle("Conteggi");

  double rangeX_min, rangeX_max;

  trova_plateu(rangeX_min, rangeX_max, v_soglie1, v_media, v_sigma);

  //rangeX_min = 1.1;
  //rangeX_max = 1.25;


  TGraph g_fill1, g_fill2;
  for (int i = 0; i < 100; i++) {
    g_fill1.SetPoint(i, rangeX_min, i*1000);
    g_fill2.SetPoint(i, rangeX_min, i*1000);
  }

  g_fill1.SetLineColor(4);
  g_fill1.SetLineWidth(8000);
  g_fill1.SetFillStyle(3004);
  g_fill1.SetFillColor(6);

  g_fill2.SetLineColor(4);
  g_fill2.SetLineWidth(-8000);
  g_fill2.SetFillStyle(3004);
  g_fill2.SetFillColor(6);

  TMultiGraph multi;
  multi.Add(&g1);
  multi.Add(&g_fill1);
  multi.Add(&g_fill2);
  //multi.GetYaxis()->SetRange(0,100000);

  TCanvas c1;
  c1.SetGridx ();
  c1.SetGridy ();
  c1.SetWindowSize (1050, 900);
  c1.SetLeftMargin (0.15);

  multi.SetTitle("Rate vs Soglia; V_{soglia} [V]; Conteggi");
  multi.Draw("ACP");

  //c1.Print("plottissimo.pdf", "pdf");
  theApp.Run();


  return 0;
}
