#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>

#include "TF1.h"
#include "TMultiGraph.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TFitResult.h"
#include "TGraphErrors.h"
#include "TAxis.h"
#include "TApplication.h"
#include "TStyle.h"
#define USE_MATH_DEFINES

double propagazione_errori( double x, double y, double sigma_x, double sigma_y)
{
	return sqrt( pow(x*sigma_y, 2) + pow(y*sigma_x , 2) )/pow(y,2);
}

int main(int argc, char **argv)
{
    gStyle->SetOptFit(1112);
    TApplication theApp("theApp", &argc, argv);

    std::ifstream datiEst;
    datiEst.open("medie_angoliEst.txt", std::ios::in);
    std::vector<double> v_angoliEst, v_meanEst, v_sigmaEst;
    do
    {
        double p0, p1, p2;
        datiEst >> p0 >> p1 >> p2;
        if (datiEst.eof() == true)
           break;
        v_angoliEst.push_back(p0);
        v_meanEst.push_back(p1);
        v_sigmaEst.push_back(p2);
    }while( datiEst.is_open() == true);
    datiEst.close();

    std::ifstream datiOvest;
    datiOvest.open("medie_angoliOvest.txt", std::ios::in);
    std::vector<double> v_angoliOvest, v_meanOvest, v_sigmaOvest;
    do
    {
        double p00, p01, p02;
        datiOvest >> p00 >> p01 >> p02;
        if (datiOvest.eof() == true)
            break;
        v_angoliOvest.push_back(p00);
        v_meanOvest.push_back(p01);
        v_sigmaOvest.push_back(p02);
        
    }while( datiOvest.is_open() == true);
    datiOvest.close();
    
 //  v_meanOvest[1] = v_meanEst[1];
   double normalizzazione = v_meanEst[0];
   double n_sigma = v_sigmaEst[0];

    TGraphErrors * punti_E = new TGraphErrors("est");
    TGraphErrors * punti_O = new TGraphErrors("ovest");
    for (int i = 0; i < v_angoliEst.size(); i++){
        punti_E->SetPoint(i, v_angoliEst[i], v_meanEst[i] / normalizzazione);
        punti_E->SetPointError(i, 2., propagazione_errori( v_meanEst[i] , normalizzazione , v_sigmaEst[i], n_sigma ) );
    }

    for (int i = 0; i < v_angoliOvest.size(); i++){
        punti_O->SetPoint(i, -v_angoliOvest[i], v_meanOvest[i]/normalizzazione);
        punti_O->SetPointError(i, 2., propagazione_errori(v_meanOvest[i] , normalizzazione , v_sigmaOvest[i], n_sigma));
    }
    
    punti_E->SetMarkerStyle(20);
    punti_E->SetMarkerSize(0.5);
    
    punti_O->SetMarkerStyle(20);
    punti_O->SetMarkerSize(0.5);
    
   TF1 * modelloOvest = new TF1 ("modelloOvest", "[0]*cos([1]*x)^2", -90., 0.);
    modelloOvest->SetParameter(0, 1.);
   // modelloOvest->FixParameter(1, 3.14/180);
    TF1 * modelloEst = new TF1 ("modelloEst", "[0]*cos([1]*x)^2", 0. , 90.);
     modelloEst->SetParameter(0, 1.);
   //  modelloEst->FixParameter(1, 3.14/180);
    
    punti_O->Fit(modelloOvest, "Q", "", -90, 0.);
    punti_E->Fit(modelloEst, "Q", "", 0., 90.);
    
   TMultiGraph * multi = new TMultiGraph;
   multi->Add(punti_O);
   multi->Add(punti_E);
  


    TCanvas * c1 = new TCanvas;
    multi->Draw("AP");
   // c1->BuildLegend();
    theApp.Run();

   
   return 0;
   
}






















