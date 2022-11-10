// c++ -o analysis_time_flight `root-config --cflags` `root-config --gilibs`  analysis_time_flight.cpp 

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>

#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"

#define Nfile 3

using namespace std;

int main (int argc, char ** argv)
 {  
   ifstream temp_file;
   double temp_dati;
   vector<vector<double>> dati_in;

   //sono letti tre file: TDC, ADC0, ADC1
   for (int i = 0; i < Nfile; i++){
      dati_in.push_back ({});

      temp_file.open (argv[i], ios::in);
 
      if (file.is_open() == false){
        cout << "Impossibile aprire file " << i << endl;
        return 1;
       }

      while (true){
        temp_file >> temp_dati;

        if (temp_file.eof() == true)  break;
        dati_in[i].push_back (temp_dati);
       }

      temp_file.close();
     }
      
   TCanvas c;

   c.SetTickx ();
   c.SetTicky ();
   c.SetGridx ();
   c.SetGridy ();
   c.SetWindowSize (1050, 900);
   c.SetLeftMargin (0.15);

   TGraph g_adc0, g_adc1;  //adc0 = stop ; adc1 = start

   for (int i = 0; i < dati_in[0].size(); ++i)  //n.b. rivedere dimensione vectors dopo i tagli
     {
       g_adc0.SetPoint (g_adc0.GetN(), dati_in[1][i], dati_in[0][i]); //dati_in[0] --> TDC ; dati_in[1] --> ADC0 (S2) ; dati_in[2] --> ADC1 (S1)
       g_adc1.SetPoint (g_adc1.GetN(), dati_in[2][i], dati_in[0][i]); 
     }   

   g_adc0.SetMarkerStyle (8);
   g_adc0.SetMarkerColor (kBlue);
   g_adc0.GetXaxis()->SetTitleOffset (1.2);
   g_adc0.SetTitle ("TDC vd ADC;ADC;TDC");

   g_adc1.SetMarkerStyle (8);
   g_adc1.SetMarkerColor (kRed);

   c.cd ();
   g_adc0.Draw ("AP");
   g_adc1.Draw ("PSAME");
   c.Print ("TDC vd ADC.png", "png");

   return 0;
 }
