/*
	c++ -o plottiamo_dx plottiamo_destra.cpp `root-config --glibs --cflags`
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
  dati.open("Dati/Tdestra.txt", std::ios::in);
  std::vector<double> v_media, v_sigma;
  while (true) {
      double media, sigma;
      dati >> media >> sigma;
      if(dati.eof()==true) break;
      v_media.push_back(media);
      v_sigma.push_back(sigma);
  }
  dati.close();
//794	1.05344
  std::ifstream soglia;
  soglia.open("Dati/soglie.txt", std::ios::in);
  std::vector<double> v_soglie1, v_soglie2, v_freq, v_tempo;
  while (true) {
      double sogliola1, sogliola2;
      int freq, tempo;
      soglia >> sogliola1 >> sogliola2 >> freq >> tempo;
      if(soglia.eof()==true) break;
      v_soglie1.push_back(sogliola1);
      v_soglie2.push_back(sogliola2);
      v_freq.push_back(freq);
      v_tempo.push_back(tempo);
  }
  soglia.close();


  TGraphErrors g1;
  for (int i = 0; i < v_media.size()-5; i++) {
    g1.SetPoint(i, v_soglie1.at(i), v_media.at(i)/v_tempo.at(i));
  }

  for (int i = 0; i < v_sigma.size()-5; i++) {
    g1.SetPointError(i, 0.001, sqrt(v_media.at(i))/v_tempo.at(i)); //distribuzione poissoniana -> sigma=rad(n)
  }


  g1.SetMarkerStyle(20);
  g1.SetMarkerSize(0.7);

  g1.SetTitle("Rate vs Soglia");
  g1.GetHistogram()-> GetXaxis()->SetTitle("V_{soglia}");
  //g1.GetHistogram()->GetYaxis()->SetRangeUser(-5., 600.);
  g1.GetHistogram()-> GetYaxis()->SetTitle("Conteggi/Minuto");


  double rangeX_1, rangeX_2;
  int k = 0;
  rangeX_1 = .410;
  rangeX_2 = .509;


 TGraph g_fill1, g_fill2;
  for(int j = 60.; j<rangeX_1*200.; j++){
    for (int i = 0; i < 500; i+=5) {
      g_fill1.SetPoint(k, j/200., i);
      k++;
    }
  }

  k = 0;
  for(int j = rangeX_2*200.; j<=240.; j++){
    for (int i = 0; i < 500; i+=5) {
      g_fill2.SetPoint(k, j/200., i);
      k++;
    }
  }
  g_fill1.SetFillStyle(3004);
  g_fill1.SetMarkerStyle(20);
  g_fill1.SetMarkerColor(33);
  g_fill1.SetMarkerSize(0.75);

  g_fill2.SetFillStyle(3004);
  g_fill2.SetMarkerStyle(20);
  g_fill2.SetMarkerColor(33);
  g_fill2.SetMarkerSize(0.75);


  TMultiGraph multi;
  multi.Add(&g_fill1);
  multi.Add(&g_fill2);
  multi.Add(&g1);

  TCanvas c1;
  c1.SetGridx ();
  c1.SetGridy ();
  c1.SetLeftMargin(.15);
  c1.SetWindowSize (1050, 900);

  multi.SetTitle("; V_{soglia} [V]; #frac{Conteggi}{Minuto} #left[#frac{1}{Minuto}#right]");
  multi.GetHistogram()->GetYaxis()->SetRangeUser(0., 505.);
  multi.GetHistogram()->GetXaxis()->SetRangeUser(0., 1.12);
  multi.GetHistogram()->GetXaxis()->SetTitleSize(0.045);
  multi.GetHistogram()->GetYaxis()->SetTitleSize(0.045);
  multi.Draw("AP");


  //c1.Print("plot_destra.pdf", "pdf");
  theApp.Run();


  return 0;
}
