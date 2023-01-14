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
#include "TAxis.h"
#include "TApplication.h"
#include "TStyle.h"

#include "lib.h"

int main(int argc, char **argv){

    gStyle->SetOptFit(1112);
    TApplication theApp("theApp", &argc, argv);

    std::vector<std::vector<double>> v_dati;
    
    v_datiEst = readDataFromFile("datiEST.txt");
    v_datiOvest = readDataFromFile("datiOVEST.txt");

    for (unsigned int i = 0; i < v_dati.size(); i++)
    {
        for (unsigned int j = 0; j < v_dati[i].size(); j++)
        {
            std::cout << v_dati[i][j] << " ";
        }
        std::cout << std::endl;
    }


    return 0;
}