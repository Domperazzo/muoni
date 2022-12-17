
#include <iostream>
#include <fstream> 
#include <sstream>
#include <string>
#include <cmath>
#include <vector>
/*
#include "TApplication.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TAxis.h"
#include "TF1.h"
#include "TFitResult.h"
*/
#define NEV 100

using namespace std;
int main(int argc, char* argv[]){
    ifstream dati;
    dati.open("datiAdcTdc.txt", ios::in);
    double TDCdata, ADCdata1, ADCdata2;
    vector <double> v_tempo_misurato, v_tempo_corretto;
    vector <double> v_energia1, v_energia2;
    double m[] = {0.1221, 0.0008013};
    double q[] = {0.8727, 0.3014};

    for(int i = 0; i<NEV; i++){
        dati >> TDCdata;
        dati >> ADCdata1;
        dati >> ADCdata2;
        v_tempo_misurato.push_back(TDCdata*m[0] + q[0]);
        v_energia1.push_back(ADCdata1);
        v_energia2.push_back(ADCdata2);

    }

    
    dati.close();
    return 0;
}