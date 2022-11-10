// c++ -o analisi `root-config --cflags --glibs` libreria.cc analisi.cpp
/*
Questo script serve per graficare l'andamento dei conteggi e dell'efficienza dei due rivelatori grandi in funzione della tensione di alimentazione. Servono i files libreria.h e libreria.cc

N.B. L'ordine dei canali è il seguente: conteggi T1,S1,S2,T2 - coincidenze doppie - coincidenze triple S2,S1

N.B. Controllare che l'errore inserito sull'efficienza sia corretto
*/


#include "libreria.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>

#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TMath.h"
#include "TF1.h"


using namespace std;

int main (int argc, char ** argv)
 {
   vector <double> V, n_T1, n_S1, n_S2, n_T2, n_double, n_triple1, n_triple2;
   lettura_coincidenza ("dati2.txt", V, n_T1, n_S1, n_S2, n_T2, n_double, n_triple1, n_triple2);

   int n_data = 5; //dati1 -> 15 ; dati1 -> 5 ; dati3 ->
   double eff1[n_data], eff2[n_data], err1[n_data], err2[n_data], errX[n_data], X[n_data];  

   for (int i = 0; i < V.size(); ++i)
     {
       eff1[i] = (n_triple1[i] / n_double[i]);
       eff2[i] = (n_triple2[i] / n_double[i]);
       err1[i] = (sqrt (eff1[i]*(1-eff1[i])/n_double[i]));
       err2[i] = (sqrt (eff2[i]*(1-eff2[i])/n_double[i]));
       X[i] = V[i];
       errX[i] = 0;
     }
      

   TCanvas c_1, c_2, c_3, c_4;

   c_1.SetTickx ();
   c_1.SetTicky ();
   c_1.SetGridx ();
   c_1.SetGridy ();
   c_1.SetWindowSize (1050, 900);
   c_1.SetLeftMargin (0.15);

   c_2.SetTickx ();
   c_2.SetTicky ();
   c_2.SetGridx ();
   c_2.SetGridy ();
   c_2.SetWindowSize (1050, 900);
   c_2.SetLeftMargin (0.15);

   c_3.SetTickx ();
   c_3.SetTicky ();
   c_3.SetGridx ();
   c_3.SetGridy ();
   c_3.SetWindowSize (1050, 900);
   c_3.SetLeftMargin (0.15);

   c_4.SetTickx ();
   c_4.SetTicky ();
   c_4.SetGridx ();
   c_4.SetGridy ();
   c_4.SetWindowSize (1050, 900);
   c_4.SetLeftMargin (0.15);

   TGraphErrors g_eff1 (n_data, X, eff1, errX, err1);
   TGraphErrors g_eff2 (n_data, X, eff2, errX, err2);
   TGraph g_counts1, g_counts2;

   for (int i = 0; i < V.size(); ++i)
     {
       g_counts1.SetPoint (g_counts1.GetN(), V[i], n_S1[i]);
       g_counts2.SetPoint (g_counts2.GetN(), V[i], n_S2[i]);
     }   

   g_eff1.SetMarkerStyle (8);
   g_eff1.SetMarkerColor (kGreen + 2);
   g_eff1.GetXaxis()->SetTitleOffset (1.2);
//   g_eff1.SetTitle ("Efficienza rivelatore superiore #(){#Deltat = 3 min, V_{soglia} = 800 mV};V [V];#epsilon_{S1}");  //dati1
   g_eff1.SetTitle ("Efficienza rivelatore superiore #(){#Deltat = 3 min, V_{soglia} = 800 mV};V [V];#epsilon_{S1}");  //dati2

   g_eff2.SetMarkerStyle (8);
   g_eff2.SetMarkerColor (kRed);
   g_eff2.GetXaxis()->SetTitleOffset (1.2);
//   g_eff2.SetTitle ("Efficienza rivelatore inferiore #(){#Deltat = 3 min, V_{soglia} = 798 mV};V [V];#epsilon_{S2}");  //dati1
   g_eff2.SetTitle ("Efficienza rivelatore inferiore #(){#Deltat = 3 min, V_{soglia} = 798 mV};V [V];#epsilon_{S2}");  //dati2

   g_counts1.SetMarkerStyle (8);
   g_counts1.SetMarkerColor (kGreen + 2);
   g_counts1.GetXaxis()->SetTitleOffset (1.2);
//   g_counts1.SetTitle ("Conteggi rivelatore superiore #(){#Deltat = 3 min, V_{soglia} = 800 mV};V [V];counts");  //dati1
   g_counts1.SetTitle ("Conteggi rivelatore superiore #(){#Deltat = 3 min, V_{soglia} = 802 mV};V [V];counts");  //dati2

   g_counts2.SetMarkerStyle (8);
   g_counts2.SetMarkerColor (kRed);
   g_counts2.GetXaxis()->SetTitleOffset (1.2);
//   g_counts2.SetTitle ("Conteggi rivelatore inferiore #(){#Deltat = 3 min, V_{soglia} = 798 mV};V [V];counts");  //dati1
   g_counts2.SetTitle ("Conteggi rivelatore inferiore #(){#Deltat = 3 min, V_{soglia} = 396 mV};V [V];counts");  //dati2


   c_1.cd ();
   g_eff1.Draw ("AP");

   c_2.cd ();
   g_eff2.Draw ("AP");

   c_3.cd ();
   g_counts1.Draw ("AP");

   c_4.cd ();
   g_counts2.Draw ("AP");

// dati1
/* 
   c_1.Print ("Grafici eff - V per S1 (V_soglia = 800 mV).png", "png");
   c_2.Print ("Grafici eff - V per S2 (V_soglia = 798 mV).png", "png");
   c_3.Print ("Grafici counts - V per S1 (V_soglia = 800 mV).png", "png");
   c_4.Print ("Grafici counts - V per S2 (V_soglia = 798 mV).png", "png");
*/

// dati2

   c_1.Print ("Grafici eff - V per S1 (V_soglia = 802 mV).png", "png");
   c_2.Print ("Grafici eff - V per S2 (V_soglia = 396 mV).png", "png");
   c_3.Print ("Grafici counts - V per S1 (V_soglia = 802 mV).png", "png");
   c_4.Print ("Grafici counts - V per S2 (V_soglia = 396 mV).png", "png");


   return 0;
 }

