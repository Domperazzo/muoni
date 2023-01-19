/*
  c++ -o mediamo mediamo.cpp `root-config --glibs --cflags`
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>

#include "TF1.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TFitResult.h"
#include "TAxis.h"
#include "TApplication.h"
#include "TStyle.h"


int main(int argc, char* argv[]){

    TApplication theApp("theApp", &argc, argv);
    gStyle->SetOptFit(1112);

    double angolo = 0.;
    TH1F h_conteggi("h_conteggi", "h_conteggi", 50, 0, 1500);

    std::ifstream dati;
    dati.open("SCA_0deg.txt", std::ios::in);
    std::vector<double> v_conteggi;
    while (true){
        double conteggi;
        dati >> conteggi;
        if (dati.eof() == true)
            break;
        h_conteggi.Fill(conteggi);
    }
    dati.close();
    /*
        TCanvas c1;
        h_conteggi.Draw();
        theApp.Run();
    */

    std::ofstream outfile_media("medie_angoliOvest.txt", std::ios::app);
    if (outfile_media.is_open()){
        outfile_media << angolo << "\t" << h_conteggi.GetMean() << "\t" << h_conteggi.GetStdDev() << "\n" << std::endl;
        outfile_media.close();
    }

    return 0;
}