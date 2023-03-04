/*
c++ -o distribuzione_tempi distribuzione_tempi.cpp `root-config --glibs --cflags`
./distribuzione_tempi 9.5cm
*/

#include <iostream>
#include <fstream> 
#include <sstream>
#include <string>
#include <cmath>
#include <vector>

#include "TApplication.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TH1F.h"
#include "TAxis.h"
#include "TFitResult.h"

#define NEV 80000

using namespace std;
int main(int argc, char* argv[]){

//distribuzione dei tempi di volo per ogni distanza L    
    TH1F distribuzione_tempi ("hist", "Distribuzione dei tempi di volo", sqrt(NEV), 1., 500.); // nome, n bin, min, max

    ifstream dati;
	string fileDati = "Dati/Dati_tdcadc_";
    string lunghezza = argv[1];
    string estensione = ".txt";
    dati.open((fileDati+lunghezza+estensione).c_str(), ios::in);
    double TDCdata, ADCdata1, ADCdata2;

    double m[] = {0.1221, 0.0008013};
    double q[] = {0.8727, 0.3014};

    while(!dati.eof()){
        dati >> TDCdata;
        dati >> ADCdata1;
        dati >> ADCdata2;
        distribuzione_tempi.Fill(TDCdata);
    }
    dati.close();

    TCanvas c1 ("c1", "", 800, 800);

    distribuzione_tempi.SetFillColor (kOrange + 2) ;
    distribuzione_tempi.Draw ();
    string fileGrafici = "Grafici/Distribuzione_tempi_";
    string estensionePDF = ".pdf";
    c1.Print((fileGrafici+lunghezza+estensionePDF).c_str(), "pdf");
    
    return 0;
}
