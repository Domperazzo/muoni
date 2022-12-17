//c++ -o finderr finderr.cpp `root-config --cflags --glibs`


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

using namespace std;

#define STEP 0.001
#define TOLLERANZA 0.1

bool leggi_dati(vector<double> & x, vector<double> & e_x, vector<double> & y, vector<double> & e_y, char* myFile){
	double scal1, scal2, scal3, scal4;
	ifstream Infile;
	Infile.open(myFile, fstream::in);
	if (Infile.good () == false){
		cout << "Errore nell'apertura dei file " << myFile << endl;
		return false;
	}
	cout << "Leggo file di dati " << myFile << endl;
	while(true){
		Infile >> scal1 >> scal2 >> scal3 >> scal4;
		x.push_back(scal1);
		e_x.push_back(scal3);
		y.push_back(scal2);
		e_y.push_back(scal4);
		if (Infile.eof() == true) break;

	}
	Infile.close();
	return true;
}

double myFun(double *x, double *par){	//y=ax+b	
	double A = par[0];		//a
	double B = par[1];		//b
	double w = x[0];
	return w*A+B;
}


int main(int argc, char *argv[]){
	vector<double> x;
	vector<double> e_x;
	vector<double> y;
	vector<double> e_y;

	if(!leggi_dati(x,e_x,y,e_y,argv[1])) return false;


	TApplication myApp("myApp",NULL,NULL);
	TCanvas* c_1 = new TCanvas("c_1","c_1",0,0,700,500);
	TGraphErrors * myGraph = new TGraphErrors;
	myGraph -> Draw();
	gStyle -> SetOptFit(1111);
	gStyle -> SetOptStat(1111);

	for(int i = 0; i < x.size(); i++){
		myGraph -> SetPoint(i, x[i], y[i]);
	}

	double err = 0.001;	// o 0
	double chi2 = 0;
	TF1 * fun = new TF1("fun",myFun, 25,40,2);
	fun -> SetParName(0, "A");
	fun -> SetParName(1, "B");

	do{
		err = err + STEP;
		for(int i = 0; i < x.size(); i++) myGraph -> SetPointError(i, err, e_y[i]);
		myGraph -> Fit("fun", "Q");
		chi2 = fun -> GetChisquare() / fun -> GetNDF();
		cout << "chi2 = " << chi2 << endl;
		c_1 -> Modified();
		c_1 -> Update();
	}
	while( abs(chi2 - 1) > TOLLERANZA);

	myGraph -> Fit("fun");

	cout << "\nErrore = " << err << endl;
	
	myApp.Run();
	return 0;
}