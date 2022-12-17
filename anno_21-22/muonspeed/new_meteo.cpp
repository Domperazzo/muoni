/*
https://api.weather.com/v1/location/LIML:9:IT/observations/historical.json?apiKey=6532d6454b8aa370768e63d6ba5a832e&units=m&language=it-IT&startDate=20201110&endDate=20201114

curl "https://api.weather.com/v1/location/LIML:9:IT/observations/historical.json?apiKey=6532d6454b8aa370768e63d6ba5a832e&units=m&language=it-IT&startDate=20201110&endDate=20201117" > meteo.txt


possibili miglioramenti: invece di creare ogni volta 6 grafici potrei scrivere una classe che contiene di per sè tutto... 
renderebbe molto più breve e leggibile il codice

*/

// c++ -o new_meteo new_meteo.cpp `root-config --cflags --glibs` -lcurl
// ./new_meteo /mnt/c/.../dati.txt

#define strtk_no_tr1_or_boost 1

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <chrono>
#include <ctime>
#include <curl/curl.h>

#include "strtk.hpp"

#include "TApplication.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TVectorD.h"
#include "TH1D.h"
#include "THistPainter.h"
	
#define DOWNLOAD true	
#define START_DATE "20201113"				//per curl
#define END_DATE "20201119"			

#define GIORNO_START 13			
#define ORA_START 13				

#define ALFA -0.15
#define BETA 0.025

using namespace std;

class Data{
	private:
		int day;
		string month;
		int year;
		int ora;
		int min;
		double press;				//pressione
		int temp;
		string tipo;
	public:
		Data(int unix_time){
			time_t raw = unix_time;
			struct tm * timeinfo;
			timeinfo = localtime(&raw);
			string str_temp = asctime(timeinfo);
			month = str_temp.substr(4,3);
			day = stoi(str_temp.substr(8,10));
			year = stoi(str_temp.substr(20,24));
			ora = stoi(str_temp.substr(11,13));
			min = stoi(str_temp.substr(14,16));
			press = 0;
			temp = 0;
		}

		void Print(){
			cout << day << " " << month << " " << year <<  " " << ora << ":" << min << " -> " << temp << "C " << press << " hPa" << endl;
		}

		void Set_press(double n){
			press = n;
			return;
		}
		void Set_temp(int n){
			temp = n;
			return;
		}
		void Set_type(const string & t){
			tipo = t;
			return;
		}

		
		int Get_ora_round(){
			if(min <= 30) return ora;
			else return ora + 1;
		}
		int Get_day(){
			return day;
		}
		string Get_type(){
			return tipo;
		}
		int Get_temp(){
			return temp;
		}
		double Get_press(){
			return press;
		}
};

class Dictionary_val{
	private:
		string name;
		int val;
	public:	
		Dictionary_val(const string & nome, int v):name(nome){
			val = v;
		}
		string Get_name(){
			return name;
		}
		int Get_val(){
			return val;
		}
};

vector<Dictionary_val*> dizionario;
vector<Data*> v_date;

void load_dictionary(){
	string temp;
	int val;

	//valori:
	//
	//	sole 			0
	//	sereno(notte) 	1
	//  Nebbia 			2
	//  nuvoloso 		3
	// 	Pioggia 		4			<--- e tromba marina (??)
	//  Poca Nebbia 	5

	val = 2;
	temp = "Banchi di nebbia";
	dizionario.push_back(new Dictionary_val(temp,val));

	val = 5;
	temp = "Strato sottile di nebbia";
	dizionario.push_back(new Dictionary_val(temp,val));

	val = 5;
	temp = "Foschia";
	dizionario.push_back(new Dictionary_val(temp,val));

	val = 2;
	temp = "Nebbia";
	dizionario.push_back(new Dictionary_val(temp,val));

	val = 1;
	temp = "Discreto (notte)";
	dizionario.push_back(new Dictionary_val(temp,val));

	val = 3;
	temp = "Molto nuvoloso";
	dizionario.push_back(new Dictionary_val(temp,val));

	val = 4;
	temp = "Tromba marina";
	dizionario.push_back(new Dictionary_val(temp,val));

	val = 3;
	temp = "Poco nuvoloso";
	dizionario.push_back(new Dictionary_val(temp,val));

	val = 4;
	temp = "Debole pioviggine";
	dizionario.push_back(new Dictionary_val(temp,val));

	val = 4;
	temp = "Deboli piogge";
	dizionario.push_back(new Dictionary_val(temp,val));

	val = 4;
	temp = "Pioviggine";
	dizionario.push_back(new Dictionary_val(temp,val));

	val = 4;
	temp = "Pioggia";
	dizionario.push_back(new Dictionary_val(temp,val));

	return;
}

int tipe_to_val(const string & meteo){
	for(int i = 0; i < dizionario.size(); i++){
		string temp = dizionario[i] -> Get_name();
		if(temp == meteo) return dizionario[i] -> Get_val();
	}
	cout << "errore in type to val" << endl;
	return -1;
}

int find_nearest(int ora){
	int giorno = (int) (ora / 24) + GIORNO_START;
	int ora_att = ora % 24;
	for (int i = 0; i < v_date.size(); i++){
		if(giorno == v_date[i] -> Get_day() && ora_att == v_date[i] -> Get_ora_round()) return i;
	}
	cout << "errore in find_nearest per data: " << giorno << " - " << ora_att << endl;
	return 0;
}

bool leggi_dati(vector<double> & conteggiS1, vector<double> & conteggiS2, vector<double> & doppie, int argc, char** argv){
	if(argc < 2){
		cout << "Digita il nome del file di dati" << endl;
		return false;
	}
	double scal1, scal2, scal3, scal4, scal5, scal6, scal7, scal8, scal9, scal10, scal11, scal12;
	ifstream Infile;
	Infile.open(argv[1], fstream::in);
	if (Infile.good () == false){
		cout << "Errore nell'apertura dei file" << endl;
		return false;
	}
	cout << "Leggo file di dati " << argv[1] << endl;
	while(true){
		Infile >> scal1 >> scal2 >> scal3 >> scal4 >> scal5 >> scal6 >> scal7 >> scal8 >> scal9 >> scal10 >> scal11 >> scal12;
		
		conteggiS1.push_back(scal1);
		conteggiS2.push_back(scal2);
		doppie.push_back(scal5);

		if (Infile.eof() == true) break;
	}
	Infile.close();

	cout << endl;
	cout << "Letti " << conteggiS1.size() << " conteggiS1" << endl;
	cout << "Letti " << conteggiS2.size() << " conteggiS2" << endl;
	cout << "Lette " << doppie.size() << " doppie" << endl;
	cout << endl;

	return true;
}

bool read_meteo_data(){
	vector<int> tempi;
	vector<string> tipi;
	vector<int> temperature;
	vector<double> pressioni;


	vector<string> lettura;
	//stringe di riconoscimento
	string rec_time = "valid_time_gmt"; 
	string rec_type = "\"wx_phrase\"";
	string rec_temp = "\"temp\"";
	string rec_pres = "\"pressure\"";


	string line;
	ifstream myfile ("meteo.txt");
	if(myfile.is_open()){
		getline(myfile, line);
		strtk::parse(line, "," ,lettura);
		myfile.close();
	}
	else return false;

	for(int i = 0; i < lettura.size(); i++){
		int pos = 0;
		string token;
		if (lettura[i].find(rec_time) != string::npos){
				pos = 17;	//14 + 3
				token = lettura[i].substr(pos, lettura[i].length());
			//	cout << "time : " << stoi(token) << endl;
				tempi.push_back(stoi(token));
			}
		else if (lettura[i].find(rec_type) != string::npos){
				pos = 13;	// 11 +2
				token = lettura[i].substr(pos, lettura[i].length());
				pos = token.find("\"");
				token = token.substr(0,pos);
			//	cout << "tipi : " << token << endl;
				tipi.push_back(token);
			}
		else if (lettura[i].find(rec_temp) != string::npos){
				pos = 7;	//6 + 1
				token = lettura[i].substr(pos, lettura[i].length());
			//	cout << "temp : " << stoi(token) << endl;
				temperature.push_back(stoi(token));
			}
		else if (lettura[i].find(rec_pres) != string::npos){
				pos = 11;	//10 + 1
				token = lettura[i].substr(pos, lettura[i].length());
			//	cout << "press : " << stod(token) << endl;
				pressioni.push_back(stod(token));
			}


	}


	for(int i = 0; i < tempi.size(); i++){
		int tempo = tempi[i];
		string tipo = tipi[i];
		int temperatura = temperature[i];
		double pressione = pressioni[i];

		Data * temp  = new Data(tempo);
		temp -> Set_type(tipo);
		temp -> Set_temp(temperatura);
		temp -> Set_press(pressione);


		v_date.push_back(temp);
	}

	return true;
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream){
	size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  	return written;
}

void downloadMeteo(){
		CURL *curl_handle;
	  	static const char *pagefilename = "meteo.txt";
	  	FILE *pagefile;

	  	curl_global_init(CURL_GLOBAL_ALL);
	 
	  	curl_handle = curl_easy_init();
	 

	  	string request = "https://api.weather.com/v1/location/LIML:9:IT/observations/historical.json?apiKey=6532d6454b8aa370768e63d6ba5a832e&units=m&language=it-IT&startDate=";
	  	request.append(START_DATE);
	  	request.append("&endDate=");
	  	request.append(END_DATE);

	  	curl_easy_setopt(curl_handle, CURLOPT_URL, &request[0]);
	  	curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
	  	curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
	  	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
	 
	  	pagefile = fopen(pagefilename, "wb");
	  	if(pagefile) {
	 
	    	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);
	 
	    	curl_easy_perform(curl_handle);
	 
	    	fclose(pagefile);
	  	}
	 	
	  	curl_easy_cleanup(curl_handle);
	 
	  	curl_global_cleanup();	
	  	cout << "Download completato\n" << endl;
}

bool checkDictionaryIsComplete(){
	for(int i = 0; i < v_date.size(); i++){
		string x = v_date[i] -> Get_type();
		bool presente = false;

		for(int j = 0; j < dizionario.size(); j++){
			string dict_type = dizionario[j] -> Get_name();
			if(dict_type == x) presente = true;
		}

		if(presente == false){
			cout << "Nel dizionario non ho trovato: -" << x << "-" << endl;
			return false;
		}
	}
	cout << "Dizionario completo \n" << endl;
	return true;
}

TGraph * createSimpleTGraph(){
	TGraph* graph = new TGraph;                   
	gr_std1 -> SetMarkerSize(1);
	gr_std1 -> SetMarkerStyle(8);
	return graph
}

int main(int argc, char *argv[]){

	if(DOWNLOAD == true)
		downloadMeteo();
	
	if(read_meteo_data() == false){
		cout << "Lettura fallita" << endl;
		return 1;
	} 
	else cout << "Lettura completata con successo" << endl;;

	load_dictionary();
	if(checkDictionaryIsComplete() == false)
		return 1;

	//-------------------------------
	//-------------------------------

	TApplication myApp("myApp",NULL,NULL);
	gStyle -> SetOptFit(1111);

	//vettori per le variabili da memorizzare
	vector<double> conteggiS1, conteggiS2, doppie;

	//lettura del file di dati
	if( !(leggi_dati(conteggiS1, conteggiS2, doppie, argc, argv)) ) return 1;
	cout << "Lettura completata con successo\n" << endl;

//andamento meteo per S1

	TGraph* gr_std1 = createSimpleTGraph();                   
	gr_std1 -> SetMarkerColor(kBlack);

	TGraph* gr_0 = createSimpleTGraph(); 	//sole
	gr_0 -> SetMarkerColor(kYellow);

	TGraph* gr_1 = createSimpleTGraph();	//sereno(notte)             <-------------
	gr_1 -> SetMarkerColor(kBlue + 4);

	TGraph* gr_2 = createSimpleTGraph();		//nebbia
	gr_2 -> SetMarkerColor(kGray + 1);

	TGraph* gr_3 = createSimpleTGraph();		//nuvoloso
	gr_3 -> SetMarkerColor(kGray + 2);

	TGraph* gr_4 = createSimpleTGraph();		//pioggia
	gr_4 -> SetMarkerColor(kCyan - 3);

	TGraph* gr_5 = createSimpleTGraph();		//poca nebbia
	gr_5 -> SetMarkerColor(kGray);
	
	//---------------------------------------------

	TGraph* gr_pressione = createSimpleTGraph();
	gr_pressione -> SetMarkerColor(kBlack);

	TGraph* gr_temperatura = createSimpleTGraph();
	gr_temperatura -> SetMarkerColor(kBlack);

	//---------------------------------------------

	for(int i = 0; i < conteggiS1.size(); i++){
		int j = find_nearest(i + ORA_START); //trova l'indice che corrisponde all'ora più vicina

		string meteo = v_date[j] -> Get_type();

		int caso = tipe_to_val(meteo); //cerca nel dizionario a quale valore corrisponde

		double y;
		if(i > 0) y = (conteggiS1[i] - conteggiS1[i-1]) / 60;
		else y = conteggiS1[i] / 60;
		int N;

		N = gr_std1 -> GetN();
		gr_std1 -> SetPoint(N,i,y);

		//------------------------------

		int temp = v_date[j] -> Get_temp();
		double press = v_date[j] -> Get_press();

		gr_temperatura -> SetPoint(N,i,temp);
		gr_pressione -> SetPoint(N,i,press);

		//------------------------------

		switch(caso){
			case 0: 
				N = gr_0 -> GetN();
				gr_0 -> SetPoint(N,i,y);
				break;
			case 1: 
				N = gr_1 -> GetN();
				gr_1 -> SetPoint(N,i,y);
				break;
			case 2: 
				N = gr_2 -> GetN();
				gr_2 -> SetPoint(N,i,y);
				break;
			case 3: 
				N = gr_3 -> GetN();
				gr_3 -> SetPoint(N,i,y);
				break;
			case 4: 
				N = gr_4 -> GetN();
				gr_4 -> SetPoint(N,i,y);
				break;
			case 5: 
				N = gr_5 -> GetN();
				gr_5 -> SetPoint(N,i,y);
				break;
			default:
				cout << "\nNon riesco a inserire un elemento per S1" << endl;
				break;
		}
	}

	TCanvas* meteo_conteggis1 = new TCanvas("meteo_conteggis1","meteo_conteggis1",0,0,700,500);
	meteo_conteggis1->cd();


	
	gr_2->Draw("AP");		//grafico della nebbia, ahah

	if(gr_0 -> GetN() != 0) gr_0 -> Draw("SAME AP");
	if(gr_1 -> GetN() != 0) gr_1 -> Draw("SAME P");
	if(gr_3 -> GetN() != 0) gr_3 -> Draw("SAME P");
	if(gr_4 -> GetN() != 0) gr_4 -> Draw("SAME P");
	if(gr_5 -> GetN() != 0) gr_5 -> Draw("SAME P");


	gr_0 -> GetXaxis() -> SetLimits(0, conteggiS1.size() + 10);
	gr_0 -> GetXaxis() -> SetRangeUser(0, conteggiS1.size() + 10);

	meteo_conteggis1 -> Modified();
	meteo_conteggis1 -> Update();

	cout << endl;
	cout << "Andamento per s1:\n"
		<< "Assolato        " 	<< "\tmedia = " << gr_0 -> GetMean(2) << "\tconteggi = " << gr_0 -> GetN() << "\n"
		<< "Sereno (notte)  " 	<< "\tmedia = " << gr_1 -> GetMean(2) << "\tconteggi = " << gr_1 -> GetN() << "\n"
		<< "Banchi di nebbia" 	<< "\tmedia = " << gr_2 -> GetMean(2) << "\tconteggi = " << gr_2 -> GetN() << "\n"
		<< "Nuvoloso        " 	<< "\tmedia = " << gr_3 -> GetMean(2) << "\tconteggi = " << gr_3 -> GetN() << "\n"
		<< "Pioggie sparse  " 	<< "\tmedia = " << gr_4 -> GetMean(2) << "\tconteggi = " << gr_4 -> GetN() << "\n"
		<< "Leggera foschia " 	<< "\tmedia = " << gr_5 -> GetMean(2) << "\tconteggi = " << gr_5 -> GetN() << "\n"
		<< endl;


	//rate vs pressione s1

	TCanvas* conteggi_vs_pressione_s1 = new TCanvas("conteggi_vs_pressione_s1","conteggi_vs_pressione_s1",0,0,700,500);
	conteggi_vs_pressione_s1 -> Divide(1,2);

	conteggi_vs_pressione_s1 -> cd(1);
	gr_std1 -> Draw("AP");

	conteggi_vs_pressione_s1 -> cd(2);
	gr_pressione -> Draw("AP");


	//rate vs temperatura s1

	TCanvas* conteggi_vs_temperatura_s1 = new TCanvas("conteggi_vs_temperatura_s1","conteggi_vs_temperatura_s1",0,0,700,500);
	conteggi_vs_temperatura_s1 -> Divide(1,2);

	conteggi_vs_temperatura_s1 -> cd(1);
	gr_std1 -> Draw("AP");

	conteggi_vs_temperatura_s1 -> cd(2);
	gr_temperatura -> Draw("AP");


	double mean_temp = gr_temperatura -> GetMean(2);
	double mean_press = gr_pressione -> GetMean(2);
	double mean_rate_s1 = gr_std1 -> GetMean(2);

	cout << "temperatura media: " << mean_temp << endl;
	cout << "pressione media: " << mean_press << endl;
	cout << "rate medio (s1): " << mean_rate_s1 << endl;

	//attesa : I = (I0+ALFA(P-P0) + BETA (T-T0) )*I0
	TCanvas* rate_reale_vs_atteso_s1 = new TCanvas("rate_reale_vs_atteso_s1","rate_reale_vs_atteso_s1",0,0,700,500);
	rate_reale_vs_atteso_s1 -> Divide(1,2);

	TGraph* gr_atteso_s1 = new TGraph;
	gr_atteso_s1 -> SetMarkerColor(kBlack);
	gr_atteso_s1 -> SetMarkerSize(1);
	gr_atteso_s1 -> SetMarkerStyle(8);


	for(int i = 0; i < conteggiS1.size(); i++){
		int j = find_nearest(i + ORA_START); //trova l'indice che corrisponde all'ora più vicina

		double I_0 = mean_rate_s1;
		int N;
		N = gr_atteso_s1 -> GetN();

		int temp = v_date[j] -> Get_temp();
		double press = v_date[j] -> Get_press();

		double y =I_0 + I_0*(ALFA*(mean_press - press) + BETA*(mean_temp - temp));

		gr_atteso_s1 -> SetPoint(N,i,y);
	}

	rate_reale_vs_atteso_s1 -> cd(1);
	gr_std1 -> Draw("AP");

	rate_reale_vs_atteso_s1 -> cd(2);
	gr_atteso_s1 -> Draw("AP");

//andamento meteo per S2

	TGraph* gr_std2= new TGraph;                   
	gr_std2 -> SetMarkerColor(kBlack);
	gr_std2 -> SetMarkerSize(1);
	gr_std2 -> SetMarkerStyle(8);

	TGraph* gr_0_2 = new TGraph;		//sole
	gr_0_2 -> SetMarkerColor(kYellow);
	gr_0_2 -> SetMarkerSize(1);
	gr_0_2 -> SetMarkerStyle(8);

	TGraph* gr_1_2 = new TGraph;		//sereno(notte)             <-------------
	gr_1_2 -> SetMarkerColor(kBlue + 4);
	gr_1_2 -> SetMarkerSize(1);
	gr_1_2 -> SetMarkerStyle(8);

	TGraph* gr_2_2 = new TGraph;		//nebbia
	gr_2_2 -> SetMarkerColor(kGray + 1);
	gr_2_2 -> SetMarkerSize(1);
	gr_2_2 -> SetMarkerStyle(8);

	TGraph* gr_3_2 = new TGraph;		//nuvoloso
	gr_3_2 -> SetMarkerColor(kGray + 2);
	gr_3_2 -> SetMarkerSize(1);
	gr_3_2 -> SetMarkerStyle(8);

	TGraph* gr_4_2 = new TGraph;		//pioggia
	gr_4_2 -> SetMarkerColor(kCyan - 3);
	gr_4_2 -> SetMarkerSize(1);
	gr_4_2 -> SetMarkerStyle(8);

	TGraph* gr_5_2 = new TGraph;		//poca nebbia
	gr_5_2 -> SetMarkerColor(kGray);
	gr_5_2 -> SetMarkerSize(1);
	gr_5_2 -> SetMarkerStyle(8);
	

	for(int i = 0; i < conteggiS2.size(); i++){
		int j = find_nearest(i + ORA_START); //trova l'indice che corrisponde all'ora più vicina

		string meteo = v_date[j] -> Get_type();

		int caso = tipe_to_val(meteo); //cerca nel dizionario a quale valore corrisponde

		double y;
		if(i > 0) y = (conteggiS2[i] - conteggiS2[i-1]) / 60;
		else y = conteggiS2[i] / 60;
		int N;

		N = gr_std2 -> GetN();
		gr_std2 -> SetPoint(N,i,y);

		switch(caso){
			case 0: 
				N = gr_0_2 -> GetN();
				gr_0_2 -> SetPoint(N,i,y);
				break;
			case 1: 
				N = gr_1_2 -> GetN();
				gr_1_2 -> SetPoint(N,i,y);
				break;
			case 2: 
				N = gr_2_2 -> GetN();
				gr_2_2 -> SetPoint(N,i,y);
				break;
			case 3: 
				N = gr_3_2 -> GetN();
				gr_3_2 -> SetPoint(N,i,y);
				break;
			case 4: 
				N = gr_4_2 -> GetN();
				gr_4_2 -> SetPoint(N,i,y);
				break;
			case 5: 
				N = gr_5_2 -> GetN();
				gr_5_2 -> SetPoint(N,i,y);
				break;
			default:
				cout << "\nNon riesco a inserire un elemento per S2" << endl;
				break;
		}
	}

	TCanvas* meteo_conteggis2 = new TCanvas("meteo_conteggis2","meteo_conteggis2",0,0,700,500);
	meteo_conteggis2->cd();


	
	gr_2_2 -> Draw("AP");		//grafico della nebbia, ahah

	if(gr_0_2 -> GetN() != 0) gr_0_2 -> Draw("SAME AP");
	if(gr_1_2 -> GetN() != 0) gr_1_2 -> Draw("SAME P");
	if(gr_3_2 -> GetN() != 0) gr_3_2 -> Draw("SAME P");
	if(gr_4_2 -> GetN() != 0) gr_4_2 -> Draw("SAME P");
	if(gr_5_2 -> GetN() != 0) gr_5_2 -> Draw("SAME P");


	gr_0_2 -> GetXaxis() -> SetLimits(0, conteggiS1.size() + 10);
	gr_0_2 -> GetXaxis() -> SetRangeUser(0, conteggiS1.size() + 10);

	meteo_conteggis2 -> Modified();
	meteo_conteggis2 -> Update();

	cout << endl;
	cout << "Andamento per s2:\n"
		<< "Assolato        " 	<< "\tmedia = " << gr_0_2 -> GetMean(2) << "\tconteggi = " << gr_0_2 -> GetN() << "\n"
		<< "Sereno (notte)  " 	<< "\tmedia = " << gr_1_2 -> GetMean(2) << "\tconteggi = " << gr_1_2 -> GetN() << "\n"
		<< "Banchi di nebbia" 	<< "\tmedia = " << gr_2_2 -> GetMean(2) << "\tconteggi = " << gr_2_2 -> GetN() << "\n"
		<< "Nuvoloso        " 	<< "\tmedia = " << gr_3_2 -> GetMean(2) << "\tconteggi = " << gr_3_2 -> GetN() << "\n"
		<< "Pioggie sparse  " 	<< "\tmedia = " << gr_4_2 -> GetMean(2) << "\tconteggi = " << gr_4_2 -> GetN() << "\n"
		<< "Leggera foschia " 	<< "\tmedia = " << gr_5_2 -> GetMean(2) << "\tconteggi = " << gr_5_2 -> GetN() << "\n"
		<< endl;


	//rate vs pressione s2

	TCanvas* conteggi_vs_pressione_s2 = new TCanvas("conteggi_vs_pressione_s2","conteggi_vs_pressione_s2",0,0,700,500);
	conteggi_vs_pressione_s2 -> Divide(1,2);

	conteggi_vs_pressione_s2 -> cd(1);
	gr_std2 -> Draw("AP");

	conteggi_vs_pressione_s2 -> cd(2);
	gr_pressione -> Draw("AP");


	//rate vs temperatura s2

	TCanvas* conteggi_vs_temperatura_s2 = new TCanvas("conteggi_vs_temperatura_s2","conteggi_vs_temperatura_s2",0,0,700,500);
	conteggi_vs_temperatura_s2 -> Divide(1,2);

	conteggi_vs_temperatura_s2 -> cd(1);
	gr_std2 -> Draw("AP");

	conteggi_vs_temperatura_s2 -> cd(2);
	gr_temperatura -> Draw("AP");

	double mean_rate_s2 = gr_std2 -> GetMean(2);

	cout << "temperatura media: " << mean_temp << endl;
	cout << "pressione media: " << mean_press << endl;
	cout << "rate medio (s2): " << mean_rate_s2 << endl;

	//attesa : I = (I0+ALFA(P-P0) + BETA (T-T0) )*I0
	TCanvas* rate_reale_vs_atteso_s2 = new TCanvas("rate_reale_vs_atteso_s2","rate_reale_vs_atteso_s2",0,0,700,500);
	rate_reale_vs_atteso_s2 -> Divide(1,2);

	TGraph* gr_atteso_s2 = new TGraph;
	gr_atteso_s2 -> SetMarkerColor(kBlack);
	gr_atteso_s2 -> SetMarkerSize(1);
	gr_atteso_s2 -> SetMarkerStyle(8);


	for(int i = 0; i < conteggiS2.size(); i++){
		int j = find_nearest(i + ORA_START); //trova l'indice che corrisponde all'ora più vicina

		double I_0 = mean_rate_s2;
		int N;
		N = gr_atteso_s2 -> GetN();

		int temp = v_date[j] -> Get_temp();
		double press = v_date[j] -> Get_press();

		double y =I_0 + I_0*(ALFA*(mean_press - press) + BETA*(mean_temp - temp));

		gr_atteso_s2 -> SetPoint(N,i,y);
	}

	rate_reale_vs_atteso_s2 -> cd(1);
	gr_std2 -> Draw("AP");

	rate_reale_vs_atteso_s2 -> cd(2);
	gr_atteso_s2 -> Draw("AP");

//andamento doppie

	TGraph* gr_stdd= new TGraph;                   
	gr_stdd -> SetMarkerColor(kBlack);
	gr_stdd -> SetMarkerSize(1);
	gr_stdd -> SetMarkerStyle(8);

	TGraph* gr_0_d = new TGraph;		//sole
	gr_0_d -> SetMarkerColor(kYellow);
	gr_0_d -> SetMarkerSize(1);
	gr_0_d -> SetMarkerStyle(8);

	TGraph* gr_1_d = new TGraph;		//sereno(notte)             <-------------
	gr_1_d -> SetMarkerColor(kBlue + 4);
	gr_1_d -> SetMarkerSize(1);
	gr_1_d -> SetMarkerStyle(8);

	TGraph* gr_2_d = new TGraph;		//nebbia
	gr_2_d -> SetMarkerColor(kGray + 1);
	gr_2_d -> SetMarkerSize(1);
	gr_2_d -> SetMarkerStyle(8);

	TGraph* gr_3_d = new TGraph;		//nuvoloso
	gr_3_d -> SetMarkerColor(kGray + 2);
	gr_3_d -> SetMarkerSize(1);
	gr_3_d -> SetMarkerStyle(8);

	TGraph* gr_4_d = new TGraph;		//pioggia
	gr_4_d -> SetMarkerColor(kCyan - 3);
	gr_4_d -> SetMarkerSize(1);
	gr_4_d -> SetMarkerStyle(8);

	TGraph* gr_5_d = new TGraph;		//poca nebbia
	gr_5_d -> SetMarkerColor(kGray);
	gr_5_d -> SetMarkerSize(1);
	gr_5_d -> SetMarkerStyle(8);
	

	for(int i = 0; i < doppie.size(); i++){
		int j = find_nearest(i + ORA_START); //trova l'indice che corrisponde all'ora più vicina

		string meteo = v_date[j] -> Get_type();

		int caso = tipe_to_val(meteo); //cerca nel dizionario a quale valore corrisponde

		double y;
		if(i > 0) y = (doppie[i] - doppie[i-1]) / 60;
		else y = doppie[i] / 60;
		int N;

		N = gr_stdd -> GetN();
		gr_stdd -> SetPoint(N,i,y);

		switch(caso){
			case 0: 
				N = gr_0_d -> GetN();
				gr_0_d -> SetPoint(N,i,y);
				break;
			case 1: 
				N = gr_1_d -> GetN();
				gr_1_d -> SetPoint(N,i,y);
				break;
			case 2: 
				N = gr_2_d -> GetN();
				gr_2_d -> SetPoint(N,i,y);
				break;
			case 3: 
				N = gr_3_d -> GetN();
				gr_3_d -> SetPoint(N,i,y);
				break;
			case 4: 
				N = gr_4_d -> GetN();
				gr_4_d -> SetPoint(N,i,y);
				break;
			case 5: 
				N = gr_5_d -> GetN();
				gr_5_d -> SetPoint(N,i,y);
				break;
			default:
				cout << "\nNon riesco a inserire un elemento per le doppie" << endl;
				break;
		}
	}

	TCanvas* meteo_conteggisd = new TCanvas("meteo_conteggisd","meteo_conteggisd",0,0,700,500);
	meteo_conteggisd->cd();


	
	gr_2_d -> Draw("AP");		//grafico della nebbia, ahah

	if(gr_0_d -> GetN() != 0) gr_0_d -> Draw("SAME AP");
	if(gr_1_d -> GetN() != 0) gr_1_d -> Draw("SAME P");
	if(gr_3_d -> GetN() != 0) gr_3_d -> Draw("SAME P");
	if(gr_4_d -> GetN() != 0) gr_4_d -> Draw("SAME P");
	if(gr_5_d -> GetN() != 0) gr_5_d -> Draw("SAME P");


	gr_0_d -> GetXaxis() -> SetLimits(0, doppie.size() + 10);
	gr_0_d -> GetXaxis() -> SetRangeUser(0, doppie.size() + 10);

	meteo_conteggisd -> Modified();
	meteo_conteggisd -> Update();

	cout << endl;
	cout << "Andamento per le doppie:\n"
		<< "Assolato        " 	<< "\tmedia = " << gr_0_d -> GetMean(2) << "\tconteggi = " << gr_0_d -> GetN() << "\n"
		<< "Sereno (notte)  " 	<< "\tmedia = " << gr_1_d -> GetMean(2) << "\tconteggi = " << gr_1_d -> GetN() << "\n"
		<< "Banchi di nebbia" 	<< "\tmedia = " << gr_2_d -> GetMean(2) << "\tconteggi = " << gr_2_d -> GetN() << "\n"
		<< "Nuvoloso        " 	<< "\tmedia = " << gr_3_d -> GetMean(2) << "\tconteggi = " << gr_3_d -> GetN() << "\n"
		<< "Pioggie sparse  " 	<< "\tmedia = " << gr_4_d -> GetMean(2) << "\tconteggi = " << gr_4_d -> GetN() << "\n"
		<< "Leggera foschia " 	<< "\tmedia = " << gr_5_d -> GetMean(2) << "\tconteggi = " << gr_5_d -> GetN() << "\n"
		<< endl;


	//rate vs pressione doppie

	TCanvas* conteggi_vs_pressione_sd = new TCanvas("conteggi_vs_pressione_sd","conteggi_vs_pressione_sd",0,0,700,500);
	conteggi_vs_pressione_sd -> Divide(1,2);

	conteggi_vs_pressione_sd -> cd(1);
	gr_stdd -> Draw("AP");

	conteggi_vs_pressione_sd -> cd(2);
	gr_pressione -> Draw("AP");


	//rate vs temperatura doppie

	TCanvas* conteggi_vs_temperatura_sd = new TCanvas("conteggi_vs_temperatura_sd","conteggi_vs_temperatura_sd",0,0,700,500);
	conteggi_vs_temperatura_sd -> Divide(1,2);

	conteggi_vs_temperatura_sd -> cd(1);
	gr_stdd -> Draw("AP");

	conteggi_vs_temperatura_sd -> cd(2);
	gr_temperatura -> Draw("AP");

	double mean_rate_sd = gr_stdd -> GetMean(2);

	cout << "temperatura media: " << mean_temp << endl;
	cout << "pressione media: " << mean_press << endl;
	cout << "rate medio (doppie): " << mean_rate_sd << endl;

 //attesa : I = (I0+ALFA(P-P0) + BETA (T-T0) )*I0
	TCanvas* rate_reale_vs_atteso_sd = new TCanvas("rate_reale_vs_atteso_sd","rate_reale_vs_atteso_sd",0,0,700,500);
	rate_reale_vs_atteso_sd -> Divide(1,2);

	TGraph* gr_atteso_sd = new TGraph;
	gr_atteso_sd -> SetMarkerColor(kBlack);
	gr_atteso_sd -> SetMarkerSize(1);
	gr_atteso_sd -> SetMarkerStyle(8);


	for(int i = 0; i < doppie.size(); i++){
		int j = find_nearest(i + ORA_START); //trova l'indice che corrisponde all'ora più vicina

		double I_0 = mean_rate_sd;
		int N;
		N = gr_atteso_sd -> GetN();

		int temp = v_date[j] -> Get_temp();
		double press = v_date[j] -> Get_press();

		double y =I_0 + I_0*(ALFA*(mean_press - press) + BETA*(mean_temp - temp));

		gr_atteso_sd -> SetPoint(N,i,y);
	}

	rate_reale_vs_atteso_sd -> cd(1);
	gr_stdd -> Draw("AP");

	rate_reale_vs_atteso_sd -> cd(2);
	gr_atteso_sd -> Draw("AP");

//end

	myApp.Run();

	return 0;
}
