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

int get_i_max(std::vector<double> vec){
    int max = 0;
    int i_max = 0;
    for (int i = 0; i < vec.size(); i++){
        if (vec.at(i) > max){
            max = vec.at(i);
            i_max = i;
        }
    }
    return i_max;
}

double get_perc(int i, std::vector<double> v1, std::vector<double> v2){
    double perc = 0.0;
    if (v1.at(i)>v2.at(i)){
        perc = (v2.at(i)/v1.at(i))*100;
    } else {
        perc = (v1.at(i) / v2.at(i)) * 100;
    }
    return perc;
}

void analisi_differenze(std::vector<double> v1, std::vector<double> v2, std::vector<double> v3){
    std::vector<double> diff;
    for (int i = 0; i < v2.size(); i++){
        diff.push_back(v2.at(i)-v1.at(i));
    }
    for (int i = 0; i < diff.size(); i++){
        if (diff.at(i) < 0){
            diff.at(i) = -diff.at(i);
        }
    }
    int i_max = get_i_max(diff);
    std::cout << "La distanza massima si trova a " << v3.at(i_max) << " gradi e la differenza è di " << diff.at(i_max) << " in percentuale: " << 100-get_perc(i_max, v1, v2)  << "%" << "\n";
}

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
    
    analisi_differenze(v_meanEst, v_meanOvest, v_angoliEst);

 //  v_meanOvest[1] = v_meanEst[1];
   double normalizzazione = v_meanEst[0];
   double n_sigma = v_sigmaEst[0];

    TGraphErrors * punti_E = new TGraphErrors();
    TGraphErrors * punti_O = new TGraphErrors();
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
    punti_E->SetMarkerColor(2);
    punti_E->SetTitle("Est");

    
    punti_O->SetMarkerStyle(20);
    punti_O->SetMarkerSize(0.5);
    punti_O->SetMarkerColor(4);
    punti_O->SetTitle("Ovest");

    TF1 *modelloOvest = new TF1("modelloOvest", "[0]*cos([1]*x)^2", -90., 0.); //[1] ha le dimensioni di radianti/grado
    modelloOvest->SetParName(0, "Ovest0");
    modelloOvest->SetParName(1, "Ovest1");
    modelloOvest->SetParameter(0, 1.);
    modelloOvest->SetLineColor(kGreen+3);
   // modelloOvest->FixParameter(1, 3.14/180);
    TF1 * modelloEst = new TF1 ("modelloEst", "[0]*cos([1]*x)^2", 0. , 90.);
    modelloEst->SetParName(0, "Est0");
    modelloEst->SetParName(1, "Est1");
    modelloEst->SetParameter(0, 1.);
    modelloEst->SetLineColor(kMagenta + 3);
    //  modelloEst->FixParameter(1, 3.14/180);
    
    punti_O->Fit(modelloOvest, "Q", "", -90, 0.);
    punti_E->Fit(modelloEst, "Q", "", 0., 90.);
    
   TMultiGraph * multi = new TMultiGraph;
   multi->Add(punti_O);
   multi->Add(punti_E);
   multi->SetTitle("Conteggi al variare dell'angolo zenitale #vartheta; Angolo zenitale #vartheta; Conteggi");

    TCanvas * c1 = new TCanvas;
    multi->Draw("AP");
    c1->BuildLegend();
    theApp.Run();

   
   return 0;
   
}





















