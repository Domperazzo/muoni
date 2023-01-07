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



double logo (double * x, double * par)
  {
    return par[0] + par[1] * log(x[0]/(x[0]-par[2])) ;
  }

using namespace std ;

int main (int argc, char ** argv){

  int nbin = 100;
  double sigma_TDC1;
  double sigma_TDC2;

  double m[] = {0.1221, 0.0008013}; //coefficiente angolare retta di calibrazione TDC
  double q[] = {0.8727, 0.3014}; //intercetta retta di calibrazione TDC

  vector<double> ADC1, ADC2, TDC, x1, e1, y1, ex1, x2, e2, ex2, y2;
  double adc1, adc2, tdc, maxADC1=0, minADC1=10000, maxADC2=0, minADC2=10000, bin1, bin2, somma1=0, somma1q=0, somma2=0, somma2q=0, conta1=0, conta2=0, i=0, j=0;

  ifstream myfile;
  myfile.open("Dati_tdcadc_38,2cm.txt");
 
  while(!myfile.eof())
  {
  
    myfile >> tdc;
    tdc = tdc*m[0] + q[0];

    myfile >> adc1;
    myfile >> adc2;
    if(adc1>=140  && adc1<=320 && adc2 >=100 && adc2 <= 240){  

    ADC1.push_back(adc1);
    ADC2.push_back(adc2);
    TDC.push_back(tdc);
    
    if(adc1<minADC1)
        minADC1=adc1;
        
    if(adc2<minADC2)
        minADC2=adc2;
        
    if(adc1>maxADC1)
        maxADC1=adc1;
    
    if(adc2>maxADC2)
        maxADC2=adc2;
    }
  }
  myfile.close();

  bin1=(maxADC1-minADC1)/nbin;
      
  bin2=(maxADC2-minADC2)/nbin;
      
  for(i=0;i<nbin;i++){
          
    for(j=0;j<ADC1.size();j++){
      if( ( ADC1.at(j)<(minADC1+(i+1)*bin1) )&&( ADC1.at(j)>(minADC1+i*bin1))){
        somma1=somma1+TDC.at(j);
        somma1q=somma1q+TDC.at(j)*TDC.at(j);
        conta1++;
      }
      if( ( ADC2.at(j)<(minADC2+(i+1)*bin2) )&&( ADC2.at(j)>(minADC2+i*bin2))){
        somma2=somma2+TDC.at(j);
        somma2q=somma2q+TDC.at(j)*TDC.at(j);
        conta2++;
      }
          
          
    }
          
    x1.push_back(minADC1+i*bin1+bin1/2);
    ex1.push_back(0);
    cout<<x1.at(i)<<" \n "<<endl;

    y1.push_back(somma1/conta1);
    sigma_TDC1 = (somma1q/conta1+(somma1/conta1)*(somma1/conta1))/conta1;
    e1.push_back(sqrt(  pow(q[1], 2) + pow(m[0]*sigma_TDC1, 2) + pow((somma1/conta1)*m[1], 2)  ));
    cout<<y1.at(i)<<" \n "<<endl;

    x2.push_back(minADC2+i*bin2+bin2/2);
    ex2.push_back(0);

    y2.push_back(somma2/conta2);
    sigma_TDC2 = (somma2q/conta2+(somma2/conta2)*(somma2/conta2))/conta2;
    e2.push_back(sqrt(  pow(q[1], 2) + pow(m[0]*sigma_TDC2, 2) + pow((somma2/conta2)*m[1], 2)  ));
          
    somma1=0;
    conta1=0;
    somma1q = 0;
    somma2=0;
    conta2=0;
    somma2q = 0;
  }




  TGraphErrors funz (x1.size (), &x1[0], &y1[0], &ex1[0], &e1[0]) ;
  TGraphErrors funz2 (x2.size (), &x2[0], &y2[0], &ex2[0], &e2[0]) ;

  funz.SetMarkerStyle (4) ;
  funz.SetMarkerColor (kRed) ;
  funz2.SetMarkerStyle (4) ;
  funz2.SetMarkerColor (kRed) ;

  funz.SetTitle("tempi di volo Vs energia 1");

  funz.GetXaxis()->SetTitle("ADC1");
  funz.GetYaxis()->SetTitle("TDC");

  funz2.SetTitle("tempi di volo Vs energia 2");

  funz2.GetXaxis()->SetTitle("ADC2");
  funz2.GetYaxis()->SetTitle("TDC");

//fit energia 1

  TF1 f_fit ("f_fit", logo, minADC1-bin1, maxADC1+bin1, 3) ;
  //f_fit.SetParameter (0, 31); 
  f_fit.SetParameter (1, 44);
  //f_fit.SetParameter (2, 18); //perche i tagli tolgono la parte piu interessante del fit?(dove non è piu una retta)
  TFitResultPtr fit_result = funz.Fit (&f_fit, "S") ;

  cout << endl ;
  cout.precision (3) ;
  cout << "risultato del fit: " << fit_result->IsValid () << endl ;
  cout << " k: " << f_fit.GetParameter (0) << "\t+- " << f_fit.GetParError (0) << endl ;
  cout << " tau: " << f_fit.GetParameter (1) << "\t+- " << f_fit.GetParError (1) << endl ;
  cout << " Vs: " << f_fit.GetParameter (2) << "\t+- " << f_fit.GetParError (2) << endl ;

  TCanvas c1 ("c1", "", 800, 800) ;
  funz.Draw ("AP") ;
  c1.Print ("amplitude_walk_ADC1.png", "png") ; 

// fit energia 2

  TF1 f_fit2 ("f_fit2", logo, minADC2-bin2, maxADC2+bin2, 3) ;
  f_fit2.SetParameter (0, 31); 
  //f_fit2.SetParameter (1, -2);
  f_fit2.SetParameter (2, -0.40); //perche i tagli tolgono la parte piu interessante del fit?(dove non è piu una retta)
  TFitResultPtr fit_result2 = funz2.Fit (&f_fit2, "S") ;

  cout << endl ;
  cout.precision (3) ;
  cout << "risultato del fit: " << fit_result2->IsValid () << endl ;
  cout << " k: " << f_fit2.GetParameter (0) << "\t+- " << f_fit2.GetParError (0) << endl ;
  cout << " tau: " << f_fit2.GetParameter (1) << "\t+- " << f_fit2.GetParError (1) << endl ;
  cout << " Vs: " << f_fit2.GetParameter (2) << "\t+- " << f_fit2.GetParError (2) << endl ;

  TCanvas c2 ("c2", "", 800, 800) ;
  funz2.Draw ("AP") ;
  c2.Print ("amplitude_walk_ADC2.png", "png") ; 


    return 0 ;
  }