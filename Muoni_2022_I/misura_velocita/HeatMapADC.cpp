#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>

#include "TH2F.h"
#include "TCanvas.h"

#define MIN 0
#define MAX 1000

using namespace std;
int main (int argc, char ** argv)
  {
    double TDC = 0, ADC1=0, ADC2=0;
    TCanvas c1 ("c1", "c1", 100, 100, 1000, 1000) ;
    TH2F h2 ("h2", "Eventi in funzione di ADC1 e ADC2", 200, MIN, MAX, 200, MIN, MAX) ;

    ifstream myfile;
      myfile.open("datiAdcTdc.txt");
  
    while(!myfile.eof())
    {
      myfile >> TDC;
      myfile >> ADC1;
      myfile >> ADC2;
      h2.Fill (ADC1,ADC2) ;
    }
    myfile.close();


    h2.GetXaxis ()->SetTitle ("energie ADC1") ;
    h2.GetYaxis ()->SetTitle ("energie ADC2") ;
    h2.Draw ("LEGO") ;

    c1.Print ("Eventi_Vs_energie.png", "png") ;

    return 0 ;
  }