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
    g1.SetPointError(i, 0.001, sqrt(v_media.at(i)/2));
  }

  g1.SetMarkerStyle(21);
  g1.SetMarkerSize(0.75);


  double rangeX_1, rangeX_2;

  rangeX_1 = .400; //0.41
  rangeX_2 = .510; //0.50


  TGraph g_fill1, g_fill2;

  int k = 0;
  for(int j = 66; j < rangeX_1*200.; j++){
    for(int i = 0; i < 350; i+=2){
      //  std::cout<<i<<std::endl;
        g_fill1.SetPoint(k, j/200., i);
        k++;
      }
  }
  k = 0;
  for(int m = rangeX_2*200; m<=164; m++){
    for(int l = 0; l < 350; l+=2){
        g_fill2.SetPoint(k, m/200., l);
        k++;
      }
  }


  //g_fill1.SetLineColor(33);
  //g_fill1.SetLineWidth(8000);
  g_fill1.SetFillStyle(3004);
  g_fill1.SetFillColor(33);
  g_fill1.SetMarkerColor(33);
  g_fill1.SetMarkerSize(0.5);
  g_fill1.SetMarkerStyle(20);



  //g_fill2.SetLineColor(33);
  //g_fill2.SetLineWidth(-10200);
  g_fill2.SetFillStyle(3004);
  g_fill2.SetFillColor(33);
  g_fill2.SetMarkerColor(33);
  g_fill2.SetMarkerSize(0.5);
  g_fill2.SetMarkerStyle(20);


  TMultiGraph multi;
  multi.Add(&g_fill1);
  multi.Add(&g_fill2);
  multi.Add(&g1);

  TCanvas c1;
  c1.SetGridx ();
  c1.SetGridy ();
  c1.SetLeftMargin(.15);
  c1.SetBottomMargin(.15);

  c1.SetWindowSize (1150, 900);


  multi.SetTitle(" ; V_{soglia} [V]; #frac{Conteggi}{Minuto} #left[#frac{1}{min}#right]");
  multi.GetHistogram()->GetYaxis()->SetRangeUser(0., 360.);
  multi.GetHistogram()->GetXaxis()->SetRangeUser(0., 0.90);
  multi.GetHistogram()->GetXaxis()->SetTitleSize(0.05);
  multi.GetHistogram()->GetYaxis()->SetTitleSize(0.04);
  multi.GetHistogram()->GetXaxis()->SetLabelOffset(0.005);
  multi.GetHistogram()->GetYaxis()->SetLabelOffset(0.001);

  multi.Draw("AP");


  c1.Print("Grafici/plot_sinistra(rivelatore1).pdf", "pdf");
  theApp.Run();


  return 0;
}
