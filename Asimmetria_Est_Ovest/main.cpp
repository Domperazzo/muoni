/*
  c++ -o main main.cpp `root-config --glibs --cflags`

  da fittare con un cos^2(x) però uno per grafico
*/

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



int main(int argc, char **argv){

    gStyle->SetOptFit(1112);
    TApplication theApp("theApp", &argc, argv);
    int angoliEst = 0;
    int angoliOvest = 0;
    std::ifstream datiEst;
    datiEst.open("Dati/medie_angoliEst.txt", std::ios::in);
    std::vector<double> v_angoliEst, v_meanEst, v_sigmaEst;
    while (true){
        double p0, p1, p2;
        datiEst >> p0 >> p1 >> p2;
        v_angoliEst.push_back(p0);
        v_meanEst.push_back(p1);
        v_sigmaEst.push_back(p2);

        if (datiEst.eof() == true)break;
        angoliEst++;
    }
    datiEst.close();

    std::ifstream datiOvest;
    datiOvest.open("Dati/medie_angoliOvest.txt", std::ios::in);
    std::vector<double> v_angoliOvest, v_meanOvest, v_sigmaOvest;
    while (true)
    {
        double p00, p01, p02;
        datiOvest >> p00 >> p01 >> p02;
        v_angoliOvest.push_back(p00);
        v_meanOvest.push_back(p01);
        v_sigmaOvest.push_back(p02);
        if (datiOvest.eof() == true)
            break;
        angoliOvest++;
    }
    datiOvest.close();

    double normalizzazione = v_meanOvest[0];

    TGraphErrors gEst, gOvest;
    for (int i = 0; i < v_angoliEst.size(); i++){
        gEst.SetPoint(i, v_angoliEst.at(i), v_meanEst.at(i) / normalizzazione);
        gEst.SetPointError(i, 0., v_sigmaEst.at(i)/normalizzazione);
    }

    for (int i = 0; i < v_angoliOvest.size(); i++){
        gOvest.SetPoint(i, -v_angoliOvest.at(i), v_meanOvest.at(i)/normalizzazione);
        gOvest.SetPointError(i, 0., v_sigmaOvest.at(i)/normalizzazione);
    }
    gEst.SetMarkerStyle(20);
    gEst.SetMarkerSize(0.5);

    gOvest.SetMarkerStyle(20);
    gOvest.SetMarkerSize(0.5);

    TF1 modello("modello", "[0]*cos([1]*x)^2");
    TFitResultPtr fit_resultOvest = gOvest.Fit(&modello, "SQ");
    
    TFitResultPtr fit_resultEst = gEst.Fit(&modello, "SQ");

    TMultiGraph multi;
    multi.Add(&gEst);
    multi.Add(&gOvest);


    TCanvas c1;
    multi.Draw("AP");
    //gOvest.Draw("AP");
    theApp.Run();

    return 0;
}