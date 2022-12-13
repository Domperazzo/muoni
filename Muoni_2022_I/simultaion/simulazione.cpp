#include <cstdlib>
#include <iostream>
#include <cmath>

#include "TH1F.h"
#include "TCanvas.h"
#include "controlli.h"


#define L 100 /*distanza tra i 2 rivelatori in cm*/
#define N 100000 /*tentativi*/


int main (int argc, char ** argv)
  {
    srand(time(NULL)); 
    double a=0,somma=0,sommaquadra=0;
    TCanvas c1 ("c1", "c1", 100, 100, 1000, 1000) ;

    TH1F h ("h", "Simulazione spazio percorso muoni", sqrt(N), H-30, H+30) ;

    int j=0;
    while (j < N){
        if(is_coincidence(L, K_x, K_y, x0, y0)){
          a=get_path();
          h.Fill (a);
          somma=somma+a;
          sommaquadra=sommaquadra+a*a;
          j++;
        }
    }
    
    std::cout << "media: " << somma/N << std::endl ;
    std::cout << "devst: " << sqrt( ( sommaquadra/N-(somma/N)*(somma/N) )*N / (N-1)) << std::endl ;
    
    h.GetXaxis ()->SetTitle ("Dist (cm)") ;
    h.SetFillColor (kOrange + 1) ;
    h.Draw () ;

    c1.Print ("Distanze simulate.png", "png") ;

    return 0 ;
  }