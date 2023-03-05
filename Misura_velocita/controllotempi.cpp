/*
  c++ -o controllotempi controllotempi.cpp `root-config --glibs --cflags`
  ./controllotempi 9.5cm
  abbassare gli errori di un fattore necessario a rendere il tutto piu bello
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

#include "TF1.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TFitResult.h"
#include "TAxis.h"
#include "TApplication.h"
#include "TStyle.h"


double logo (double * x, double * par)
  {
    return par[0] + par[1]*x[0] ;
  }

using namespace std ;

int main (int argc, char ** argv){

  TApplication theApp("theApp", &argc, argv);
  gStyle->SetOptFit(1112);
  
  double tau1[2], tau2[2], Vs1[2], Vs2[2];
  double e_TDC;

  ifstream parametri;
  parametri.open("Dati/Parametri_medi.txt");
  parametri.ignore(numeric_limits<streamsize>::max(), '\n');
  parametri >> tau1[0] >> tau1[1] >> tau2[0] >> tau2[1] >> Vs1[0] >> Vs1[1] >> Vs2[0] >> Vs2[1];

  parametri.close();


  int nbin = 100;
  double sigma_TDC1;
  double sigma_TDC2;

  double m[] = {0.1221, 0.0008013}; // coefficiente angolare retta di calibrazione TDC
  double q[] = {0.8727, 0.3014};    // intercetta retta di calibrazione TDC

  vector<double> ADC1, ADC2, TDC_megacorretto, e_TDC_megacorretto, TDC_scorretto, eTDC, x1, e1, y1, ex1, x2, e2, ex2, y2;
  double adc1, adc2, tdc_c, tdc_s, maxADC1 = 0, minADC1 = 10000, maxADC2 = 0, minADC2 = 10000, bin1, bin2, peso1, peso2, somma1 = 0, sommapesi1 = 0, somma2 = 0, sommapesi2 = 0, varianza_tdc, i = 0, j = 0;
  double taglioSupADC1, taglioInfADC1, taglioInfADC2, taglioSupADC2;
  ifstream dati;
  string fileDati = "Dati/Dati_tdcadc_";
  string lunghezza = argv[1];
  string estensione = ".txt";
  dati.open((fileDati+lunghezza+estensione).c_str());

  if(stod(argv[1]) == 9.5){
    e_TDC = 10.45; //canali, 
    taglioInfADC1 = 180;
    taglioSupADC1 = 300;
    taglioInfADC2 = 120;
    taglioSupADC2 = 240;
  }
  if(stod(argv[1]) == 38.2){
    e_TDC = 11.91; //canali
    taglioInfADC1 = 140;
    taglioSupADC1 = 320;
    taglioInfADC2 = 120;
    taglioSupADC2 = 240;
  }
  if(stod(argv[1]) == 97.15){
    e_TDC = 16.39; //canali
    taglioInfADC1 = 120;
    taglioSupADC1 = 320;
    taglioInfADC2 = 80;
    taglioSupADC2 = 200;
  }
  if(stod(argv[1]) == 171.5){
    e_TDC = 19.41; //canali
    taglioInfADC1 = 120;
    taglioSupADC1 = 320;
    taglioInfADC2 = 80;
    taglioSupADC2 = 200;
  }

  while (!dati.eof())
  {

    dati >> tdc_s;
    tdc_c = tdc_s * m[0] + q[0];

    dati >> adc1;
    dati >> adc2;

    if (adc1 >= taglioInfADC1 && adc1 <= taglioSupADC1 && adc2 >= taglioInfADC2 && adc2 <= taglioSupADC2){

      ADC1.push_back(adc1);
      ADC2.push_back(adc2);
      TDC_scorretto.push_back(tdc_s);
      TDC_megacorretto.push_back(tdc_c - tau1[0] * log(adc1 / (adc1 - Vs1[0])) + tau2[0] * log(adc2 / (adc2 - Vs2[0]))); /*correzione tempi*/
      varianza_tdc = pow(q[1], 2) + pow(e_TDC*m[0], 2) + pow(tdc_s*m[1], 2) + pow(tau1[1]*log( adc1/(adc1-Vs1[0]) ), 2) + pow(tau2[1]*log(adc2/(adc2-Vs2[0])), 2) + pow(Vs1[1]*tau1[0]/(adc1-Vs1[0]), 2) + pow(Vs2[1]*tau2[0]/(adc2-Vs2[0]), 2);

      e_TDC_megacorretto.push_back( sqrt(varianza_tdc) );
      cout<<" errore tdc corretto: "<<sqrt(varianza_tdc)<<endl;

      if (adc1 < minADC1)
        minADC1 = adc1;

      if (adc2 < minADC2)
        minADC2 = adc2;

      if (adc1 > maxADC1)
        maxADC1 = adc1;

      if (adc2 > maxADC2)
        maxADC2 = adc2;
    }
  }
  dati.close();
  
  
  
  ofstream OutFile; /* Dichiarazione di tipo */
  string estensioneCorr = "_AWcorr.txt";
  OutFile.open ((fileDati+lunghezza+estensioneCorr).c_str()); /* Apertura del file */
  if (!OutFile) {
    cout << "Errore di apertura del file" << endl; /* controllo */
  } else {
  for(int i=0;i<ADC1.size();i++) {
    OutFile << TDC_megacorretto.at(i) <<" "<< e_TDC_megacorretto.at(i) <<" "<< ADC1.at(i) << " " << ADC2.at(i) << "\n" << endl; /* scrittura dati */
  }
  OutFile.close(); /* chiusura file */
  }


  bin1=(maxADC1-minADC1)/nbin;
      
  bin2=(maxADC2-minADC2)/nbin;
      
  for(i=0;i<nbin;i++){
          
    for(j=0;j<ADC1.size();j++){
      if( ( ADC1.at(j)<(minADC1+(i+1)*bin1) )&&( ADC1.at(j)>(minADC1+i*bin1))){
        peso1 = 1/( pow(e_TDC_megacorretto.at(j), 2) ); /* peso = 1/sigma quadra */
        somma1=somma1+TDC_megacorretto.at(j)*peso1;
        sommapesi1=sommapesi1+peso1;

      }
      if( ( ADC2.at(j)<(minADC2+(i+1)*bin2) )&&( ADC2.at(j)>(minADC2+i*bin2))){
        peso2 = 1/( pow(e_TDC_megacorretto.at(j), 2) ); 
        somma2=somma2+TDC_megacorretto.at(j)*peso2;
        sommapesi2=sommapesi2+peso2;

      }
          
          
    }
          
    x1.push_back(minADC1+i*bin1+bin1/2);
    ex1.push_back(1);

    y1.push_back(somma1/sommapesi1);
    e1.push_back(1/sqrt(sommapesi1));


    x2.push_back(minADC2+i*bin2+bin2/2);
    ex2.push_back(1);

    y2.push_back(somma2/sommapesi2);    
    e2.push_back(1/sqrt(sommapesi2));
          
    somma1=0;
    sommapesi1=0;
    somma2=0;
    sommapesi2=0;

  }


  TGraphErrors funz (x1.size (), &x1[0], &y1[0], &ex1[0], &e1[0]) ;
  TGraphErrors funz2 (x2.size (), &x2[0], &y2[0], &ex2[0], &e2[0]) ;

  funz.SetMinimum(20);
  funz.SetMaximum(40);
  
  funz2.SetMinimum(20);
  funz2.SetMaximum(40);

  funz.SetMarkerStyle(105);
  funz.SetMarkerColor(4);
  funz.GetXaxis()->SetTitleSize(0.05);
  funz.GetYaxis()->SetTitleSize(0.05);

  funz2.SetMarkerStyle(105);
  funz2.SetMarkerColor(4);
  funz2.GetXaxis()->SetTitleSize(0.05);
  funz2.GetYaxis()->SetTitleSize(0.05);

  funz.SetTitle("  ");

  funz.GetXaxis()->SetTitle("ADC1");
  funz.GetYaxis()->SetTitle("TDC");

  funz2.SetTitle("  ");

  funz2.GetXaxis()->SetTitle("ADC2");
  funz2.GetYaxis()->SetTitle("TDC");

//fit energia 1
  TF1 f_fit ("f_fit", logo, minADC1-bin1, maxADC1+bin1, 2) ;

  TFitResultPtr fit_result = funz.Fit (&f_fit, "S") ;

  cout << endl ;
  cout.precision (3) ;
  cout << "risultato del fit: " << fit_result->IsValid () << endl ;
  cout << " k: " << f_fit.GetParameter (0) << "\t+- " << f_fit.GetParError (0) << endl ;
  cout << " m: " << f_fit.GetParameter(1) << "\t+- " << f_fit.GetParError(1) << endl;

  TCanvas c1 ("c1", "", 800, 800) ;
  c1.SetLeftMargin(0.15);
  c1.SetBottomMargin(0.15);
  funz.Draw ("AP") ;
  string fileGrafici = "Grafici/";
  string estensionePDF1 = "_corretto_ADC1.pdf";
 

  c1.Print((fileGrafici+lunghezza+estensionePDF1).c_str(), "pdf");

  // fit energia 2

  TF1 f_fit2 ("f_fit2", logo, minADC2-bin2, maxADC2+bin2, 2) ;
  f_fit2.SetParameter (0, 31); 
  TFitResultPtr fit_result2 = funz2.Fit (&f_fit2, "S") ;

  cout << endl ;
  cout.precision (3) ;
  cout << "risultato del fit: " << fit_result2->IsValid () << endl ;
  cout << " k: " << f_fit2.GetParameter (0) << "\t+- " << f_fit2.GetParError (0) << endl ;
  cout << " m: " << f_fit2.GetParameter(1) << "\t+- " << f_fit2.GetParError(1) << endl;

  TCanvas c2 ("c2", "", 800, 800) ;
  c2.SetLeftMargin(0.15);
  c2.SetBottomMargin(0.15);
  funz2.Draw ("AP") ;
  string estensionePDF2 = "_corretto_ADC2.pdf";
  

  c2.Print ((fileGrafici+lunghezza+estensionePDF2).c_str(), "pdf") ; 
  theApp.Run();

    return 0 ;
  }
