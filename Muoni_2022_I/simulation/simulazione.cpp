#include <cstdlib>
#include <iostream>
#include <cmath>
#include <chrono>
#include <ctime>

#include "TGraph2D.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TPad.h"
#include "TStyle.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "controlli.h"
#include "muon_class.h"


#define L 100 /*distanza tra i 2 rivelatori in cm*/
#define N 10e8 /*tentativi*/
#define R 80
#define larghezza 80.0 //in cm
#define profondita 30.0 //in cm



int main (int argc, char ** argv){

    auto start = std::chrono::system_clock::now();
    TApplication theApp("theApp", &argc, argv);
    srand(time(NULL)); 
    double path_lenght=0;

    TGraph2D g2d;
    TH1F h_path ("h", "Simulazione spazio percorso muoni", sqrt(N), 99, 110);
    TH1F hx_posizione ("hx", "distribuzione x", 1000, -40, 40);
    TH1F hy_posizione ("hy", "distribuzione y", 1000, -30, 30);
    TH2F h2_positions("Distribuzione sul secondo rivelatore", "Distribuzione sul secondo rivelatore", 80, -40, 40, 80, -15, 15);

    int j=0;
    while (j < N){ 
        muone muon(rand_phi(0, M_PI), rand_theta(0));
        g2d.SetPoint(j, R*sin(muon.get_phi())*cos(muon.get_theta()), R*sin(muon.get_phi())*sin(muon.get_theta()), R*cos(muon.get_phi()));
        double x0 = rand_range(-larghezza/2, larghezza/2);
        double y0 = rand_range(-profondita/2, profondita/2);
        bool coinc = is_coincidence(L, muon.coeff_x(), muon.coeff_y(), x0, y0);
        if( coinc == true ){
          path_lenght =get_path(L, muon.coeff_x(), muon.coeff_y(), rand_range(-larghezza/2, larghezza/2), rand_range(-profondita/2, profondita/2));
          h_path.Fill(path_lenght);
        //std::cout << x_coinc_coord(L, muon.coeff_x(), x0) << "\t" << y_coinc_coord(L, muon.coeff_y(), y0) << "\n";
          h2_positions.Fill(x_coinc_coord(L, muon.coeff_x(), x0), y_coinc_coord(L, muon.coeff_y(), y0));
          hx_posizione.Fill(x_coinc_coord(L, muon.coeff_x(), x0));
          hy_posizione.Fill(y_coinc_coord(L, muon.coeff_y(), y0));
          if (j%1000000 == 0){
            std::cout << "Svolte/Totale: \t" << j << "/" << N << "\n" ;
        }
          
        j++;
      }
    }
    
    TH1D * projh2X = h2_positions.ProjectionX();
    TH1D * projh2Y = h2_positions.ProjectionY();

    TCanvas c1;
    h_path.SetFillColor (kOrange + 1) ;
    h_path.Draw ();
    c1.Print ("Distanze simulate.pdf", "pdf") ;

    TCanvas c2;
    hx_posizione.SetFillColor(kAzure);
    hx_posizione.Draw();
    c2.Print ("dist x.pdf", "pdf");

    TCanvas c3;
    hy_posizione.Draw();
    hy_posizione.SetFillColor(kViolet);
    c3.Print ("dist y.pdf", "pdf");

    TCanvas c4;
    gStyle->SetPalette(57);
    h2_positions.Draw("COLZ1");
    c4.Print("Distribuzione2D.pdf", "pdf");
    TCanvas c5;
    projh2X->SetFillColor(kAzure);
    projh2X->Draw();
    c5.Print("Distribuzione2Dx.pdf", "pdf");
    TCanvas c6;
    projh2Y->SetFillColor(kViolet);
    projh2Y->Draw();
    c6.Print("Distribuzione2Dy.pdf", "pdf");
    TCanvas c7;
    g2d.Draw("pcol");
    c7.Print("Posizioni iniziali.pdf", "pdf");
    theApp.Run();


    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished computation at " << std::ctime(&end_time)
                 << "elapsed time: " << elapsed_seconds.count() << "s" << '\n';

    return 0;
  }