//c++ -o histotempi histotempi.cpp `root-config --cflags --glibs`
// ./histotempi /mnt/c/Users/Rodolfo/Desktop/calibrazione/3/ 

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector> 

#include "TApplication.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TVectorD.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TPaveStats.h"
#include "TFitResult.h"
#include "TMatrixD.h"
#include "TVirtualFitter.h"

#include "args.h"

using namespace std;

#define CONVERSIONETDC 0.1209 + 37.29
#define CONV_A 0.1209
#define CONV_B 37.29
#define ERR_CONV_A 0.0004
#define ERR_CONV_B 0.11


bool leggi_dati(vector<double> & ampiezzas1, char* myFile){
	double scal1;
	ifstream Infile;
	Infile.open(myFile, fstream::in);
	if (Infile.good () == false){
		cout << "Errore nell'apertura dei file " << myFile << endl;
		return false;
	}
	cout << "Leggo file di dati " << myFile << endl;
	while(true){
		Infile >> scal1;
		ampiezzas1.push_back(scal1);
		if(scal1 == 0) cout << ampiezzas1.size() << endl;
		if (Infile.eof() == true) break;
	}
	Infile.close();
	cout << "Numero dati: " << ampiezzas1.size() << endl;
	return true;
}

int main(int argc, char *argv[]){

	//----------------------------------------------------------------------
	// 						      visualization
	//----------------------------------------------------------------------

	START_ARGS
	args -> NewArg("-ns", "nanoseconds");
	args -> NewArg("-f", "fit");
	HELP_ARGS

	TApplication myApp("myApp",NULL,NULL);
	gStyle -> SetOptFit(1111);
	gStyle -> SetOptStat(1111);

	TCanvas* c_tempi;

	c_tempi = new TCanvas("c_tempi","c_tempi",0,0,700,500);
	c_tempi -> cd();
	TH1D* th_tempi;

	if(args->GetValue("-ns")=="true") th_tempi = new TH1D("Tempi", "Tempi", 250, 40, 80);
	else th_tempi = new TH1D("Tempi", "Tempi", 250, 0, 500);

	vector<double> tempi;

	if(!(leggi_dati(tempi, argv[1]))) return 1;

	for(int i = 0; i < tempi.size(); i++){
		if(args->GetValue("-ns")=="true"){
			if(tempi[i] < 900) th_tempi -> Fill(tempi[i] * CONVERSIONETDC);
		} 
		else{
			if(tempi[i] < 900) th_tempi -> Fill(tempi[i]);
		}
	}

	th_tempi -> Draw();
	


	double errt, errc2, tmedio;

	if(args->GetValue("-f")=="true"){
		TF1 * mygaus = new TF1("mygaus","gaus",0,100);
		th_tempi -> Fit("mygaus");
		errt = mygaus -> GetParError(1); 
		tmedio = mygaus -> GetParameter(1);
	}
	else{
		tmedio = th_tempi -> GetMean();
		errt = th_tempi -> GetMeanError();
	}
	
	errc2 = pow(CONV_A*errt,2)+pow((tmedio)*ERR_CONV_A,2)+pow(ERR_CONV_B,2);
	cout << "\nTempo di volo = " << tmedio * CONVERSIONETDC << " +- " << sqrt(errc2) << endl;

	

	myApp.Run();

}





