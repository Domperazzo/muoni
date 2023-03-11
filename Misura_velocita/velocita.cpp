/*
c++ -o velocita velocita.cpp `root-config --glibs --cflags`
./velocita 9.5cm 38.2cm 97.15cm 171.5cm
*/


#include <iostream>
#include <fstream> 
#include <sstream>
#include <string>
#include <cmath>
#include <vector>

#include "TApplication.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TF1.h"
#include "TAxis.h"
#include "TFitResult.h"
#include "TStyle.h"
#include "TGraphPainter.h"
#include "TFrame.h"
#include "TGraphAsymmErrors.h"

double retta (double * x, double * par)
  {
    return par[0] + par[1] * x[0] ;
  }

using namespace std;
int main(int argc, char* argv[]){
  TApplication theApp("theApp", &argc, argv);

  double m[] = {0.1221, 0.0008013}; // coefficiente angolare retta di calibrazione TDC
  double q[] = {0.8727, 0.3014};    // intercetta retta di calibrazione TDC
  
  gStyle->SetOptFit(1112);

  vector<double> TDC, TDC_err, TDC_errb, TDC_errt, dist, dist_err;
  double adc1, adc2, tdc, e_tdc, somma=0, peso, sommapesi=0;
  TCanvas c1 ("c1", "c1", 100, 100, 1000, 1000) ;
  double parametro_correttivo[4] = {11.22/9.5, 40.90/38.2, 99.21/97.15, 172.9/171.5};
  double distanze[4] = {9.5, 38.2, 97.15, 171.5};
  double e_distanze[4] = {1, 1, 1, 1};

  ifstream dati_corretti;
  string fileDati = "Dati/Dati_tdcadc_";
	string estensioneCorr = "_AWcorr.txt";
	
	for(int i = 0; i<4; i++){
    dati_corretti.open((fileDati+argv[i+1]+estensioneCorr).c_str());
   
  	while(!dati_corretti.eof()){
  		dati_corretti >> tdc;
  		dati_corretti >> e_tdc;
  		dati_corretti >> adc1; //ignorato
  		dati_corretti >> adc2; //ignorato
  		
  		peso = 1/pow(e_tdc, 2);
  		somma=somma+tdc*peso;
  		sommapesi=sommapesi+peso;
  	}
  	dati_corretti.close();
  	  
    TDC.push_back(somma/sommapesi);
    cout<<"tdc: "<<somma/sommapesi<<endl;
    TDC_err.push_back(sqrt(1/sommapesi));

    cout<<"err tdc: "<<sqrt(1/sommapesi)<<endl;


    dist.push_back(distanze[i]*parametro_correttivo[i]/100.);
    //cout << "lunghezza corretta in metri: " << distanze[i]*parametro_correttivo[i]/100. << endl;
    dist_err.push_back(e_distanze[i]*parametro_correttivo[i]/100);
    //cout << "errore lunghezza corretta in metri: " << e_distanze[i]*parametro_correttivo[i]/100 << endl;

   
    somma = 0; 
    sommapesi = 0;
  }
	

  
  vector<double> TDC_errb_agg{0.4*m[0] + q[0], 0.1*m[0] + q[0], 0, 0.4*m[0] + q[0]};
  vector<double> TDC_errt_agg{0, 0, 0.1*m[0] + q[0], 0};

  for(int i = 0; i<4; i++){
    TDC_errb.push_back(TDC_err.at(i) + TDC_errb_agg.at(i));
    TDC_errt.push_back(TDC_err.at(i) + TDC_errt_agg.at(i));

  }

  TGraphAsymmErrors funz(dist.size (), &dist[0], &TDC[0], &dist_err[0], &dist_err[0], &TDC_errb[0], &TDC_errt[0]);
  
  //gae->Draw("a2");
  //gae->Draw("p");
	//TGraphErrors funz (dist.size (), &dist[0], &TDC[0], &dist_err[0], &dist_err[0], &TDC_errb[0], &TDC_errt[0]) ;
	funz.SetMarkerStyle (4) ;
	funz.SetMarkerColor (kRed) ;
	
	funz.GetXaxis()->SetTitle("distanze [m]");
  funz.GetYaxis()->SetTitle("tempi[ns]");
  funz.GetXaxis()->SetTitleSize(0.05);
  funz.GetYaxis()->SetTitleSize(0.05);

  funz.SetTitle(" ");
    
  TF1 f_fit ("f_fit", retta, 0., 3., 2) ;
  f_fit.SetParNames("q", "m");

  TFitResultPtr fit_result = funz.Fit (&f_fit, "S") ;

  cout << endl ;
	cout.precision (7) ;
  cout << "risultato del fit: " << fit_result->IsValid () << endl ;
  cout << "termine noto : " << f_fit.GetParameter (0) << "\t+- " << f_fit.GetParError (0) << endl ;
  cout << "pendenza (inverso velocità muoni)     : " << f_fit.GetParameter (1) << "\t+- " << f_fit.GetParError (1) << endl ;
    
  cout<< "valore velocita: " << pow(10, 9)/f_fit.GetParameter (1) << "m/s +- " << f_fit.GetParError (1)*pow(10, 9)/pow(f_fit.GetParameter (1), 2) << "m/s" << endl;

  c1.SetLeftMargin(0.15);
  c1.SetBottomMargin(0.15);
  funz.Draw ("AP") ;
  c1.Print ("velocità_finale.pdf", "pdf") ; 

  theApp.Run();
  return 0;
}
