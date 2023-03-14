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
    TH1F distribuzione_tempi_tagliati_eccesso ("9.5 cm", "  ", sqrt(NEV), 1., 500.); // nome, n bin, min, max
    
    TApplication theApp("theApp", &argc, argv);


    ifstream dati_tagliati_eccesso;
	string fileDati = "Dati/Dati_tdcadc_";
    string lunghezza = argv[1];
    string estensione = ".txt";
    dati_tagliati_eccesso.open((fileDati+lunghezza+estensione).c_str(), ios::in);
    double TDCdata, ADCdata1, ADCdata2;
    double taglioSupADC1, taglioInfADC1, taglioInfADC2, taglioSupADC2;

    double m[] = {0.1221, 0.0008013}; // coefficiente angolare retta di calibrazione TDC
    double q[] = {0.8727, 0.3014};    // intercetta retta di calibrazione TDC


    if(stod(argv[1]) == 9.5){
        taglioInfADC1 = 140; //-->-0.4canali (-->  -0.6)
        taglioSupADC1 = 340;
        taglioInfADC2 = 80;
        taglioSupADC2 = 280;
    }
    if(stod(argv[1]) == 38.2){
        taglioInfADC1 = 100; //-->-0.1canali (--> -0.1)
        taglioSupADC1 = 360;
        taglioInfADC2 = 80;
        taglioSupADC2 = 280;
      }
    if(stod(argv[1]) == 97.15){
        taglioInfADC1 = 80; //-->+0.1canali (--> +0.6)
        taglioSupADC1 = 360;
        taglioInfADC2 = 80;
        taglioSupADC2 = 260;
    }
    if(stod(argv[1]) == 171.5){
        taglioInfADC1 = 80; //--> 40 canali a dx e a sx --> -0.4canali (-->60 canali a dx e a sx-->-2.3 canali)
        taglioSupADC1 = 360;
        taglioInfADC2 = 40;
        taglioSupADC2 = 240;
    }


    while(!dati_tagliati_eccesso.eof()){
        dati_tagliati_eccesso >> TDCdata;
        dati_tagliati_eccesso >> ADCdata1;
        dati_tagliati_eccesso >> ADCdata2;
        if (ADCdata1 >= taglioInfADC1 && ADCdata1 <= taglioSupADC1 && ADCdata2 >= taglioInfADC2 && ADCdata2 <= taglioSupADC2){
            distribuzione_tempi_tagliati_eccesso.Fill(TDCdata);
        } 
    }
    dati_tagliati_eccesso.close();
    

    TCanvas c1 ("c1", "", 800, 800);

    c1.SetLeftMargin(.17);
    c1.SetBottomMargin(.17);
    
    distribuzione_tempi_tagliati_eccesso.GetXaxis()->SetTitle("TDC [canali]");
    distribuzione_tempi_tagliati_eccesso.GetYaxis()->SetTitle("conteggi");
    distribuzione_tempi_tagliati_eccesso.GetXaxis()->SetTitleSize(0.055);
    distribuzione_tempi_tagliati_eccesso.GetYaxis()->SetTitleSize(0.055);
    distribuzione_tempi_tagliati_eccesso.GetXaxis()->SetLabelSize(0.035);
    distribuzione_tempi_tagliati_eccesso.GetYaxis()->SetLabelSize(0.035);


    distribuzione_tempi_tagliati_eccesso.SetFillColor (kOrange + 2) ;
    distribuzione_tempi_tagliati_eccesso.Draw ();
    string fileGrafici = "Grafici/distribuzione_tempi_tagliati_eccesso";
    string estensionePDF = ".pdf";

 //   c1.Print((fileGrafici+lunghezza+estensionePDF).c_str(), "pdf");

//distribuzione dei tempi di volo per ogni distanza L    
    TH1F distribuzione_tempi_tagliati_giusti ("9.5 cm", "  ", sqrt(NEV), 1., 500.); // nome, n bin, min, max

    ifstream dati_tagliati;
    dati_tagliati.open((fileDati+lunghezza+estensione).c_str(), ios::in);


    if(stod(argv[1]) == 9.5){
        taglioInfADC1 = 180;
        taglioSupADC1 = 300;
        taglioInfADC2 = 120;
        taglioSupADC2 = 240;
    }
    if(stod(argv[1]) == 38.2){
        taglioInfADC1 = 140;
        taglioSupADC1 = 320;
        taglioInfADC2 = 120;
        taglioSupADC2 = 240;
      }
    if(stod(argv[1]) == 97.15){
        taglioInfADC1 = 120;
        taglioSupADC1 = 320;
        taglioInfADC2 = 80;
        taglioSupADC2 = 200;
    }
    if(stod(argv[1]) == 171.5){
        taglioInfADC1 = 120;
        taglioSupADC1 = 320;
        taglioInfADC2 = 80;
        taglioSupADC2 = 200;
    }

    while(!dati_tagliati.eof()){

        dati_tagliati >> TDCdata;
        dati_tagliati >> ADCdata1;
        dati_tagliati >> ADCdata2;
        if (ADCdata1 >= taglioInfADC1 && ADCdata1 <= taglioSupADC1 && ADCdata2 >= taglioInfADC2 && ADCdata2 <= taglioSupADC2){
            distribuzione_tempi_tagliati_giusti.Fill(TDCdata);
        }
    }
    dati_tagliati.close();

    TCanvas c2 ("c2", "", 800, 800);

    c2.SetLeftMargin(.17);
    c2.SetBottomMargin(.17);
    
    distribuzione_tempi_tagliati_giusti.GetXaxis()->SetTitle("TDC [canali]");
    distribuzione_tempi_tagliati_giusti.GetYaxis()->SetTitle("conteggi");
    distribuzione_tempi_tagliati_giusti.GetXaxis()->SetTitleSize(0.055);
    distribuzione_tempi_tagliati_giusti.GetYaxis()->SetTitleSize(0.055);
    distribuzione_tempi_tagliati_giusti.GetXaxis()->SetLabelSize(0.035);
    distribuzione_tempi_tagliati_giusti.GetYaxis()->SetLabelSize(0.035);



    distribuzione_tempi_tagliati_giusti.SetFillColor (kOrange + 2) ;
    distribuzione_tempi_tagliati_giusti.Draw ();
    string fileGraficiTagliati = "Grafici/Distribuzione_tempi_tagliati_";
    c2.Print((fileGraficiTagliati+lunghezza+estensionePDF).c_str(), "pdf");
    theApp.Run();
    return 0;
}
