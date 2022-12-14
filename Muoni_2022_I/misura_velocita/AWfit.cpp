#include <iostream>
#include <vector>
#include <cmath>

#include "TF1.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TFitResult.h"

#include "funzioni.h"

#define Vs=100 //tensione di soglia

using namespace std ;

double funz(double * x, double * par)
  {
    return par[0] + par[1] * ln(x[0]/(x[0]-Vs)) ;
  }




int main (int argc, char ** argv)
{
  vector<double> vx ;
  vector<double> vy ;
  vector<double> ex ;
  vector<double> ey ;
  double k;

  TF1 f_true ("f_true", funz, 0., 1000, 2) ;
  f_true.SetParameter (0, 0.4) ;
  f_true.SetParameter (1, 1.5) ;

  ifstream myfile;
  myfile.open("a.txt");
  
  while(!myfile.eof())
  {
    myfile >> k;
    vx.push_back (k) ;
    ex.push_back (0) ;
    myfile >> k;
    vy.push_back (k) ;
    ey.push_back (0) ;
    myfile >> k;
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
  c1.Print ("fitAW.png", "png") ; 

  return 0 ;
}