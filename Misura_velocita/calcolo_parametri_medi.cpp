
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <istream>

using namespace std;

int main(int argc, char* argv[]){

    double tau1[4], e_tau1[4], Vs1[4], e_vs1[4];
    double tau2[4], e_tau2[4], Vs2[4], e_vs2[4];
    double tau1_medio[2] ={0, 0}, Vs1_medio[2] = {0, 0};
    double tau2_medio[2] = {0, 0}, Vs2_medio[2] = {0, 0};

    ifstream parametri1;
    parametri1.open("Dati/Parametri_correzione_9.5cm.txt");
    parametri1.ignore(numeric_limits<streamsize>::max(), '\n');
    parametri1 >> tau1[0] >> e_tau1[0] >> tau2[0] >> e_tau2[0] >> Vs1[0] >> e_vs1[0] >> Vs2[0] >> e_vs2[0];
    parametri1.close();



    ifstream parametri2;
    parametri2.open("Dati/Parametri_correzione_38.2cm.txt");
    parametri2.ignore(numeric_limits<streamsize>::max(), '\n');
    parametri2 >> tau1[1] >> e_tau1[1] >> tau2[1] >> e_tau2[1] >> Vs1[1] >> e_vs1[1] >> Vs2[1] >> e_vs2[1];
    parametri2.close();

    ifstream parametri3;
    parametri3.open("Dati/Parametri_correzione_97.15cm.txt");
    parametri3.ignore(numeric_limits<streamsize>::max(), '\n');
    parametri3 >> tau1[2] >> e_tau1[2] >> tau2[2] >> e_tau2[2] >> Vs1[2] >> e_vs1[2] >> Vs2[2] >> e_vs2[2];
    parametri3.close();

    ifstream parametri4;
    parametri4.open("Dati/Parametri_correzione_171.5cm.txt");
    parametri4.ignore(numeric_limits<streamsize>::max(), '\n');
    parametri4 >> tau1[3] >> e_tau1[3] >> tau2[3] >> e_tau2[3] >> Vs1[3] >> e_vs1[3] >> Vs2[3] >> e_vs2[3];
    parametri4.close();
    
    for(int i = 0; i<4; i++){
    	tau1_medio[0] = tau1_medio[0] + tau1[i]/pow(e_tau1[i], 2); 
    	Vs1_medio[0] = Vs1_medio[0] + Vs1[i]/pow(e_vs1[i], 2); 
     	tau2_medio[0] = tau2_medio[0] + tau2[i]/pow(e_tau2[i], 2);
      	Vs2_medio[0] = Vs2_medio[0] + Vs2[i]/pow(e_vs2[i], 2); 

      	
      	tau1_medio[1] = tau1_medio[1] + 1/pow(e_tau1[i], 2); 
    	Vs1_medio[1] = Vs1_medio[1] + 1/pow(e_vs1[i], 2); 
     	tau2_medio[1] = tau2_medio[1] + 1/pow(e_tau2[i], 2);
      	Vs2_medio[1] = Vs2_medio[1] + 1/pow(e_vs2[i], 2); 

    }
    tau1_medio[0] = tau1_medio[0] / tau1_medio[1];
    Vs1_medio[0] = Vs1_medio[0] / Vs1_medio[1]; 
    tau2_medio[0] = tau2_medio[0] / tau2_medio[1];
    Vs2_medio[0] = Vs2_medio[0] / Vs2_medio[1]; 

       
    tau1_medio[1] = 1/sqrt(tau1_medio[1]);
    Vs1_medio[1] = 1/sqrt(Vs1_medio[1]);
    tau2_medio[1] = 1/sqrt(tau2_medio[1]);
    Vs2_medio[1] = 1/sqrt(Vs2_medio[1]);
    
    ofstream OutFile;
    OutFile.open ("Dati/Parametri_medi.txt");
    if (!OutFile) {
        cout << "Errore di apertura del file" << endl; 
        } 
    else {
        OutFile << "tau1 \t" << " e_tau1 \t" << " tau2 \t" << " e_tau2 \t" << " Vs1 \t" << " e_vs1 \t\t" << " Vs2 \t" << " e_vs2 " << endl;
        OutFile << tau1_medio[0] << " " << tau1_medio[1] << " \t " << tau2_medio[0] << " " <<
         tau2_medio[1] << " \t "<< Vs1_medio[0] << " " << Vs1_medio[1] << " \t " << Vs2_medio[0] << " " << Vs2_medio[1] << " \n" << endl; 
    OutFile.close();
    }

    return 0;
}
