#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

#include "TF1.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TFitResult.h"



double logo (double * x, double * par)
  {
    return par[0] + par[1] * log(x[0]/(x[0]-par[2])) ;
  }

using namespace std ;

int main (int argc, char ** argv)
  {
    vector<double> ADC1, ADC2, TDC, x1, e1, y1, ex1, x2, e2, ex2, y2;
    double adc1, adc2, tdc, maxADC1=0, minADC1=10000, maxADC2=0, minADC2=10000, bin1, bin2, somma1=0, somma1q=0, somma2=0, somma2q=0, conta1=0, conta2=0, i=0, j=0;

  ifstream myfile;
    myfile.open("Dati_tdcadc_38,2cm.txt");
 
  while(!myfile.eof())
  {
  
    myfile >> tdc;
    myfile >> adc1;
    myfile >> adc2;
    if(adc1>= 20 && adc1<=300 && adc2 <= 220){

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

    bin1=(maxADC1-minADC1)/100;
    
    bin2=(maxADC2-minADC2)/100;
    
    for(i=0;i<100;i++){
        
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
        
        cout<<x1.at(i)<<" \n "<<endl;
        
        e1.push_back(0.05*sqrt(somma1q/conta1+(somma1/conta1)*(somma1/conta1)));
        y1.push_back(somma1/conta1);

        cout<<y1.at(i)<<" \n "<<endl;

        ex1.push_back(0);
        x2.push_back(minADC2+i*bin2+bin2/2);
        e2.push_back(0.05*sqrt(somma2q/conta2+(somma2/conta2)*(somma2/conta2)));
        y2.push_back(somma2/conta2);
        ex2.push_back(0);
        
        somma1=0;
        conta1=0;
        somma1q = 0;
        somma2=0;
        conta2=0;
        somma2q = 0;
    }




    TGraphErrors funz (x1.size (), &x1[0], &y1[0], &ex1[0], &e1[0]) ;
    funz.SetMarkerStyle (4) ;
    funz.SetMarkerColor (kRed) ;

  TF1 f_fit ("f_fit", logo, minADC1-bin1, maxADC2+bin2, 3) ;
  f_fit.SetParameter (0, 31.5) ;
  f_fit.SetParameter (1, 12.8) ;
  f_fit.SetParameter (2, 18) ;
  TFitResultPtr fit_result = funz.Fit (&f_fit, "S") ;

  cout << endl ;
  cout.precision (3) ;
  cout << "risultato del fit: " << fit_result->IsValid () << endl ;
  cout << "termine noto : " << f_fit.GetParameter (0) << "\t+- " << f_fit.GetParError (0) << endl ;
  cout << "pendenza     : " << f_fit.GetParameter (1) << "\t+- " << f_fit.GetParError (1) << endl ;

  TCanvas c1 ("c1", "", 800, 800) ;
  funz.Draw ("AP") ;
  c1.Print ("amplitude_walk_ADC1.png", "png") ; 


    return 0 ;
  }