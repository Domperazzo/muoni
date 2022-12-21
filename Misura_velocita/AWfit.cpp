#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

#include "TF1.h"
#include "TGraphErrors.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TFitResult.h"
#include "TStyle.h"
#include "TLatex.h"

#define Vs 1.4 //tensione di soglia in V

using namespace std ;
double funz(double * x, double * par)
  {
    return par[0] + par[1] * log(x[0]/(x[0]-Vs)) ;
  }


int main (int argc, char ** argv)
{
  double TDCdata, ADCdata1, ADCdata2, TDCdata_corretto;
  double m[] = {0.1221, 0.0008013}; //coefficiente angolare retta di calibrazione TDC
  double q[] = {0.8727, 0.3014}; //intercetta retta di calibrazione TDC
    
  vector<double> vx ;
  vector<double> vy ;
  vector<double> ex ;
  vector<double> ey ;
  double k;

  gStyle->SetOptFit(1112);

  TF1 f ("f", funz, 0., 1000, 2) ;
  f.SetParameter (0, 31.5) ;
  f.SetParameter (1, 12.8) ;
  f.SetParName(0, "k");
  f.SetParName(1, "#tau");


  ifstream myfile;
  myfile.open("Dati_tdcadc_38,2cm.txt");
  while(!myfile.eof())
  {
    myfile >> TDCdata;
    TDCdata_corretto = TDCdata*m[0] + q[0];
    if(TDCdata_corretto >= pow(10, -8)){ //escludo i numeri bassi perche altrimenti il fit col log non da successo, la domanda è perchè ci sono questi dati strani?
    vy.push_back (TDCdata_corretto);
    ey.push_back (0);
    }

    myfile >> ADCdata1;
    myfile >> ADCdata2;
    if(ADCdata2>=2){
    vx.push_back (ADCdata2) ;
    ex.push_back (0) ;
    }  
  }
  myfile.close();

  TGraphErrors g_funz (vx.size (), &vx[0], &vy[0], &ex[0], &ey[0]) ;

  TFitResultPtr fit_result = g_funz.Fit (&f, "S") ;

  cout.precision (5) ;
  cout << "risultato del fit: " << fit_result->IsValid () << endl ;
  cout << "k : " << f.GetParameter (0) << "\t+- " << f.GetParError (0) << endl ;
  cout << "tau : " << f.GetParameter (1) << "\t+- " << f.GetParError (1) << endl ;

  g_funz.SetMarkerStyle (4) ;

  g_funz.SetTitle("tempi di volo Vs energia 2");
  g_funz.GetXaxis()->SetTitle("ADC2");
  g_funz.GetYaxis()->SetTitle("tempo[ns]");
  
  g_funz.GetXaxis()->SetNdivisions(5);


  TCanvas c1 ("c1", "", 800, 800) ;
  g_funz.Draw ("AP") ;
  c1.Print ("AWfit_38,2cm_energia2.pdf", "pdf") ; 

  return 0 ;
}