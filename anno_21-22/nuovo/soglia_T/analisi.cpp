// c++ -o analisi `root-config --cflags --glibs` libreria.cc analisi.cpp
/*
Questo script permette di graficare l'andamento dei conteggi dei rivelatori in funzione della tensione di soglia
Servono i file ibreria.h e libreria.cc per la function lettura, che salva in quattro vectors i dati V_down, V_up, n_down, n_up
*/

#include "libreria.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>

#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"

using namespace std;

int main (int argc, char ** argv)
 {
   vector <double> V_down, V_up, counts_down, counts_up;
   lettura ("dati.txt", V_down, V_up, counts_down, counts_up);

   TCanvas c_1, c_2;

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

   TGraph g_down, g_up;

   for (int i = 0; i < V_down.size(); ++i)
     {
       g_down.SetPoint (g_down.GetN(), V_down[i], counts_down[i]); 
       g_up.SetPoint (g_up.GetN(), V_up[i], counts_up[i]);
     }   

   g_down.SetMarkerStyle (8);
   g_down.SetMarkerColor (kGreen + 2);
   g_down.GetXaxis()->SetTitleOffset (1.2);
   g_down.SetTitle ("Rivelatore inferiore (tempo misurazione = 3 min);V_{soglia} [mV];n#circ conteggi");

   g_up.SetMarkerStyle (8);
   g_up.SetMarkerColor (kRed);
   g_up.GetXaxis()->SetTitleOffset (1.2);
   g_up.SetTitle ("Rivelatore superiore (tempo misurazione = 3 min);V_{soglia} [mV];n#circ conteggi");


   c_1.cd ();
   g_down.Draw ("AP");

   c_2.cd ();
   g_up.Draw ("AP");

   c_1.Print ("Grafici counts - V_soglia (down) per rivelatori piccoli.png", "png");
   c_2.Print ("Grafici counts - V_soglia (up) per rivelatori piccoli.png", "png");

   return 0;
 }
