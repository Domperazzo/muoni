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
    g_S1.SetPointError(i, .001, sqrt(v_conteggiS1.at(i)/2));
    g_S2.SetPoint(i, v_sogliaS2.at(i), v_conteggiS2.at(i)/2);
    g_S2.SetPointError(i, .001, sqrt(v_conteggiS2.at(i)/2));
  }


  g_S1.SetMarkerStyle(21);
  g_S1.SetMarkerSize(1.2);
  g_S2.SetMarkerStyle(21);
  g_S2.SetMarkerSize(1.2);

  g_S1.GetHistogram()-> GetXaxis()->SetTitle("V_{soglia}");
  g_S1.GetHistogram()-> GetYaxis()->SetTitle("Conteggi/Minuto");

  g_S2.GetHistogram()-> GetXaxis()->SetTitle("V_{soglia}");
  g_S2.GetHistogram()-> GetYaxis()->SetTitle("Conteggi/Minuto");

//grafico soglia S1
  double rangeX_1, rangeX_2;

  rangeX_1 = 1.40;
  rangeX_2 = 1.80;


  TGraph g_fill1, g_fill2;
  
  int k = 0;
  for(int j = 70; j < rangeX_1*100.; j++){
    for(int i = 0; i < 110000; i+=1000){
        g_fill1.SetPoint(k, j/100., i);
        k++;
      }
  }
  k = 0;
  for(int m = rangeX_2*100; m<=200; m++){
    for(int l = 0; l < 110000; l+=1000){
        g_fill2.SetPoint(k, m/100., l);
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
  multi.Add(&g_S1);

  TCanvas c1;
  c1.SetGridx ();
  c1.SetGridy ();
  c1.SetLeftMargin(.15);
  c1.SetBottomMargin(.15);

  c1.SetWindowSize (1050, 900);

  multi.SetTitle(" ; V_{soglia} [V]; #frac{Conteggi}{Minuto} #left[#frac{1}{min}#right]");
  multi.GetHistogram()->GetXaxis()->SetTitleSize(0.05);
  multi.GetHistogram()->GetYaxis()->SetTitleSize(0.04);
  multi.GetHistogram()->GetXaxis()->SetLabelOffset(0.005);
  multi.GetHistogram()->GetYaxis()->SetLabelOffset(0.001);
  multi.Draw("AP");


  c1.Print("Grafici/Soglia S1.pdf", "pdf");


//grafico soglia S2
  double rangeX_3, rangeX_4;

  rangeX_3 = 1.40;
  rangeX_4 = 1.80;


  TGraph g_fill3, g_fill4;
  
  int k2 = 0;
  for(int j2 = 70; j2 < rangeX_3*100.; j2++){
    for(int i2 = 0; i2 < 110000; i2+=1000){
        g_fill3.SetPoint(k2, j2/100., i2);
        k2++;
      }
  }
  k2 = 0;
  for(int m2 = rangeX_4*100; m2<=200; m2++){
    for(int l2 = 0; l2 < 110000; l2+=1000){
        g_fill4.SetPoint(k2, m2/100., l2);
        k2++;
      }
  }


  //g_fill3.SetLineColor(33);
  //g_fill3.SetLineWidth(8000);
  g_fill3.SetFillStyle(3004);
  g_fill3.SetFillColor(33);
  g_fill3.SetMarkerColor(33);
  g_fill3.SetMarkerSize(0.5);
  g_fill3.SetMarkerStyle(20);



  //g_fill4.SetLineColor(33);
  //g_fill4.SetLineWidth(-10200);
  g_fill4.SetFillStyle(3004);
  g_fill4.SetFillColor(33);
  g_fill4.SetMarkerColor(33);
  g_fill4.SetMarkerSize(0.5);
  g_fill4.SetMarkerStyle(20);

  TMultiGraph multi2;
  multi2.Add(&g_fill3);
  multi2.Add(&g_fill4);
  multi2.Add(&g_S2);

  TCanvas c2;
  c2.SetGridx ();
  c2.SetGridy ();
  c2.SetLeftMargin(.15);
  c2.SetBottomMargin(.15);

  c2.SetWindowSize (1050, 900);

  multi2.SetTitle(" ; V_{soglia} [V]; #frac{Conteggi}{Minuto} #left[#frac{1}{min}#right]");
  multi2.GetHistogram()->GetXaxis()->SetTitleSize(0.05);
  multi2.GetHistogram()->GetYaxis()->SetTitleSize(0.04);
  multi2.GetHistogram()->GetXaxis()->SetLabelOffset(0.005);
  multi2.GetHistogram()->GetYaxis()->SetLabelOffset(0.001);
  multi2.Draw("AP");


  c2.Print("Grafici/Soglia S2.pdf", "pdf");
  theApp.Run();

  return 0;
}
