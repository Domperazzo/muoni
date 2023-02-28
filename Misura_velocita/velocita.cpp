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
#include "TF1.h"
#include "TAxis.h"
#include "TFitResult.h"
#include "TStyle.h"

double retta (double * x, double * par)
  {
    return par[0] + par[1] * x[0] ;
  }

using namespace std;
int main(int argc, char* argv[]){

    gStyle->SetOptFit(1112);

    vector<double> TDC, TDC_err, dist, dist_err;
    double adc1, adc2, tdc, e_tdc, somma=0, peso, sommapesi=0;
    TCanvas c1 ("c1", "c1", 100, 100, 1000, 1000) ;
    double correzione[4] = {11.22, 40.90, 99.21, 172.9};
    double e_correzione[4] = {2.883/sqrt(pow(10, 8)), 2.279/sqrt(pow(10, 8)), 3.332/sqrt(pow(10, 8)), 2.248/sqrt(pow(10, 8))};
	
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
    cout << "media tempi: " << somma/sommapesi << endl;
    TDC_err.push_back(sqrt(1/sommapesi));
    cout << "errore:" <<sqrt(1/sommapesi) << endl;
   
    dist.push_back(correzione[i]/100.);
    cout << "lunghezza non corretta in metri: " << stod(argv[i+1])/100. << endl;
    dist_err.push_back(e_correzione[i]/100);
   
    somma = 0; 
    sommapesi = 0;
    }
	
	TGraphErrors funz (dist.size (), &dist[0], &TDC[0], &dist_err[0], &TDC_err[0]) ;
	funz.SetMarkerStyle (4) ;
	funz.SetMarkerColor (kRed) ;
	
	funz.GetXaxis()->SetTitle("distanze [m]");
    funz.GetYaxis()->SetTitle("tempi[ns]");
    
    funz.SetTitle("tempi di volo Vs distanze");

    TF1 f_fit ("f_fit", retta, 0., 3., 2) ;
    TFitResultPtr fit_result = funz.Fit (&f_fit, "S") ;

  	cout << endl ;
	cout.precision (7) ;
    cout << "risultato del fit: " << fit_result->IsValid () << endl ;
    cout << "termine noto : " << f_fit.GetParameter (0) << "\t+- " << f_fit.GetParError (0) << endl ;
    cout << "pendenza (inverso velocità muoni)     : " << f_fit.GetParameter (1) << "\t+- " << f_fit.GetParError (1) << endl ;
    
    cout<< "valore velocita: " << pow(10, 9)/f_fit.GetParameter (1) << "m/s +- " << f_fit.GetParError (1)*pow(10, 9)/pow(f_fit.GetParameter (1), 2) << "m/s" << endl;


    funz.Draw ("AP") ;
    c1.Print ("velocità_finale.pdf", "pdf") ; 


    return 0;
}
