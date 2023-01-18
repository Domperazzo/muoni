/*
  c++ -o main main.cpp lib.cc `root-config --glibs --cflags`
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>

#include "TF1.h"
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

    std::ifstream dati;
    dati.open("medie_angoli.txt", std::ios::in);
    std::vector<double> v_angoli, v_conteggi;
    while (true){
        double angolo, conteggi;
        dati >> angolo >> conteggi;
        if (dati.eof() == true)
            break;
        v_angoli.push_back(angolo);
        v_conteggi.push_back(conteggi);
        
    }
    dati.close();

    TGraphErrors


    theApp.run();

    return 0;
}