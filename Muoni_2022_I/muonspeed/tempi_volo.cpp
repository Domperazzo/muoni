//c++ -o tempi_volo tempi_volo.cpp `root-config --cflags --glibs`
// ./tempi_volo /mnt/c/Users/Rodolfo/Desktop/calibrazione/3/ 

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

#include "libraryCoinc.h"

using namespace std;

#define DEBUG false

#define AUTOLIMIT true

#define ADC0_FILE "datiADC0.txt"
#define ADC1_FILE "datiADC1.txt"
#define TDC_FILE "datiTDC.txt"

#define E_TDC 1.72
#define E_ADC 1


#define MINS1 0
#define MAXS1 900
#define MINS2 0
#define MAXS2 900

#define CHAN_ADC0 1
#define CHAN_ADC1 1
#define CHAN_TDC  1

#define BINTEMPI 100

#define DISTANZA 0.82
#define E_DIST 0.02

#define CONVERSIONETDC 0.1197 + 2.116
#define CONV_A 0.1197
#define CONV_B 2.116
#define ERR_CONV_A 0.000537
#define ERR_CONV_B 0.2389

#define RITARDO 30.6
#define E_RIT 2

vector<evento*> v_eventi;


	//setlimits 
bool fun1 = true;


int main(int argc, char *argv[]){

	if (DEBUG == true)	cout << "\nLETTURA DEI FILE" << endl;
	//----------------------------------------------------------------------
	// 						    lettura dei file
	//----------------------------------------------------------------------

	vector<double> ampiezzas1;
	vector<double> ampiezzas2;
	vector<double> tempi;

	int dist = leggi_dati_container(ampiezzas1, ampiezzas2, tempi, argc, argv);

	if (DEBUG == true)	cout << "\nVISUALIZATION" << endl;
	//----------------------------------------------------------------------
	// 						      visualization
	//----------------------------------------------------------------------

	TApplication myApp("myApp",NULL,NULL);
	gStyle -> SetOptFit(1111);
	gStyle -> SetOptStat(1111);

	if (DEBUG == true)	cout << "\nISTOGRAMMI ADC" << endl;
	//----------------------------------------------------------------------
	// 						      istogrammi adc
	//----------------------------------------------------------------------

	TCanvas* c_adc0 = new TCanvas("c_adc0","c_adc0",0,0,700,500);
	TCanvas* c_adc1 = new TCanvas("c_adc1","c_adc1",0,0,700,500);
	TCanvas* c_2d = new TCanvas("c_2d","c_2d",0,0,700,500);

	TH1D* h_adc0 = new TH1D("ADC S1", "ADC S1", 1000, 0, 1000);	
	TH1D* h_adc1 = new TH1D("ADC S2", "ADC S2", 1000, 0, 1000);	
	TH2D* h2_adc01 = new TH2D("ADC S1-S2", "ADC S21-S", 100, 0, 200, 100, 0, 400);

	for(int i = 0; i < v_eventi.size(); i++){
		double adc0 = v_eventi[i] -> Get_adc0();
		double adc1 = v_eventi[i] -> Get_adc1();
		double tdc = v_eventi[i] -> Get_tdc();

		h_adc0 -> Fill(adc0);
		h_adc1 -> Fill(adc1);
		h2_adc01 -> Fill(adc0,adc1);
	}

	c_adc0 -> cd();
	h_adc0 -> Draw();

	c_adc1 -> cd();
	h_adc1 -> Draw();

	c_2d -> cd();
	h2_adc01 -> Draw("COLZ");

	if (DEBUG == true)	cout << "\nGRAFICO INIZIALE" << endl;
	//----------------------------------------------------------------------
	// 						   grafico iniziale
	//----------------------------------------------------------------------

	TCanvas* c_graphv1 = new TCanvas("c_graphv1","c_graphv1",0,0,700,500);
	TGraphErrors * g_adc0 = new TGraphErrors;
	TGraphErrors * g_adc1 = new TGraphErrors;

	//------------find limiti per i grafici
	double mins1_graph;
	double maxs1_graph;
	double mins2_graph;
	double maxs2_graph;

	cout << "\n\nAUTOLIMIT: " << AUTOLIMIT << endl;
	if(AUTOLIMIT == true){
		setlimits(h_adc0, & mins1_graph, & maxs1_graph);
		setlimits(h_adc1, & mins2_graph, & maxs2_graph);
	}
	else{
		mins1_graph = MINS1;
		maxs2_graph = MAXS2;
		mins2_graph = MINS2;
		maxs1_graph = MAXS1;
	}
	cout << "S1: " << mins1_graph << " < x < " << maxs1_graph << endl;
	cout << "S2: " << mins2_graph << " < x < " << maxs2_graph << endl;
	cout << endl;

	graficoiniziale(mins1_graph,maxs1_graph,g_adc0,mins2_graph,maxs2_graph,g_adc1);

	c_graphv1 -> cd();

	g_adc0 -> GetXaxis() -> SetRangeUser(0,maxs2_graph + 20);
	g_adc1 -> GetXaxis() -> SetRangeUser(0,maxs2_graph + 20);

	g_adc0 -> GetXaxis() -> SetLimits(0,maxs2_graph + 20);
	g_adc1 -> GetXaxis() -> SetLimits(0,maxs2_graph + 20);

	g_adc0 -> GetHistogram() -> SetMaximum(300);
	g_adc0 -> GetHistogram() -> SetMinimum(200);
	g_adc1 -> GetHistogram() -> SetMaximum(300);
	g_adc1 -> GetHistogram() -> SetMinimum(200);

	g_adc0 -> Draw("AP z");
	g_adc1 -> Draw("P z same");

	if (DEBUG == true)	cout << "\nFIT" << endl;
	//----------------------------------------------------------------------
	// 						         fit
	//----------------------------------------------------------------------

	TF1 * fun_adc0 = new TF1("fun_adc0",myFun, mins1_graph,maxs1_graph,3);
	fun_adc0 -> SetLineColor(kRed);
	TF1 * fun_adc1 = new TF1("fun_adc1",myFun, mins2_graph,maxs2_graph,3);
	fun_adc1 -> SetLineColor(kBlue);

	fun_adc0 -> SetParName(0, "Tau0");
	fun_adc0 -> SetParName(1, "Vth0");
	fun_adc0 -> SetParName(2, "Off0");

	fun_adc1 -> SetParName(0, "Tau1");
	fun_adc1 -> SetParName(1, "Vth1");
	fun_adc1 -> SetParName(2, "Off1");


	fun_adc0 -> SetParameter(0, -4e4);
	fun_adc0 -> SetParameter(1, 1e-2);
	fun_adc0 -> SetParameter(2, 280);

	fun_adc1 -> SetParameter(0, -4e4);
	fun_adc1 -> SetParameter(1, 1e-2);
	fun_adc1 -> SetParameter(2, 280);


	/*
			//sigmoide
	fun_adc0 -> SetParameter(0, -14.8);
	fun_adc0 -> SetParameter(1, 20.2);
	fun_adc0 -> SetParameter(2, 255);

	fun_adc1 -> SetParameter(0, -14.8);
	fun_adc1 -> SetParameter(1, 20.2);
	fun_adc1 -> SetParameter(2, 255);
*/

	TVirtualFitter::SetMaxIterations(20000);

	TFitResultPtr r_0 = g_adc0 -> Fit("fun_adc0", "R S E M");
	TFitResultPtr r_1 = g_adc1 -> Fit("fun_adc1", "R S E M");

	c_graphv1 -> Modified();
	c_graphv1 -> Update();
	TPaveStats *st0 = (TPaveStats*) g_adc0->FindObject("stats");

	if (st0 == NULL) {
		cout << "st0 is nullptr!!!!" << endl;
	} else {
		st0 -> SetX1NDC(0.3); //new x start position
		st0 -> SetX2NDC(0.6); //new x end position
		c_graphv1 -> Modified();
		c_graphv1 -> Update();
	}

	c_graphv1 -> Modified();
	c_graphv1 -> Update();


	if (DEBUG == true)	cout << "\nGRAFICO CORRETTO" << endl;
	//----------------------------------------------------------------------
	// 						   grafico corretto
	//----------------------------------------------------------------------

	TCanvas* c_corretto = new TCanvas("c_corretto","c_corretto",0,0,700,500);
	c_corretto -> cd();
	TGraphErrors * g_adc0v2 = new TGraphErrors;
	TGraphErrors * g_adc1v2 = new TGraphErrors;

	g_adc0v2 -> GetXaxis() -> SetTitle("canali ADC");
	g_adc0v2 -> GetYaxis() -> SetTitle("canali TDC");
	g_adc0v2 -> SetMarkerStyle(20);
	g_adc0v2 -> SetMarkerSize(0.5);
	g_adc0v2 -> SetMarkerColor(kRed);

	g_adc1v2 -> GetXaxis() -> SetTitle("canali ADC");
	g_adc1v2 -> GetYaxis() -> SetTitle("canali TDC");
	g_adc1v2 -> SetMarkerStyle(20);
	g_adc1v2 -> SetMarkerSize(0.5);
	g_adc1v2 -> SetMarkerColor(kBlue);


	double tau0 	= fun_adc0 -> GetParameter(0);
	double off0 	= fun_adc0 -> GetParameter(2);

	double tau1 	= fun_adc1 -> GetParameter(0);
	double vth1 	= fun_adc1 -> GetParameter(1);
	double off1 	= fun_adc1 -> GetParameter(2);

	TMatrixD cov_0 = r_0 -> GetCovarianceMatrix();

	TMatrixD cov_1 = r_1 -> GetCovarianceMatrix();

	double cov_tv_0 = cov_0(0,1);
	double cov_tv_1 = cov_1(0,1);

	cov_1.Print();
	cov_0.Print();

	cout << "cov0 " << cov_tv_0 << endl;
	cout << "cov1 " << cov_tv_1 << endl;

	for(int i = mins1_graph; i <= maxs1_graph; i++){
		double sum = 0;
		double sumt = 0;
		int count = 0;
		double dev = 1;
		for(int j = 0; j < v_eventi.size(); j++){
			double adc0 = v_eventi[j] -> Get_adc0();
			double tdc = v_eventi[j] -> Get_tdc_adc0(tau0,vth0,tau1,vth1);//Get_tdc_adc0(tau0,vth0,tau1,vth1);//Get_tdc_corretto(tau0,vth0,tau1,vth1);
			if(adc0 == i){
				count++;
				sumt = sumt + tdc;
			}
		}
		sumt = sumt / count;
		if(count == 1) dev = E_TDC;
		int N = g_adc0v2 -> GetN();
		if(count > 0){
			g_adc0v2 -> SetPoint(N,i,sumt);
			g_adc0v2 -> SetPointError(N,E_ADC,dev);
		}
		//cout << "punto: " << N << " " << i << " " << sumt << endl;
	}

	for(int i = mins2_graph; i <= maxs2_graph; i++){
		double sum = 0;
		double sumt = 0;
		int count = 0;
		double dev = 1;
		for(int j = 0; j < v_eventi.size(); j++){
			double adc1 = v_eventi[j] -> Get_adc1();
			double tdc = v_eventi[j] -> Get_tdc_adc1(tau0,vth0,tau1,vth1);//Get_tdc_adc1(tau0,vth0,tau1,vth1);//Get_tdc_corretto(tau0,vth0,tau1,vth1);
			if(adc1 == i){
				count++;
				sumt = sumt + tdc;
			}
		}
		sumt = sumt / count;
		if(count == 1) dev = E_TDC;
		int N = g_adc1v2 -> GetN();
		if(count > 0){
			g_adc1v2 -> SetPoint(N,i,sumt);
			g_adc1v2 -> SetPointError(N,E_ADC,dev);
		}
	}


	g_adc0v2 -> GetXaxis() -> SetRangeUser(0,maxs2_graph+20);
	g_adc1v2 -> GetXaxis() -> SetRangeUser(0,maxs2_graph+20);

	g_adc0v2 -> GetXaxis() -> SetLimits(0,maxs2_graph+20);
	g_adc1v2 -> GetXaxis() -> SetLimits(0,maxs2_graph+20);

	g_adc0v2 -> GetHistogram() -> SetMaximum(350);
	g_adc0v2 -> GetHistogram() -> SetMinimum(200);
	g_adc1v2 -> GetHistogram() -> SetMaximum(350);
	g_adc1v2 -> GetHistogram() -> SetMinimum(200);

	g_adc0v2 -> Draw("AP z");
	g_adc1v2 -> Draw("P z same");
	
	
	if (DEBUG == true)	cout << "\nISTOGRAMMA TDC" << endl;
	//----------------------------------------------------------------------
	// 						   istogramma tdc
	//----------------------------------------------------------------------

	TCanvas* c_tempi = new TCanvas("c_tempi","c_tempi",0,0,700,500);
	c_tempi -> Divide(2,1);


	TH1D* th_tempi = new TH1D("Tempi", "Tempi", 300, 0, 500);

	//th_tempi -> GetXaxis() -> SetLimits(0, 100);
	//th_tempi -> GetXaxis() -> SetRangeUser(0, 100);

	for(int i = 0; i < v_eventi.size(); i++){
		double tdc = v_eventi[i] -> Get_tdc();
		double adc0 = v_eventi[i] -> Get_adc0();
		double adc1 = v_eventi[i] -> Get_adc1();		
		bool adc0_b = adc0 <= maxs1_graph && adc0 >= mins1_graph;
		bool adc1_b = adc1 <= maxs2_graph && adc1 >= mins2_graph;
		if(adc0_b && adc1_b) th_tempi -> Fill(tdc);
	}

	c_tempi->cd(1);	

	c_tempi->Modified();	
	c_tempi->Update();	

	th_tempi -> Draw();

	//correzione
	TH1D* th_tempi_corr = new TH1D("Tempi corretti", "Tempi corretti", 300,0, 500);

	//th_tempi_corr -> GetXaxis() -> SetLimits(0, 70);
	//th_tempi_corr -> GetXaxis() -> SetRangeUser(0, 70);

	for(int i = 0; i < v_eventi.size(); i++){
		double tdc = v_eventi[i] -> Get_tdc_corretto(tau0,vth0,tau1,vth1);
		double adc0 = v_eventi[i] -> Get_adc0();
		double adc1 = v_eventi[i] -> Get_adc1();		
		bool adc0_b = adc0 <= maxs1_graph && adc0 >= mins1_graph;
		bool adc1_b = adc1 <= maxs2_graph && adc1 >= mins2_graph;
		if(adc0_b && adc1_b) th_tempi_corr -> Fill(tdc);
	}

	c_tempi -> cd(2);	
	c_tempi -> Modified();	
	c_tempi -> Update();	
	th_tempi_corr -> Draw();

	if (DEBUG == true)	cout << "\nSTIMA DI V" << endl;
	//----------------------------------------------------------------------
	// 						   stima velocità di m
	//----------------------------------------------------------------------

	TF1 * mygaus = new TF1("mygaus","gaus",0,100);	
	th_tempi_corr -> Fit("mygaus", "L M E");
	double tmedio = mygaus -> GetParameter(1) * CONVERSIONETDC - RITARDO;
	double sigmat = sqrt(pow(mygaus -> GetParameter(2),2)+pow(E_RIT,2)) / 1000000000;
	tmedio = tmedio / 1000000000;
	double c = dist / tmedio;
	double errc = sqrt(pow(E_DIST/tmedio,2)+pow(c,2)*pow(sigmat,2));

	double errt = (mygaus -> GetParameter(2)/sqrt(th_tempi_corr -> GetEntries()))*CONVERSIONETDC;
	double errc2 = pow(CONV_A*errt,2)+pow((tmedio - RITARDO)*ERR_CONV_A,2)+pow(ERR_CONV_B,2);

	cout << "\n\n-------------------------------------------\n" << endl;
	cout << "Delta offset: " << off1 - off0 << " -> "  << (off1 - off0) * CONVERSIONETDC << " ns " << endl;

	cout << "\n\n-------------------------------------------\n" << endl;
	cout << "stima della velocità dei muoni (m/s): " << c << " +- " << errc << endl;

	cout << "\n\n-------------------------------------------\n" << endl;
																										//GetParameter(2)/sqrt(th_tempi_corr -> GetEntries())
	cout << "stima del tempo di volo dei muoni: " << mygaus -> GetParameter(1) * CONVERSIONETDC << " +- " << 
			 sqrt(errc2) <<  " ns " << endl;


	myApp.Run();
	return 0;
}
