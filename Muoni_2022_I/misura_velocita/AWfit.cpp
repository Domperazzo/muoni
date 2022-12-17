#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

#include "TF1.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TFitResult.h"

#define Vs 1.4 //tensione di soglia in V

using namespace std ;
double funz(double * x, double * par)
  {
    return par[0] + par[1] * log(x[0]/(x[0]-Vs)) ;
  }



int main (int argc, char ** argv)
{
  double TDCdata, ADCdata1, ADCdata2;
  double m[] = {0.1221, 0.0008013};
  double q[] = {0.8727, 0.3014};
    
  vector<double> vx ;
  vector<double> vy ;
  vector<double> ex ;
  vector<double> ey ;
  double k;

  TF1 f_true ("f_true", funz, 0., 1000, 2) ;
  //f_true.SetParameter (0, 0.4) ;
  //f_true.SetParameter (1, 1.5) ;

  ifstream myfile;
  myfile.open("datiAdcTdc.txt");
  
  while(!myfile.eof())
  {
    myfile >> TDCdata;
    vy.push_back (TDCdata*m[0] + q[0]);
    ey.push_back (0);

    myfile >> ADCdata1;
    vx.push_back (ADCdata1) ;
    ex.push_back (0) ;
    myfile >> ADCdata2;
  }
  myfile.close();



  TGraphErrors g_funz (vx.size (), &vx[0], &vy[0], &ex[0], &ey[0]) ;
  g_funz.SetMarkerStyle (4) ;
  g_funz.SetMarkerColor (kRed) ;

  TF1 f_fit ("f_fit", funz, 0., 1000. , 2) ;
  TFitResultPtr fit_result = g_funz.Fit (&f_fit, "S") ;

  cout << endl ;
  cout.precision (3) ;
  cout << "risultato del fit: " << fit_result->IsValid () << endl ;
  cout << "k : " << f_fit.GetParameter (0) << "\t+- " << f_fit.GetParError (0) << endl ;
  cout << "tau : " << f_fit.GetParameter (1) << "\t+- " << f_fit.GetParError (1) << endl ;

  TCanvas c1 ("c1", "", 800, 800) ;
  g_funz.Draw ("AP") ;
  c1.Print ("AWfit.png", "png") ; 

  return 0 ;
}