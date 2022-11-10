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

int main(int argc, char **argv) {
  TApplication theApp("theApp", &argc, argv);
  gStyle->SetOptFit(1112);

  std::ifstream dati;
  dati.open("Tdestra.txt", std::ios::in);
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
  soglia.open("soglie.txt", std::ios::in);
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
  for (int i = 0; i < v_media.size(); i++) {
    g1.SetPoint(i, v_soglie1.at(i), v_media.at(i)/v_tempo.at(i));
  }

  for (int i = 0; i < v_sigma.size(); i++) {
    g1.SetPointError(i, 0., sqrt(v_media.at(i))/v_tempo.at(i)); //distribuzione poissoniana -> sigma=rad(n)
  }


  g1.SetMarkerStyle(20);
  g1.SetMarkerSize(0.5);

  g1.SetTitle("Rate vs Soglia");
  g1.GetHistogram()-> GetXaxis()->SetTitle("V_{soglia}");
  //g1.GetHistogram()->GetYaxis()->SetRangeUser(-5., 600.);
  g1.GetHistogram()-> GetYaxis()->SetTitle("Conteggi/Minuto");

  g1.RemovePoint(1);
  g1.RemovePoint(7);





  //double rangeX_min, rangeX_max;

  //trova_plateu(rangeX_min, rangeX_max, v_soglie1, v_media, v_sigma);

  //rangeX_min = 1.1;
  //rangeX_max = 1.25;

/*
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
*/
  TCanvas c1;
  c1.SetGridx ();
  c1.SetGridy ();
  c1.SetWindowSize (1050, 900);
  c1.SetLeftMargin (0.15);

  g1.SetTitle("Conteggi degli eventi al variare della tensione di soglia; V_{soglia} [V]; #frac{Conteggi}{Minuto} #left[#frac{1}{Minuto}#right]");
  g1.Draw("ACP");

  //c1.Print("plottissimo.pdf", "pdf");
  theApp.Run();


  return 0;
}
