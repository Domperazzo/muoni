#include <cstdlib>
#include <iostream>
#include <cmath>
#include <fstream>
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


#define L 1 //distanza tra i 2 rivelatori in cm
#define N 10e7 //tentativi
#define R 80
#define larghezza 80.0 //in cm
#define profondita 30.0 //in cm



int main (int argc, char ** argv){

    auto start = std::chrono::system_clock::now();
    TApplication theApp("theApp", &argc, argv);
    srand(time(NULL)); 
    double path_lenght=0;

    TH1F h_path ("h", "Simulazione spazio percorso muoni", sqrt(N), L-1, L+10);
    TH2F h2_positions("Distribuzione sul secondo rivelatore", "Distribuzione sul secondo rivelatore", 80, -40, 40, 80, -15, 15);

    int j=0;
    int conteggi = 0;
    while (j < N){
      muone muon(rand_theta(f_cos2, -M_PI/2, 0, 4/M_PI), rand_phi(0, 2*M_PI));
      double x0 = rand_range(-larghezza / 2, larghezza / 2);
      double y0 = rand_range(-profondita / 2, profondita / 2);
      if (is_coincidence(L, muon.coeff_x(), muon.coeff_y(), x0, y0) == true)
      {
        path_lenght = get_path(L, muon.coeff_x(), muon.coeff_y(), rand_range(-larghezza / 2, larghezza / 2), rand_range(-profondita / 2, profondita / 2));
        h_path.Fill(path_lenght);
        h2_positions.Fill(x_coinc_coord(L, muon.coeff_x(), x0), y_coinc_coord(L, muon.coeff_y(), y0));
        if (j % 1000000 == 0)
        {
          std::cout << "Svolte/Totale: \t" << j << "/" << N << "\n";
        }
        j++;
      }
      conteggi++;
    }
    
    std::cout << conteggi << " muoni su " << N << " sono passati da entrambi i rilevatori" << "\n" ;

    std::ofstream outfile_media2("distanze.txt", std::ios::app);
    if (outfile_media2.is_open())
    {
      outfile_media2 << h_path.GetMean(1) << "\t" << h_path.GetStdDev(1) << "\t" << L << std::endl;
      outfile_media2.close();
    }

    TH1D * projh2X = h2_positions.ProjectionX();
    TH1D * projh2Y = h2_positions.ProjectionY();

    gStyle->SetPalette(1);


    TCanvas c1;
    h_path.SetFillColor (kOrange + 1) ;
    h_path.Draw ();
    c1.Print ("Distanze simulate L=1.pdf", "pdf") ;

    TCanvas c2;
    c2.SetRightMargin(.15);
    gStyle->SetPalette(57);
    h2_positions.Draw("COLZ1");
    c2.Print("Distribuzione2D L=1.pdf", "pdf");

    TCanvas c3;
    projh2X->SetFillColor(kAzure);
    projh2X->Draw();
    c3.Print("Distribuzione2Dx.pdf", "pdf");
    TCanvas c4;
    projh2Y->SetFillColor(kViolet);
    projh2Y->Draw();
    c4.Print("Distribuzione2Dy.pdf", "pdf");
    TCanvas c5;
    g2d.SetMarkerStyle(20);
    g2d.Draw("surf2");
    c5.Print("Posizioni iniziali.pdf", "pdf");

    TCanvas c6;
    h_theta.Draw();
    c6.Print()
    TCanvas c7;
    h_phi.Draw();
    c6.Print();
    
    theApp.Run();
    return 0;
  }
