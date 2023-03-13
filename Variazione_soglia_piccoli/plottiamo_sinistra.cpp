/*
	c++ -o plottiamo_sx plottiamo_sinistra.cpp `root-config --glibs --cflags`
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
#include "TAxis.h"
#include "TAttFill.h"
#include "TMultiGraph.h"
#include "TStyle.h"

int main(int argc, char **argv) {
  TApplication theApp("theApp", &argc, argv);
  gStyle->SetOptFit(1112);

  std::ifstream dati;
  dati.open("Dati/Tsinistra.txt", std::ios::in);
  std::vector<double> v_media, v_soglie;// v_sigma;
  while (true) {
      double soglia, media1, media2;// sigma;
      dati >> soglia >> media1 >> media2;// >> sigma;
      if(dati.eof()==true) break;
      v_soglie.push_back(soglia);
      v_media.push_back((media1+media2)/2);
      //v_sigma.push_back(sigma);
  }
  dati.close();

  TGraphErrors g1;
  for (int i = 0; i < v_media.size(); i++) {
    g1.SetPoint(i, v_soglie.at(i), v_media.at(i));
  }
  for (int i = 0; i < v_media.size(); i++) {
  g1.SetPointError(i, 0., sqrt(48));
  }

  g1.SetMarkerStyle(20);
  g1.SetMarkerSize(0.7);

  double rangeX_1, rangeX_2;

  rangeX_1 = .410;
  rangeX_2 = .501;


  TGraph g_fill1, g_fill2, g_fill3;
  for (int i = 0; i < 350; i++) {
    g_fill1.SetPoint(i, rangeX_1, i);
    g_fill2.SetPoint(i, rangeX_2, i);
  }

  g_fill1.SetLineColor(4);
  g_fill1.SetLineWidth(8000);
  g_fill1.SetFillStyle(3004);
  g_fill1.SetFillColor(40);
  g_fill1.SetMarkerColor(0);

  g_fill2.SetLineColor(4);
  g_fill2.SetLineWidth(-10200);
  g_fill2.SetFillStyle(3004);
  g_fill2.SetFillColor(40);
  g_fill2.SetMarkerColor(0);


  TMultiGraph multi;
  multi.Add(&g_fill1);
  multi.Add(&g_fill2);
  multi.Add(&g1);

  TCanvas c1;
  c1.SetGridx ();
  c1.SetGridy ();
  c1.SetLeftMargin(.15);
  c1.SetWindowSize (1050, 900);

  multi.SetTitle("Conteggi degli eventi al variare della tensione di soglia; V_{soglia} [V]; #frac{Conteggi}{Minuto} #left[#frac{1}{Minuto}#right]");
  multi.GetHistogram()->GetYaxis()->SetRangeUser(0., 350.);
  multi.GetHistogram()->GetXaxis()->SetRangeUser(0., 0.82);
  multi.GetHistogram()->GetXaxis()->SetTitleSize(0.045);
  multi.GetHistogram()->GetYaxis()->SetTitleSize(0.045);
  multi.Draw("ALP");

  c1.Print("plot_sinistra.pdf", "pdf");
  theApp.Run();


  return 0;
}
