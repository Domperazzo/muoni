class evento{
	public:
		evento();
		evento(double tdcval, double adc0val, double adc1val){
			tdc = tdcval;
			adc0 = adc0val;
			adc1 = adc1val;			
		};

		double Get_tdc(){
			return tdc;
		}
		double Get_adc0(){
			return adc0;
		}
		double Get_adc1(){
			return adc1;
		}
		
				//correzione nuova sigmoid
		double Get_tdc_corretto(double tau0, double vth0, double tau1, double vth1){
			return tdc + tau0 * log(vth0/(adc0-vth0))  - 4/tau0 - tau1 * log(vth1/(adc1-vth1)) + 4/tau1;
		}
		double Get_tdc_adc0(double tau0, double vth0, double tau1, double vth1){
			return tdc - tau0 * log(vth0/(adc0-vth0)) + 4/tau0;
		}
		double Get_tdc_adc1(double tau0, double vth0, double tau1, double vth1){
			return tdc - tau1 * log(vth1/(adc1-vth1)) + 4/tau1;
		}

		
	
		double Get_tdc_off(double off0, double off1){
			return - tdc + off0 + off1;
		}
	private:
		double tdc;
		double adc0;
		double adc1;
};

		// funzione sigmoide
double myFun(double *x, double *par){		
	double A = par[0];		//tau
	double B = par[1];		//Vth
	double C = par[2];		//offset
	double D = x[0];
	return C+A*log(B/(D-B));
}

void graficoiniziale(double mins1_graph,double maxs1_graph,TGraphErrors* g_adc0,double mins2_graph,double maxs2_graph,TGraphErrors* g_adc1){

	g_adc0 -> GetXaxis() -> SetTitle("canali ADC");
	g_adc0 -> GetYaxis() -> SetTitle("canali TDC");
	g_adc0 -> SetMarkerStyle(20);
	g_adc0 -> SetMarkerSize(0.5);
	g_adc0 -> SetMarkerColor(kRed);

	g_adc1 -> GetXaxis() -> SetTitle("canali ADC");
	g_adc1 -> GetYaxis() -> SetTitle("canali TDC");
	g_adc1 -> SetMarkerStyle(20);
	g_adc1 -> SetMarkerSize(0.5);
	g_adc1 -> SetMarkerColor(kBlue);

	for(int i = mins1_graph; i <= maxs1_graph; i++){
		double sum = 0;
		double sumt = 0;
		int count = 0;
		double dev = 0;
		for(int j = 0; j < v_eventi.size(); j++){
			double adc0 = v_eventi[j] -> Get_adc0();
			double adc1 = v_eventi[j] -> Get_adc1();

			bool limits = (adc0 > mins1_graph && adc0 < maxs1_graph) && (adc1 > mins2_graph && adc1 < maxs2_graph);

			double tdc = v_eventi[j] -> Get_tdc();
			if(adc0 == i && limits){
				count++;
				sumt = sumt + tdc;
			}
		}
		sumt = sumt / count;
		for(int j = 0; j < v_eventi.size(); j++){
			double adc0 = v_eventi[j] -> Get_adc0();
			double adc1 = v_eventi[j] -> Get_adc1();

			bool limits = (adc0 > mins1_graph && adc0 < maxs1_graph) && (adc1 > mins2_graph && adc1 < maxs2_graph);
			double tdc = v_eventi[j] -> Get_tdc();
			if(adc0 == i && limits){
				dev = dev + pow(tdc-sumt,2)/(count -1);
			}
		}
		dev = sqrt(dev) / sqrt(count);
		if(count == 1) dev = E_TDC;
		int N = g_adc0 -> GetN();
		if(count != 0){
			g_adc0 -> SetPoint(N,i,sumt);
		  g_adc0 -> SetPointError(N,E_ADC,dev);
		}
	}

	for(int i = mins2_graph; i <= maxs2_graph; i++){
		double sum = 0;
		double sumt = 0;
		int count = 0;
		double dev = 0;
		for(int j = 0; j < v_eventi.size(); j++){
			double adc0 = v_eventi[j] -> Get_adc0();
			double adc1 = v_eventi[j] -> Get_adc1();

			bool limits = (adc0 > mins1_graph && adc0 < maxs1_graph) && (adc1 > mins2_graph && adc1 < maxs2_graph);
			double tdc = v_eventi[j] -> Get_tdc();
			if(adc1 == i && limits){
				count++;
				sumt = sumt + tdc;
			}
		}
		sumt = sumt / count;
		for(int j = 0; j < v_eventi.size(); j++){
			double adc0 = v_eventi[j] -> Get_adc0();
			double adc1 = v_eventi[j] -> Get_adc1();

			bool limits = (adc0 > mins1_graph && adc0 < maxs1_graph) && (adc1 > mins2_graph && adc1 < maxs2_graph);
			double tdc = v_eventi[j] -> Get_tdc();
			if(adc1 == i && limits){
				dev = dev + pow(tdc-sumt,2)/(count -1);
			}
		}
		dev = sqrt(dev) / sqrt(count);
		if(count == 1) dev = E_TDC;
		int N = g_adc1 -> GetN();
		if(count != 0){
			g_adc1 -> SetPoint(N,i,sumt);
			g_adc1 -> SetPointError(N,E_ADC,dev);
		}
	}
}

void setlimits(TH1D * myhisto, double * min, double * max){	
	double maxcount = 0;
	int maxcountbin = 0;
	for(int i = 0; i < 900; i++){
		int count = myhisto -> GetBinContent(i);
		if (count > maxcount){ 
			maxcountbin = i;
			maxcount = count;
		}
	}

	TF1 * g = new TF1("g","landau",85,95);	
	if(fun1 == true){
		g -> SetParameter (1,17);
	}
	else g -> SetParameter (1,50);
	myhisto -> Fit(g, "M Q");
	g -> SetParLimits(1,maxcountbin-2, maxcountbin+2);
	double mean = g -> GetParameter (1);
	double sigma = g -> GetParameter (2);
	if(fun1 == true){
		*min = mean - 2 * sigma;
		*max = mean + 2 * sigma;
		fun1 = false;
	}
	else{
		*min = mean - 2 * sigma;
		*max = mean + 2 * sigma;
	}
	cout << "media: " << mean << "  sigma: " << sigma << endl;
	return;
}

/*
	//setlimits con FWHM
void setlimits(TH1D * myhisto, double * min, double * max){
	double maxcount = 0;
	int maxcountbin = 0;
	for(int i = 0; i < 900; i++){
		int count = myhisto -> GetBinContent(i);
		if (count > maxcount){ 
			maxcountbin = i;
			maxcount = count;
		}
	}
	maxcount = maxcount/2;

	int minbin;
	int maxbin;

	double delta = 100000;
	for(int i = 0; i < maxcountbin; i++){
		double temp = abs(maxcount - myhisto -> GetBinContent(i));
		if(delta > temp){
			delta = temp;
			minbin = i;
		}
	}

	delta = 100000;
	for(int i = maxcountbin; i < 900 ; i++){
		double temp = abs(maxcount - myhisto -> GetBinContent(i));
		if(delta > temp){
			delta = temp;
			maxbin = i;
		}
	}

	*min = myhisto -> GetBinCenter(minbin);
	*max = myhisto -> GetBinCenter(maxbin);

	cout << "\nmaxcount: " << maxcount << endl;
	cout << "bin: " << minbin << " - " << maxcountbin << " - " << maxbin << endl;
}
*/

double rand_adc(double min, double max){
	double n;
	do{
		n = random_gen(gen);
	}
	while (n < min || n > max);
	return n;
}

bool leggi_dati(vector<double> & ampiezzas1, int channel, char* myFile){
	double scal1;
	ifstream Infile;
	Infile.open(myFile, fstream::in);
	if (Infile.good () == false){
		cout << "Errore nell'apertura dei file " << myFile << endl;
		return false;
	}
	cout << "Leggo file di dati " << myFile << endl;
	while(true){
		Infile >> scal1;
		ampiezzas1.push_back(scal1);
		if (Infile.eof() == true) break;
		if (ampiezzas1.size()==300000) break;
	}
	Infile.close();
	cout << "Numero dati: " << ampiezzas1.size() << endl;
	return true;
}

double rand_tau(){
	return random_tau(gen);
}

double rand_vth(){
	return random_vth(gen);
}

bool leggi_dati_container(vector<double> & ampiezzas1, vector<double> & ampiezzas2, vector<double> & tempi, int argc, char * argv[]){
	if(argc < 2){
		cout << "Devi inserire la directory dei dati" << endl;
		return false;
	}
	int dist = 0;


	string path = argv[1];
	string path_adc0 = path + ADC0_FILE;
	string path_adc1 = path + ADC1_FILE;
	string path_tdc = path + TDC_FILE;

	if(!(leggi_dati(ampiezzas1, CHAN_ADC0, &path_adc0[0]))) return false;
	if(!(leggi_dati(tempi, CHAN_TDC, &path_tdc[0]))) return false;
	if(!(leggi_dati(ampiezzas2, CHAN_ADC1, &path_adc1[0]))) return false;

	for(int i = 0; i < tempi.size(); i++){
		if(tempi[i] != 0 && ampiezzas1[i] != 0 && ampiezzas2[i] != 0){
			evento * temp = new evento(tempi[i], ampiezzas1[i], ampiezzas2[i]);
			v_eventi.push_back(temp);
		}
		else{
			dist++;
		}
	}

	cout << "Eventi: " << v_eventi.size() << endl;
	cout << "Eventi scartati: " << dist << endl;
	
	return true;
}

void fillAdcHistos(TH1D* h_adc0, TH1D* h_adc1, TH1D* h2_adc01){
	for(int i = 0; i < v_eventi.size(); i++){
		double adc0 = v_eventi[i] -> Get_adc0();
		double adc1 = v_eventi[i] -> Get_adc1();
		double tdc = v_eventi[i] -> Get_tdc();

		h_adc0 -> Fill(adc0);
		h_adc1 -> Fill(adc1);
		h2_adc01 -> Fill(adc0,adc1);
	}
}









