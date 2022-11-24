/*
	c++ -o main main.cpp `root-config --glibs --cflags`
  tra S1 e S2 non ritardai si ha un ritardo di 1.6ns
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
#include "TLatex.h"
#include "TAttFill.h"
#include "TMultiGraph.h"
#include "TStyle.h"

int main(int argc, char **argv) {
  TApplication theApp("theApp", &argc, argv);
  gStyle->SetOptFit(1112);

  std::ifstream dati;
  dati.open("g1.txt", std::ios::in);
  std::vector<double> v_ritardo, v_conteggi;
  while (true) {
      double ritardo, conteggi;
      dati >> ritardo >> conteggi;
      if(dati.eof()==true) break;
      v_ritardo.push_back(ritardo-1.6);
      v_conteggi.push_back(conteggi);
  }
  dati.close();

  std::ifstream dati2;
  dati2.open("g2.txt", std::ios::in);
  std::vector<double> v_ritardo2, v_conteggi2;
  while (true) {
      double ritardo2, conteggi2;
      dati2 >> ritardo2 >> conteggi2;
      if(dati2.eof()==true) break;
      v_ritardo2.push_back(ritardo2-1.6);
      v_conteggi2.push_back(conteggi2);
  }
  dati2.close();




  TGraphErrors g1, g2;
  for (int i = 0; i < v_conteggi.size(); i++) {
    g1.SetPoint(i, v_ritardo.at(i), v_conteggi.at(i));
    g1.SetPointError(i, 0.4, sqrt(v_conteggi.at(i)));
  }

  for (int i = 0; i < v_conteggi2.size(); i++) {
    g2.SetPoint(i, v_ritardo2.at(i), v_conteggi2.at(i));
    g2.SetPointError(i, 0.4, sqrt(v_conteggi2.at(i)));
  }

  g1.SetMarkerStyle(20);
  g1.SetMarkerSize(0.5);

  g2.SetMarkerStyle(20);
  g2.SetMarkerSize(0.5);



/*
  TF1 modello("modello", "gaus");
  TFitResultPtr fit_result = g2.Fit(&modello, "S");

  double sigma = modello.GetParameter(2);

  double risultato;

  double resolving_time_ns = sigma ;

  double resolving_time_sec = resolving_time_ns*pow(10, -9)/60;

  risultato = (1-exp(-320282*(2*resolving_time_sec)))*229165+(1-exp(-229165*(2*resolving_time_sec)))*320282;


  std::cout << risultato<< "\n";
*/

  TF1 retta("retta", "[0]*x+[1]");
  TFitResultPtr fit_result2 = g1.Fit(&retta, "S");
  retta.SetParName(0, "m");
  retta.SetParName(1, "q");
  retta.SetParameter(0, 1.);

  TMultiGraph multi;

  multi.Add(&g1);
  multi.Add(&g2);

  TCanvas c1;
  c1.SetLeftMargin(.15);
  multi.SetTitle("Rate coincidenze accidentali; Ritardo [ns]; #frac{Conteggi}{Minuto} #left[#frac{1}{Minuto}#right]");
  multi.Draw("AP");
  theApp.Run();




  return 0;
}
