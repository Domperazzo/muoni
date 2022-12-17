//c++ -o compara_istogrammi compara_istogrammi.cpp `root-config --cflags --glibs`
// ./compara_istogrammi -f1 pdpdp.txt -f2 oisdjcsic.txt

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector> 
#include <random>

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

#define CONV_A 0.119

bool leggi_dati(vector<double> & ampiezzas1, string myFile){
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

int findbin(double min,double max,double amp, double x){
	double mindist = 1000000;
	int minbin = 0;
	//cout << "nbin" << (max - min) / amp << endl;

	for(int i = 0; i < (max - min) / amp; i++){
		double temp = abs(min + i * amp - x);
		if (temp < mindist){
			minbin = i;
			mindist = temp;
		}
	//	cout << temp << " " << mindist << endl;
	}
	return minbin;
}

void montecarlo(TH1D* Esperimento, TH1D* Simulazione){
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<> distrx(-15, 15);
	uniform_real_distribution<> distry(0, 0.25);

	double min = -15.;
	double max = 15.;
	double amp = Esperimento -> GetBinWidth(10);

	cout << "minmaxamp " << min << " " << max << " " << amp << endl;

	int tentativi = 100000;
	int doppie = 0;
	int singolasim = 0;
	int singolaesp = 0;

	for(int i = 0; i < tentativi; i++){
		double x = distrx(gen);			//rand
		double y = distry(gen);			//rand

		int bin = findbin(min,max,amp,x);

		double valyEsp = Esperimento -> GetBinContent(bin);
		double valySpe = Simulazione -> GetBinContent(bin);


		bool presentEsp = false;
		bool presentSim = false;

		if(valySpe >= y) presentEsp = true;
		if(valyEsp >= y) presentSim = true;

		if(presentSim) singolasim++;
		if(presentEsp) singolaesp++;

		if(presentEsp && presentSim) doppie++;

	}


	cout << "L'area in sovrapposizione e' il " << (double)doppie/singolasim*100 << " per cento di quella simulata" << endl;
	cout << "L'area in sovrapposizione e' il " << (double)doppie/singolaesp*100 << " per cento di quella sperimentale" << endl;
}

int main(int argc, char *argv[]){


	START_ARGS
	args -> NewArg("-f1", "Sperimentali");
	args -> NewArg("-f2", "simulazione");
	args -> NewArg("-nbin", "Numero di bin");
	HELP_ARGS

	TApplication myApp("myApp",NULL,NULL);
	gStyle -> SetOptFit(1111);
	gStyle -> SetOptStat(1111);

	vector<double> tempi1;
	vector<double> tempi2;

	if(!(leggi_dati(tempi1, args->GetValue("-f1")))) return 1;
	if(!(leggi_dati(tempi2, args->GetValue("-f2")))) return 1;

	int Nbin;
	if(args -> GetValid("-nbin")) Nbin = stoi(args -> GetValue("-nbin"));
	else Nbin = 300;
	
	TH1D* Esperimento = new TH1D("Esperimento", "Esperimento", Nbin, -15, 15);

	double mean=0;
	for (int i = 1; i < tempi1.size(); i++) mean = mean + tempi1[i];

	mean=mean/tempi1.size();
	for(int i = 0; i < tempi1.size(); i++){
		Esperimento -> Fill((tempi1[i]-mean)*CONV_A,1.0/tempi1.size());
	}



	mean = 0;
	for (int i=1; i < tempi2.size(); i++) mean = mean + tempi2[i];

	mean = mean / tempi2.size();

	TH1D* Simulazione = new TH1D("Simulazione", "Simulazione", Nbin, -15, 15);

	for(int i = 0; i < tempi2.size(); i++)  Simulazione -> Fill(tempi2[i]-mean, 1.0/tempi2.size());
	
	

	Esperimento -> SetLineColor(kRed);
	Simulazione -> SetLineColor(kBlue);

	TCanvas* c_tempi_temp = new TCanvas("c_tempi_temp","c_tempi_temp",0,0,700,500);
	c_tempi_temp -> Divide(1,2);
	c_tempi_temp -> cd(1);
	Esperimento -> Draw();
	c_tempi_temp -> cd(2);
	Simulazione -> Draw();

	TCanvas* c_tempi = new TCanvas("c_tempi","c_tempi",0,0,700,500);
	c_tempi -> cd();

	Esperimento -> Draw();
	Simulazione -> Draw("same");

	double chi = 0;
	int bin_chi = 50;;

	TGraph* Residuo = new TGraph();
	

	for (int i=bin_chi; i<Esperimento->GetNbinsX()-75;i++){
		if (abs(Simulazione->GetBinContent(i)-Esperimento->GetBinContent(i))>0){
			chi=chi+pow((Simulazione->GetBinContent(i)-Esperimento->GetBinContent(i)),2)/Simulazione->GetBinContent(i);
			Residuo->SetPoint(i,Simulazione->GetBinCenter(i),abs(Simulazione->GetBinContent(i)-Esperimento->GetBinContent(i)));
		}
	}

	cout << "Chi2: " << chi << endl;
	cout << "A: " << Simulazione->GetBinCenter(bin_chi)<<" B "<<Simulazione->GetBinCenter(Esperimento->GetNbinsX()-75) << endl;

	TCanvas* c_residuo = new TCanvas("c_residuo","c_residuo",0,0,700,500);

	Residuo -> SetMarkerSize(0.5);
	Residuo -> SetMarkerStyle(20);

	Residuo -> Draw("AP");
	c_residuo->Modified();
	c_residuo->Update();

	montecarlo(Esperimento, Simulazione);

	myApp.Run();
}
