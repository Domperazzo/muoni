/*
    c++ -o fitdistanze fitdistanze.cpp `root-config --glibs --cflags`
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>


#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TH1F.h"
#include "TF1.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include "TApplication.h"
#include "TAxis.h"
#include "TLatex.h"
#include "TAttFill.h"
#include "TMultiGraph.h"
#include "TStyle.h"

int main(int argc, char **argv)
{
    TApplication theApp("theApp", &argc, argv);
    gStyle->SetOptFit(1112);

    std::ifstream dati;
    dati.open("distanzeSimulazione.txt", std::ios::in);
    std::vector<double> v_true, v_measured, v_errors;
    while (true)
    {
        double p1, p2, p3;
        dati >> p1 >> p2 >> p3;
        if (dati.eof() == true)
            break;
        v_true.push_back(p1);
        v_measured.push_back(p2);
        v_errors.push_back(p3);
    }
    dati.close();

    TGraphErrors g1;
    for (int i = 0; i < v_true.size(); i++)
    {
        g1.SetPoint(i, v_true.at(i), v_measured.at(i) - v_true.at(i));
        g1.SetPointError(i, 0., v_errors.at(i));
    }

    g1.SetMarkerStyle(20);
    g1.SetMarkerSize(0.5);
    
    g1.GetXaxis()->SetTitle("distanza");
    g1.GetYaxis()->SetTitle("scarti");

    //TF1 modello("modello", "x*[0]");
    //TFitResultPtr fit_result = g1.Fit(&modello, "SQ");


    TCanvas c1;
    c1.SetLeftMargin(.15);
    g1.SetTitle("andamento degli scarti");
    g1.Draw("AP");
    theApp.Run();

    return 0;
}
