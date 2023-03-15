#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>

#include "TH1F.h"
#include "TF1.h"
#include "TMultiGraph.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TFitResult.h"
#include "TGraphErrors.h"
#include "TAxis.h"
#include "TApplication.h"
#include "TStyle.h"
#define USE_MATH_DEFINES


int get_i_max(std::vector<double> vec){ //usato
    int max = 0;
    int i_max = 0;
    for (int i = 0; i < vec.size(); i++){
        if (vec.at(i) > max){
            max = vec.at(i);
            i_max = i;
        }
    }
    return i_max;
}

//  DIFFERENZE (vettori)

std::vector<double> get_diff(std::vector<double> v1, std::vector<double> v2){ //usato
    std::vector<double> diff;
    for (int i = 0; i < v2.size(); i++)
    {
        diff.push_back(v2.at(i) - v1.at(i));
    }
    for (int i = 0; i < diff.size(); i++)
    {
        if (diff.at(i) < 0)
        {
            diff.at(i) = -diff.at(i);
        }
    }
    return diff;
}

std::vector<double> get_diff_err(std::vector<double> v1_err, std::vector<double> v2_err){ //usato
    std::vector<double> diff_err;
    for (int i = 0; i < v2_err.size(); i++)
    {
        diff_err.push_back(  sqrt( pow(v1_err.at(i), 2) + pow(v2_err.at(i), 2) )  );
    }
    return diff_err;
}


//  PERCENTUALI

double get_perc(int i, std::vector<double> v_diff, std::vector<double> v_est){ //calcola la percentuale di asimmetria

    double perc = v_diff.at(i)*100/v_est.at(i);
    return perc;
}

double get_err_perc(int i, std::vector<double> v_diff, std::vector<double> v_diff_err, std::vector<double> v_est, std::vector<double> v_est_err){

    double err_perc = (get_perc(i, v_diff, v_est))*(  v_diff_err.at(i)/v_diff.at(i)  +   v_est_err.at(i)/v_est.at(i)  );
    return err_perc;
}


std::vector<double> get_v_perc(std::vector<double> v_meanEst, std::vector<double> v_meanOvest){
    std::vector<double> v_perc;
    for (int i = 0; i < v_meanEst.size(); i++){
        v_perc.push_back(get_perc(i, get_diff(v_meanEst, v_meanOvest), v_meanEst));
    }
    return v_perc;
}

std::vector<double> get_v_err_perc(std::vector<double> perc, std::vector<double> v_diff, std::vector<double> v_diff_err, std::vector<double> v_est, std::vector<double> v_est_err){
    std::vector<double> v_errore_perc;
    for(int i = 0; i<perc.size(); i++){
        v_errore_perc.push_back( get_err_perc(i, v_diff, v_diff_err, v_est, v_est_err) ); //errore sulla divisione di due numeri con errore
    }

    return v_errore_perc;
}

//  ANALISI DIFFERENZE
void analisi_differenze(std::vector<double> v1, std::vector<double> v1_err , std::vector<double> v2, std::vector<double> v2_err, std::vector<double> v3){ //usato
    std::vector<double> diff;
    for (int i = 0; i < v2.size(); i++){
        diff.push_back(v2.at(i)-v1.at(i));
    }
    for (int i = 0; i < diff.size(); i++){
        if (diff.at(i) < 0){
            diff.at(i) = -diff.at(i);
        }
    }
    int i_max = get_i_max(diff);

    std::cout << "La distanza massima si trova a " << v3.at(i_max) << " gradi e la differenza è di " << diff.at(i_max) << " in percentuale: " << get_perc(i_max, get_diff(v1, v2), v1) <<" +- " << get_err_perc(i_max, get_diff(v1, v2), get_diff_err(v1_err, v2_err), v1, v1_err) << "%" << "\n";
}

//  MEDIE PERCENTUALI PESATE

double get_mean_perc(std::vector<double> perc, std::vector<double> v_diff, std::vector<double> v_diff_err, std::vector<double> v_est, std::vector<double> v_est_err){ //usato
    double temp;
    double sommapesi = 0, sigma;
    for (int i = 0; i < perc.size(); i++){
        sigma = get_v_err_perc(perc, v_diff, v_diff_err, v_est, v_est_err).at(i);
        sommapesi += 1/pow(sigma, 2);
        temp += perc.at(i)/pow(sigma, 2);
        
    }
    return temp/sommapesi;
}

double get_err_mean_perc(std::vector<double> perc, std::vector<double> v_diff, std::vector<double> v_diff_err, std::vector<double> v_est, std::vector<double> v_est_err){
    double sommapesi = 0, sigma;
    for (int i = 0; i < perc.size(); i++){
        sigma = get_v_err_perc(perc, v_diff, v_diff_err, v_est, v_est_err).at(i);
        sommapesi += 1/pow(sigma, 2);
    }
    return 1/sqrt(sommapesi);
}

//   PER FIT
double propagazione_errori( double x, double y, double sigma_x, double sigma_y) //usato
{
	return sqrt( pow(x*sigma_y, 2) + pow(y*sigma_x , 2) )/pow(y,2);
}



int main(int argc, char **argv){
    gStyle->SetOptFit(1112);
    TApplication theApp("theApp", &argc, argv);

    std::ifstream datiEst;
    datiEst.open("Dati/medie_angoliEst.txt", std::ios::in);
    std::vector<double> v_angoliEst, v_meanEst, v_sigmaEst;
    
    double p01, p11, p21, a01, c01, e01;
    datiEst >> a01 >> c01 >> e01;

    do
    {
        datiEst >> p01 >> p11 >> p21;
        v_angoliEst.push_back(p01);
        v_meanEst.push_back(p11);
        v_sigmaEst.push_back(p21);
        std::cout<<"p01: "<<p01<<std::endl;
        std::cout<<"p11: "<< p11<<std::endl;
        if (datiEst.eof() == true)
            break;
        
    }while( datiEst.is_open() == true);
    datiEst.close();

    std::ifstream datiOvest;
    datiOvest.open("Dati/medie_angoliOvest.txt", std::ios::in);
    std::vector<double> v_angoliOvest, v_meanOvest, v_sigmaOvest;
    
    double p02, p12, p22, a02, c02, e02;
    datiOvest >> a02 >> c02 >> e02;

    do
    {
        datiOvest >> p02 >> p12 >> p22;
        v_angoliOvest.push_back(p02);
        v_meanOvest.push_back(p12);
        v_sigmaOvest.push_back(p22);
        std::cout<<"p02: "<<p02<<std::endl;
        std::cout<<"p12: "<< p12<<std::endl;
        if (datiOvest.eof() == true)
            break;
        
    }while( datiOvest.is_open() == true);
    datiOvest.close();
    
    analisi_differenze(v_meanEst, v_sigmaEst,v_meanOvest, v_sigmaOvest, v_angoliEst);

    std::vector<double> v_perc = get_v_perc(v_meanEst, v_meanOvest);

    std::cout << "La percentuale media(pesata) è: " << get_mean_perc(v_perc, get_diff(v_meanEst,v_meanOvest), get_diff_err(v_sigmaEst, v_sigmaOvest), v_meanEst , v_sigmaEst) << " e l'errore sulla percentuale è: " << get_err_mean_perc(v_perc, get_diff(v_meanEst,v_meanOvest), get_diff_err(v_sigmaEst, v_sigmaOvest), v_meanEst , v_sigmaEst) << std::endl;
//doppio fit con distribuzione cos^2
    v_angoliEst.push_back(a01);
    v_meanEst.push_back(c01);
    v_sigmaEst.push_back(e01);

    v_angoliOvest.push_back(a02);
    v_meanOvest.push_back(c02);
    v_sigmaOvest.push_back(e02);

    
    for(int i = 0; i<v_meanEst.size(); i++){
        std::cout<<v_meanEst.at(i)<<std::endl;
        std::cout<<v_sigmaEst.at(i)<<std::endl;
        std::cout<<v_meanOvest.at(i)<<std::endl;
        std::cout<<v_sigmaOvest.at(i)<<std::endl;

    }


    std::cout<<v_meanEst.size()<<std::endl; //stampa i conteggi da 20 a 80
    std::cout<<v_meanOvest.size()<<std::endl; //stampa i conteggi da 20 a 80

    double normalizzazione = v_meanEst[0];
    double n_sigma = v_sigmaEst[0];

    TGraphErrors * punti_E = new TGraphErrors();
    TGraphErrors * punti_O = new TGraphErrors();
    for (int i = 0; i < v_angoliEst.size(); i++){
        punti_E->SetPoint(i, v_angoliEst[i], v_meanEst[i] / normalizzazione);
        punti_E->SetPointError(i, 2., propagazione_errori( v_meanEst[i] , normalizzazione , v_sigmaEst[i], n_sigma ) );
    }

    for (int i = 0; i < v_angoliOvest.size(); i++){
        punti_O->SetPoint(i, -v_angoliOvest[i], v_meanOvest[i]/normalizzazione);
        punti_O->SetPointError(i, 2., propagazione_errori(v_meanOvest[i] , normalizzazione , v_sigmaOvest[i], n_sigma));
    }
    
    punti_E->SetMarkerStyle(20);
    punti_E->SetMarkerSize(0.5);
    punti_E->SetMarkerColor(2);
    punti_E->SetTitle("Est");

    
    punti_O->SetMarkerStyle(20);
    punti_O->SetMarkerSize(0.5);
    punti_O->SetMarkerColor(4);
    punti_O->SetTitle("Ovest");

    TF1 *modelloOvest = new TF1("modelloOvest", "[0]*cos([1]*(3.1415/180)*x)^2", -90., 0.); //[1] ha le dimensioni di radianti/grado
    modelloOvest->SetParName(0, "Ovest0");
    modelloOvest->SetParName(1, "Ovest1");
    modelloOvest->SetParameter(0, 1.);
    modelloOvest->SetLineColor(kGreen+3);
   // modelloOvest->FixParameter(1, 3.14/180);
    TF1 * modelloEst = new TF1 ("modelloEst", "[0]*cos([1]*(3.1415/180)*x)^2", 0. , 90.);
    modelloEst->SetParName(0, "Est0");
    modelloEst->SetParName(1, "Est1");
    modelloEst->SetParameter(0, 1.);
    modelloEst->SetLineColor(kMagenta + 3);
    //  modelloEst->FixParameter(1, 3.14/180);
    
    punti_O->Fit(modelloOvest, "Q", "", -90, 0.);
    punti_E->Fit(modelloEst, "Q", "", 0., 90.);
    
    TMultiGraph * multi = new TMultiGraph;
    multi->Add(punti_O);
    multi->Add(punti_E);
    multi->SetTitle(" ; Angolo zenitale #vartheta [gradi]; Conteggi normalizzati");
    multi->GetHistogram()->GetXaxis()->SetTitleSize(0.05);
    multi->GetHistogram()->GetYaxis()->SetTitleSize(0.05);
    multi->GetHistogram()->GetXaxis()->SetLabelSize(0.05);
    multi->GetHistogram()->GetYaxis()->SetLabelSize(0.05);

    TCanvas * c1 = new TCanvas;
    c1->SetLeftMargin(.2);
    c1->SetBottomMargin(.2);
    multi->Draw("AP");
    c1->BuildLegend();
    
    theApp.Run();

   
   return 0;
   
}