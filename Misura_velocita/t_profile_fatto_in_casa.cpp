/*
  c++ -o t_profile_fatto_in_casa t_profile_fatto_in_casa.cpp `root-config --glibs --cflags`
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
#include "TLatex.h"


double logo (double * x, double * par)
  {
    return par[0] + par[1] * log(x[0]/(x[0]-par[2])) ;
  }

using namespace std ;

int main (int argc, char ** argv){

    gStyle->SetOptFit(1112);
    TApplication theApp("theApp", &argc, argv);

    int nbin = 100;
    double sigma_TDC1;
    double sigma_TDC2;

    double m[] = {0.1221, 0.0008013}; // coefficiente angolare retta di calibrazione TDC
    double q[] = {0.8727, 0.3014};    // intercetta retta di calibrazione TDC

    vector<double> ADC1, ADC2, TDC_scorretto, TDC_corretto, x1, e1, y1, ex1, x2, e2, ex2, y2;
    double adc1, adc2, tdc_s, tdc_c, maxADC1 = 0, minADC1 = 10000, maxADC2 = 0, minADC2 = 10000, bin1, bin2, peso1, peso2, somma1 = 0, sommapesi1 = 0, somma2 = 0, sommapesi2 = 0, i = 0, j = 0;

    ifstream dati;
    dati.open("Dati/Dati_tdcadc_171,5cm.txt");

    while (!dati.eof())
    {

      dati >> tdc_s;
      tdc_c = tdc_s * m[0] + q[0];

      dati >> adc1;
      dati >> adc2;
      if (adc1 >= 140 && adc1 <= 320 && adc2 >= 100 && adc2 <= 240)
      {

        ADC1.push_back(adc1);
        ADC2.push_back(adc2);
        TDC_scorretto.push_back(tdc_s);
        TDC_corretto.push_back(tdc_c);

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

  bin1=(maxADC1-minADC1)/nbin;
      
  bin2=(maxADC2-minADC2)/nbin;
      
  for(i=0;i<nbin;i++){
          
    for(j=0;j<ADC1.size();j++){
      if( ( ADC1.at(j)<(minADC1+(i+1)*bin1) )&&( ADC1.at(j)>(minADC1+i*bin1))){
        peso1 = 1/( pow(q[1], 2) + pow(TDC_scorretto.at(j)*m[1], 2) ); /* peso = 1/sigma quadra */
        somma1=somma1+TDC_corretto.at(j)*peso1;
        sommapesi1=sommapesi1+peso1;

      }
      if( ( ADC2.at(j)<(minADC2+(i+1)*bin2) )&&( ADC2.at(j)>(minADC2+i*bin2))){
        peso2 = 1/( pow(q[1], 2) + pow(TDC_scorretto.at(j)*m[1], 2) ); 
        somma2=somma2+TDC_corretto.at(j)*peso2;
        sommapesi2=sommapesi2+peso2;

      }
          
          
    }
          
    x1.push_back(minADC1+i*bin1+bin1/2);
    ex1.push_back(0);
    //cout<<x1.at(i)<<" \n "<<endl;

    y1.push_back(somma1/sommapesi1);
    e1.push_back(1/sqrt(sommapesi1));
    //cout<<y1.at(i)<<" \n "<<endl;

    x2.push_back(minADC2+i*bin2+bin2/2);
    ex2.push_back(0);

    y2.push_back(somma2/sommapesi2);
    e2.push_back(1/sqrt(sommapesi2));
          
    somma1=0;
    sommapesi1=0;
    somma2=0;
    sommapesi2=0;

  }




  TGraphErrors funz (x1.size (), &x1[0], &y1[0], &ex1[0], &e1[0]) ;
  TGraphErrors funz2 (x2.size (), &x2[0], &y2[0], &ex2[0], &e2[0]) ;

  funz.SetMarkerStyle (105) ;
  funz.SetMarkerColor (4) ;
  funz2.SetMarkerStyle (105) ;
  funz2.SetMarkerColor (4) ;
  funz.SetTitle("Tempi di volo vs Energia rivelatore S1");

  funz.GetXaxis()->SetTitle("ADC1");
  funz.GetYaxis()->SetTitle("TDC");
  funz2.SetTitle("Tempi di volo vs Energia rivelatore S2");

  funz2.GetXaxis()->SetTitle("ADC2");
  funz2.GetYaxis()->SetTitle("TDC");
//fit energia 1

  TF1 f_fit ("f_fit", logo, minADC1-bin1, maxADC1+bin1, 3) ;
  f_fit.SetParName(0, "k");
  f_fit.SetParName(1, "tau");
  f_fit.SetParName(2, "Vs");

  /*
  9:     0 --> 31
         1 --> 13
         2 --> 80
  38,2:  0 --> 31 
         1 --> 0.4 
         2 --> 18 
  97,15: 0 --> 31
         1 --> 0.31
         2 --> 31
  171,5: 0 --> 31
         1 --> 0.04
         2 --> 20

  */
  f_fit.SetParameter (0, 31);
  f_fit.SetParameter (1, 0.04);
  f_fit.SetParameter (2, 20); //perche i tagli tolgono la parte piu interessante del fit?(dove non è piu una retta)
  TFitResultPtr fit_result = funz.Fit (&f_fit, "SQ") ;

  cout << endl ;
  cout.precision (3) ;
  cout << "risultato del fit: " << fit_result->IsValid () << endl ;
  cout << " k: " << f_fit.GetParameter (0) << "\t+- " << f_fit.GetParError (0) << endl ;
  cout << " tau: " << f_fit.GetParameter (1) << "\t+- " << f_fit.GetParError (1) << endl ;
  cout << " Vs: " << f_fit.GetParameter (2) << "\t+- " << f_fit.GetParError (2) << endl ;

  TCanvas c1 ("c1", "", 800, 800) ;
  funz.Draw ("AP") ;
  c1.Print ("Grafici/amplitude_walk_171,5_ADC1.pdf", "pdf");

// fit energia 
  TF1 f_fit2 ("f_fit2", logo, minADC2-bin2, maxADC2+bin2, 3);
  f_fit2.SetParName(0, "k");
  f_fit2.SetParName(1, "tau");
  f_fit2.SetParName(2, "Vs");
  /*
  9:     0 --> 31
         1 --> -3
         2 --> 0.1
  38,2:  0 --> 31 
         1 --> -0.04
         2 --> 18
  97,15: 0 --> 31 commentato
         1 --> -1.86
         2 --> 99
  171,5: 0 --> 36
         1 --> -12
         2 --> 99
  */
  f_fit2.SetParameter (0, 36); 
  f_fit2.SetParameter (1, -12);
  f_fit2.SetParameter (2, 99); 
  TFitResultPtr fit_result2 = funz2.Fit (&f_fit2, "SQ") ;

  cout << endl ;
  cout.precision (3) ;
  cout << "risultato del fit: " << fit_result2->IsValid () << endl ;
  cout << " k: " << f_fit2.GetParameter (0) << "\t+- " << f_fit2.GetParError (0) << endl ;
  cout << " tau: " << f_fit2.GetParameter (1) << "\t+- " << f_fit2.GetParError (1) << endl ;
  cout << " Vs: " << f_fit2.GetParameter (2) << "\t+- " << f_fit2.GetParError (2) << endl ;

  ofstream OutFile;
  OutFile.open ("Dati/Parametri_correzione_171,5.txt");
  if (!OutFile) {
    cout << "Errore di apertura del file" << endl; 
  } else {
  OutFile << "tau1 \t" << " e_tau1 \t" << " tau2 \t" << " e_tau2 \t" << " Vs1 \t" << " e_vs1 \t\t" << " Vs2 \t" << " e_vs2 " << endl;
  OutFile << f_fit.GetParameter (1) << " " << f_fit.GetParError (1) << " \t " << f_fit2.GetParameter (1)*(-1) << " " <<
    f_fit2.GetParError (1) << " \t "<< f_fit.GetParameter (2) << " " << f_fit.GetParError (2) << " \t " << f_fit2.GetParameter (2) << " " << f_fit2.GetParError (2) << " \n" << endl; 
  OutFile.close();
  }

  TCanvas c2 ("c2", "", 800, 800) ;
  funz2.Draw("AP") ;
  c2.Print ("Grafici/amplitude_walk_171,5_ADC2.pdf", "pdf") ; 

  theApp.Run();

    return 0 ;
  }
