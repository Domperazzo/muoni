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



    TGraphErrors g_S1, g_S2, g_conteggiS1, g_conteggiS2;
    for (int i = 0; i < v_sogliaS1.size(); i++) {
      g_S1.SetPoint(i, v_sogliaS1.at(i), v_conteggiS1.at(i)/2);
      g_S1.SetPointError(i, 0., sqrt(v_conteggiS1.at(i)/2));
      g_S2.SetPoint(i, v_sogliaS2.at(i), v_conteggiS2.at(i)/2);
      g_S2.SetPointError(i, 0., sqrt(v_conteggiS2.at(i)/2));
    }


    g_S1.SetMarkerStyle(20);
    g_S1.SetMarkerSize(0.5);
    g_S2.SetMarkerStyle(20);
    g_S2.SetMarkerSize(0.5);

    double rangeX_min, rangeX_max;

    rangeX_min = 1.348;
    rangeX_max = 1.450;

    TGraph g_fill1, g_fill2;
    for (int i = 0; i < 505; i++){
      g_fill1.SetPoint(i, rangeX_min, i);
      g_fill2.SetPoint(i, rangeX_max, i);
    }

    g_fill1.SetLineColor(4);
    g_fill1.SetLineWidth(10000);
    g_fill1.SetFillStyle(3004);
    g_fill1.SetFillColor(40);
    g_fill1.SetMarkerColor(0);

    g_fill2.SetLineColor(4);
    g_fill2.SetLineWidth(-10800);
    g_fill2.SetFillStyle(3004);
    g_fill2.SetFillColor(40);
    g_fill2.SetMarkerColor(0);

    TMultiGraph multi;
    multi.Add(&g_fill1);
    multi.Add(&g_fill2);
    multi.Add(&g_S1);

    TCanvas c1;
    c1.SetGridx();
    c1.SetGridy();
    c1.SetLeftMargin(.15);

    //c1.SetWindowSize (1050, 900);

    multi.SetTitle("Conteggi degli eventi rilevati da S1 al variare della tensione di soglia; V_{soglia} [V]; #frac{Conteggi}{Minuto}");
    //g_S1.GetHistogram()->GetYaxis()->SetRangeUser(0., 505.);
    //g_S1.GetHistogram()->GetXaxis()->SetRangeUser(0., 1.12);
    multi.Draw("AP");

    theApp.Run();

  return 0;
}
