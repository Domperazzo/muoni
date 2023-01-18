/*
  c++ -o mediamo mediamo.cpp lib.cc `root-config --glibs --cflags`
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

    double angolo = argv[1];
    TH1F h_conteggi("h_conteggi", "h_conteggi", 50, 0, 100);

    std::ifstream dati;
    dati.open("g1.txt", std::ios::in);
    std::vector<double> v_conteggi;
    while (true){
        double conteggi;
        dati >> conteggi;
        if (dati.eof() == true)
            break;
        //v_conteggi.push_back(conteggi);
        h_conteggi.Fill(conteggi);
    }
    dati.close();

    std::ofstream outfile_media("medie_angoli.txt", std::ios::app);
    if (outfile_media.is_open()){
        outfile_media << angolo << "\t" << h_conteggi.GetMean() << std::endl;
        outfile_media.close();
    }

    return 0;
}