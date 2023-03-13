  /*
	c++ -o main main.cpp `root-config --glibs --cflags`
  tra S1 e S2 non ritardati si ha un ritardo di 1.6ns
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

using namespace std;
int main(int argc, char **argv) {
  TApplication theApp("theApp", &argc, argv);
  gStyle->SetOptFit(1112);

  ifstream dati;
  dati.open("Dati/g1.txt", ios::in);
  vector<double> v_ritardo, v_conteggi;
  while (true) {
      double ritardo, conteggi;
      dati >> ritardo >> conteggi;
      if(dati.eof()==true) break;

      v_ritardo.push_back(ritardo-1.6);
      v_conteggi.push_back(conteggi/5); //misure prese in 5 minuti
  }
  dati.close();

  ifstream dati2;
  dati2.open("Dati/g2.txt", std::ios::in);
  std::vector<double> v_ritardo2, v_conteggi2;
  while (true) {
      double ritardo2, conteggi2;
      dati2 >> ritardo2 >> conteggi2;
      if(dati2.eof()==true) break;
      v_ritardo2.push_back(ritardo2-1.6);
      v_conteggi2.push_back(conteggi2/5); //misure prese in 5 minuti
  }
  dati2.close();

  ifstream dati3;
  dati3.open("Dati/g3.txt", std::ios::in);
  std::vector<double> v_ritardo3, v_conteggi3;
  while (true) {
      double ritardo3, conteggi3;
      dati3 >> ritardo3 >> conteggi3;
      if(dati3.eof()==true) break;
      v_ritardo3.push_back(ritardo3-1.6);
      v_conteggi3.push_back(conteggi3/5); //misure prese in 5 minuti
  }
  dati3.close();



  TGraphErrors g1, g2, g3;
  for (int i = 0; i < v_conteggi.size(); i++) {
    g1.SetPoint(i, v_ritardo.at(i), v_conteggi.at(i));
    g1.SetPointError(i, 0.4, sqrt(v_conteggi.at(i)));
  }

  for (int i = 0; i < v_conteggi2.size(); i++) {
    g2.SetPoint(i, v_ritardo2.at(i), v_conteggi2.at(i));
    g2.SetPointError(i, 0.4, sqrt(v_conteggi2.at(i)));
  }

  for (int i = 0; i < v_conteggi3.size(); i++) { //dati ignorati
    g3.SetPoint(i, v_ritardo3.at(i), v_conteggi3.at(i));
    g3.SetPointError(i, 0.4, sqrt(v_conteggi3.at(i)));
  }

  g1.SetMarkerStyle(20);
  g1.SetMarkerSize(0.5);

  g2.SetMarkerStyle(20);
  g2.SetMarkerSize(0.5);

  g3.SetMarkerStyle(20);
  g3.SetMarkerSize(0.5);


  double coincidenze_acc;

/*
  TF1 modello("modello", "gaus");
  TFitResultPtr fit_result = g2.Fit(&modello, "S");

  double sigma = modello.GetParameter(2);
  double resolving_time_ns = sigma/2;
  cout << " resolving time secondo il fit a campana = " << resolving_time_ns << " ns" << endl;
*/
  //dai dati.txt dell'Effiecienza, conteggi/min associati alle tensioni di Alimentazione scelte 
  double rA = 229165;
  double rB = 213969;

  cout << " 1/rA = " << 1/rA << " 1/rB = " << 1/rB <<endl;

  //coincidenze_acc = (1-exp(-rB*(2*resolving_time_sec)))*rA+(1-exp(-rA*(2*resolving_time_sec)))*rB; //formula piu precisa, non serve
  coincidenze_acc = rA*rB*24.68*pow(10, -9)/60; //larghezza dei due segnali messi insieme misurata con l'oscilloscopio= 24.68 +- 0.4 ns

  std::cout << "coincidenze accidentali teoricamente = " << coincidenze_acc <<  " 1/min" << "\n";


  TF1 retta("retta", "[0]");
  TFitResultPtr fit_result2 = g1.Fit(&retta, "S");
  //retta.SetParName(0, "m");
  retta.SetParName(0, "q");
  //retta.SetParameter(0, 1.);

  TMultiGraph multi;

  multi.Add(&g1);
  multi.Add(&g2);
  multi.Add(&g3);


  TCanvas c1;
  c1.SetLeftMargin(.18);
  c1.SetBottomMargin(.18);
  multi.SetTitle(" ; Ritardo [ns]; #frac{Conteggi}{Minuto} #left[#frac{1}{min}#right]");
  multi.GetHistogram()->GetXaxis()->SetTitleSize(0.055);
  multi.GetHistogram()->GetYaxis()->SetTitleSize(0.055);
  multi.GetHistogram()->GetXaxis()->SetLabelSize(0.05);
  multi.GetHistogram()->GetYaxis()->SetLabelSize(0.05);

  multi.Draw("AP");
  theApp.Run();


  return 0;
}
