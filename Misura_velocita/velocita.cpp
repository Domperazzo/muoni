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
    
    TH1F distribuzione_tempi ("hist", "Distribuzione dei tempi di volo", sqrt(NEV), 10., 50.); // nome, n bin, min, max


    ifstream dati;
    dati.open("Dati/Dati_tdcadc_97,15cm.txt", ios::in);
    double TDCdata, ADCdata1, ADCdata2;

    double m[] = {0.1221, 0.0008013};
    double q[] = {0.8727, 0.3014};

    while(!dati.eof()){
        dati >> TDCdata;
        dati >> ADCdata1;
        dati >> ADCdata2;
        distribuzione_tempi.Fill(TDCdata*m[0] + q[0]);
    }
    dati.close();

//distribuzione dei tempi di volo per ogni distanza L
    TCanvas c1 ("c1", "", 800, 800) ;

    distribuzione_tempi.SetFillColor (kOrange + 2) ;
    distribuzione_tempi.Draw ();
    c1.Print("Grafici/Distribuzione_tempi_38,2cm.pdf", "pdf");

    return 0;
}