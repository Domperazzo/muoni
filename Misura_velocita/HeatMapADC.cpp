#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>

#include "TH2F.h"
#include "TCanvas.h"
#include "TApplication.h"

#define MIN 0
#define MAX 1000

using namespace std;
int main (int argc, char ** argv)
  {
    TApplication theApp("theApp", &argc, argv);

    double TDC = 0, ADC1=0, ADC2=0;
    TCanvas c1 ("c1", "c1", 100, 100, 1000, 1000) ;
    TH2F h2 ("stats", "Eventi in funzione di ADC1 e ADC2", 200, MIN, MAX, 200, MIN, MAX) ;

    ifstream myfile;
      myfile.open("Dati/Dati_tdcadc_38.2cm.txt");
  
    while(!myfile.eof())
    {
      myfile >> TDC;
      myfile >> ADC1;
      myfile >> ADC2;
      h2.Fill (ADC1,ADC2) ;
    }
    myfile.close();

    c1.SetLeftMargin(.15);

    h2.GetXaxis ()->SetTitle ("energie ADC1") ;
    h2.GetYaxis ()->SetTitle ("energie ADC2") ;
    h2.Draw ("COLZ1") ; //LEGO
    theApp.Run();

    //c1.Print ("Grafici/Eventi_Vs_energie_9,5.pdf", "pdf") ; 

    return 0 ;
  }