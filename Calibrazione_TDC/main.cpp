/*
	c++ -o main main.cpp `root-config --glibs --cflags`

  errore su secondi 0.4
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
  dati.open("calibrazione_TDC2.txt", std::ios::in);
  std::vector<double> v_tempo, v_tdc;
  while (true) {
      double tempo, tdc;
      dati >> tempo;
      dati >> tdc;
      if(dati.eof()==true) break;
      v_tempo.push_back(tempo+5);
      v_tdc.push_back(tdc);
  }
  dati.close();

  TGraphErrors g1;

  for (int i = 0; i < v_tempo.size(); i++) {
    g1.SetPoint(i, v_tdc.at(i), v_tempo.at(i));
    g1.SetPointError(i, 1.,  0.4);
  }

  g1.SetMarkerStyle(20);
  g1.SetMarkerSize(0.3);


  TF1 modello("modello", "[0]*x+[1]");
  modello.SetParName(0, "m");
  modello.SetParName(1, "q");

  TFitResultPtr fit_result = g1.Fit(&modello, "S");

  TCanvas c1;
  g1.SetTitle("Calibrazione TDC; TDC; Tempo [ns]");
  g1.Draw("AP");
  theApp.Run();

  return 0;
}
