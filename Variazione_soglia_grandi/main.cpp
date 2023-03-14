/*
	c++ -o main main.cpp `root-config --glibs --cflags`
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
//alimentazione S1 = 1275 S2 = 1350
  std::ifstream dati;
  dati.open("Dati/dati.txt", std::ios::in);
  std::vector<double> v_sogliaS1, v_sogliaS2, v_conteggiS1, v_conteggiS2;
  while (true) {
      double sogliaS1, sogliaS2, conteggiS1, conteggiS2;
      dati >> sogliaS1;
      dati >> sogliaS2;
      dati >> conteggiS1;
      dati >> conteggiS2;
      if(dati.eof()==true) break;
      v_sogliaS1.push_back(sogliaS1);
      v_sogliaS2.push_back(sogliaS2);
      v_conteggiS1.push_back(conteggiS1);
      v_conteggiS2.push_back(conteggiS2);
  }
  dati.close();



  TGraphErrors g_S1, g_S2;

  for (int i = 0; i < v_sogliaS1.size(); i++) {
    g_S1.SetPoint(i, v_sogliaS1.at(i), v_conteggiS1.at(i)/2);
    g_S1.SetPointError(i, 0.001, sqrt(v_conteggiS1.at(i)/2));
    g_S2.SetPoint(i, v_sogliaS2.at(i), v_conteggiS2.at(i)/2);
    g_S2.SetPointError(i, 0.001, sqrt(v_conteggiS2.at(i)/2));
  }


  g_S1.SetMarkerStyle(20);
  g_S1.SetMarkerSize(0.5);
  g_S2.SetMarkerStyle(20);
  g_S2.SetMarkerSize(0.5);

  double rangeX_1, rangeX_2;
  int k = 0;
  rangeX_1 = 1.350;
  rangeX_2 = 1.450;

  TGraph g_fill1, g_fill2;
  for(int j = 70.; j<rangeX_1*100.; j++){
    for (int i = 20000; i < 110000; i+=2000) {
      g_fill1.SetPoint(k, j/100., i);
      k++;
    }
  }

  k = 0;
  for(int j = rangeX_2*100.; j<=200.; j++){
    for (int i = 20000; i < 110000; i+=2000) {
      g_fill2.SetPoint(k, j/100., i);
      k++;
    }
  }
  g_fill1.SetFillStyle(3004);
  g_fill1.SetMarkerStyle(20);
  g_fill1.SetMarkerColor(33);
  g_fill1.SetMarkerSize(0.5);

  g_fill2.SetFillStyle(3004);
  g_fill2.SetMarkerStyle(20);
  g_fill2.SetMarkerColor(33);
  g_fill2.SetMarkerSize(0.5);


  TMultiGraph multi;
  multi.Add(&g_fill1);
  multi.Add(&g_fill2);
  multi.Add(&g_S1);

  TCanvas c1;
  c1.SetGridx();
  c1.SetGridy();
  c1.SetLeftMargin(.15);


  multi.SetTitle("  ;V_{soglia} [V]; #frac{Conteggi}{Minuto}");
  multi.GetHistogram()->GetYaxis()->SetRangeUser(15000., 115000.);
  multi.GetHistogram()->GetXaxis()->SetRangeUser(10., 2.2);
  multi.GetHistogram()->GetXaxis()->SetTitleSize(0.045);
  multi.GetHistogram()->GetYaxis()->SetTitleSize(0.045);
  multi.Draw("AP");

  TMultiGraph multi2;
  multi2.Add(&g_fill1);
  multi2.Add(&g_fill2);
  multi2.Add(&g_S2);

  TCanvas c2;
  c2.SetGridx();
  c2.SetGridy();
  c2.SetLeftMargin(.15);


  multi2.SetTitle("  ;V_{soglia} [V]; #frac{Conteggi}{Minuto}");
  multi2.GetHistogram()->GetYaxis()->SetRangeUser(15000., 115000.);
  multi2.GetHistogram()->GetXaxis()->SetRangeUser(10., 2.2);
  multi2.GetHistogram()->GetXaxis()->SetTitleSize(0.045);
  multi2.GetHistogram()->GetYaxis()->SetTitleSize(0.045);
  multi2.Draw("AP");

  theApp.Run();
  return 0;
}
