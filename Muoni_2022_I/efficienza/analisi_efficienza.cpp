/*
	c++ -o analisi_efficienza analisi_efficienza.cpp `root-config --glibs --cflags`
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
//abbiamo usato una soglia di 800mV
  std::ifstream dati;
  dati.open("dati.txt", std::ios::in);
  std::vector<double> v_AlimentazioneS1, v_AlimentazioneS2, v_doppia, v_triplaS1, v_triplaS2, v_conteggiS1, v_conteggiS2;
  while (true) {
      double AlimentazioneS1, AlimentazioneS2, doppia, triplaS1, triplaS2, conteggiS1, conteggiS2;
      dati >> AlimentazioneS1;
      dati >> AlimentazioneS2;
      dati >> triplaS1;
      dati >> triplaS2;
      dati >> doppia;
      dati >> conteggiS1;
      dati >> conteggiS2;
      if(dati.eof()==true) break;
      v_AlimentazioneS1.push_back(AlimentazioneS1);
      v_AlimentazioneS2.push_back(AlimentazioneS2);
      v_triplaS1.push_back(triplaS1);
      v_triplaS2.push_back(triplaS2);
      v_doppia.push_back(doppia);
      v_conteggiS1.push_back(conteggiS1);
      v_conteggiS2.push_back(conteggiS2);

  }
  dati.close();

  std::vector<double> v_efficienzaS1, v_efficienzaS2;

  for (int i = 0; i < v_conteggiS1.size(); i++) {
    v_efficienzaS1.push_back(v_triplaS1.at(i)/v_doppia.at(i));
    v_efficienzaS2.push_back(v_triplaS2.at(i)/v_doppia.at(i));
  }

  TGraphErrors g_S1, g_S2, g_conteggiS1, g_conteggiS2;
  for (int i = 0; i < v_efficienzaS1.size(); i++) {
    g_S1.SetPoint(i, v_AlimentazioneS1.at(i), v_efficienzaS1.at(i));
    g_S2.SetPoint(i, v_AlimentazioneS2.at(i), v_efficienzaS2.at(i));
    g_conteggiS1.SetPoint(i, v_AlimentazioneS1.at(i), v_conteggiS1.at(i));
    g_conteggiS2.SetPoint(i, v_AlimentazioneS2.at(i), v_conteggiS2.at(i));
  }

  g_S1.RemovePoint(10);

  g_S1.SetMarkerStyle(20);
  g_S1.SetMarkerSize(0.5);
  g_S2.SetMarkerStyle(20);
  g_S2.SetMarkerSize(0.5);

  g_conteggiS1.SetMarkerStyle(20);
  g_conteggiS1.SetMarkerSize(0.5);
  g_conteggiS2.SetMarkerStyle(20);
  g_conteggiS2.SetMarkerSize(0.5);

  TCanvas c1;
  c1.SetGridx();
  c1.SetGridy();
  c1.SetLeftMargin(.15);
  //c1.SetWindowSize (1050, 900);

  g_S1.SetTitle("Conteggi degli eventi rilevati da S1 al variare della tensione di alimentazione; V_{alimentazione} [V]; Efficienza");
  //g_S1.GetHistogram()->GetYaxis()->SetRangeUser(0., 505.);
  //g_S1.GetHistogram()->GetXaxis()->SetRangeUser(0., 1.12);
  g_S1.Draw("ALP");

  TCanvas c2;
  c2.SetGridx();
  c2.SetGridy();
  c2.SetLeftMargin(.15);
  //c2.SetWindowSize (1050, 900);

  g_S2.SetTitle("Conteggi degli eventi rilevati da S2 al variare della tensione di alimentazione; V_{alimentazione} [V]; Efficienza");
  //g_S2.GetHistogram()->GetYaxis()->SetRangeUser(0., 505.);
  //g_S2.GetHistogram()->GetXaxis()->SetRangeUser(0., 1.12);
  g_S2.Draw("ALP");

  TCanvas c3;
  c3.SetGridx();
  c3.SetGridy();
  c3.SetLeftMargin(.15);
  //c2.SetWindowSize (1050, 900);

  g_conteggiS1.SetTitle("Conteggi vs Alimentazione per S1; V_{alimentazione} [V]; Conteggi");
  //g_S2.GetHistogram()->GetYaxis()->SetRangeUser(0., 505.);
  //g_S2.GetHistogram()->GetXaxis()->SetRangeUser(0., 1.12);
  g_conteggiS1.Draw("ALP");

  TCanvas c4;
  c4.SetGridx();
  c4.SetGridy();
  c4.SetLeftMargin(.15);
  //c2.SetWindowSize (1050, 900);

  g_conteggiS2.SetTitle("Conteggi vs Alimentazione per S2; V_{alimentazione} [V]; Conteggi");
  //g_S2.GetHistogram()->GetYaxis()->SetRangeUser(0., 505.);
  //g_S2.GetHistogram()->GetXaxis()->SetRangeUser(0., 1.12);
  g_conteggiS2.Draw("ALP");

  theApp.Run();

  return 0;
}
