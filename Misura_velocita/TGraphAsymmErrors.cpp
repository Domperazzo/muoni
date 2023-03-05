

#include <iostream>
#include <fstream> 
#include <sstream>
#include <string>
#include <cmath>
#include <vector>

#include "TApplication.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TF1.h"
#include "TAxis.h"
#include "TFitResult.h"
#include "TStyle.h"
#include "TGraphPainter.h"
#include "TFrame.h"
#include "TGraphAsymmErrors.h"


using namespace std;
	
int main(int argc, char*argv[]){
   TCanvas *c1 = new TCanvas("c1","A Simple Graph with assymetric error bars",200,10,700,500);
   c1->SetFillColor(42);
   c1->SetGrid();
   c1->GetFrame()->SetFillColor(21);
   c1->GetFrame()->SetBorderSize(12);
   Int_t n = 10;
   double x[n]   = {-0.22, 0.05, 0.25, 0.35, 0.5, 0.61,0.7,0.85,0.89,0.95};
   double y[n]   = {1,2.9,5.6,7.4,9,9.6,8.7,6.3,4.5,1};
   double exl[n] = {.05,.1,.07,.07,.04,.05,.06,.07,.08,.05};
   double eyl[n] = {.8,.7,.6,.5,.4,.4,.5,.6,.7,.8};
   double exh[n] = {.02,.08,.05,.05,.03,.03,.04,.05,.06,.03};
   double eyh[n] = {.6,.5,.4,.3,.2,.2,.3,.4,.5,.6};
   TGraphAsymmErrors *gr = new TGraphAsymmErrors(n,x,y,exl,exh,eyl,eyh);
   gr->SetTitle("TGraphAsymmErrors Example");
   gr->SetMarkerColor(4);
   gr->SetMarkerStyle(21);
   gr->Draw("ALP");
   
}
 