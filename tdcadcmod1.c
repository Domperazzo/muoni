//****************************************************************************
/*    TDC - ADC
gcc tdcadcmod1.c -g -I. -I/usr/local/pcicc32-6.9/lib/ -I/usr/include -Wall -o tdcadc /usr/local/pcicc32-6.9/lib/libcc32.so.3.0.0

Descrizione:
Il programma legge e stampa (a terminale e su file) i registri:
TDC_CHAN 									per il TDC
ADC_CHAN0 e ADC_CHAN1			per l'ADC
posti nelle station numerate con TDC_SLOT e ADC_SLOT.
La misura si ferma dopo un numero di eventi definito in NEVT.

*/
//****************************************************************************

// N è il numero della stazione nel crate (0-24)
// A è il subadress (usato per moduli a più sezioni)
// F è il numero della funzione
// Q è lo status della risposta della station (0 o 1)
// X è lo status dell'accettazione del comando (0 o 1)

#define DEVICE_NAME "/dev/cc32_1"
#define ADC_SLOT 19
#define TDC_SLOT 16
#define NEVT 1000
#define TDC_CHAN 0   
#define ADC_CHAN0 0  //S1
#define ADC_CHAN1 1  //S2
#define WAIT 1000				//wait time per una misura (in microsecondi)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <ctype.h>

#include "../lib/libcc32.h"

char *cszPrgName;
CC32_HANDLE handle;

int main(int argc, char **argv){
	// Variabili necessarie
	char *fname = DEVICE_NAME;
	int Q = 0;
	int X = 0;
	int Q1 = 0;
	int X1 = 0;
	long int j = 0;
	int time = 0;
	long int w;
	long int t;
	long int adcData0;
	long int adcData1;
	int z;
	long int tdcData;
	int k;
	int error;

	// apertura del file di output dei dati
	FILE *dati = fopen("./Dati/datitdcadc.txt","w");
	if (dati == NULL){
		perror("Errore apertura");
		exit(1);
	}

	//apre un canale di comunicazione
	if ((error = cc32_open(fname, &handle))){
		fprintf(stderr, "%s: %s: %s\n", cszPrgName, fname, strerror(error));
		exit(1);
	}
	else{
		printf("%s: open done.\n", cszPrgName);
	}

	// Reimposta a zero i registri:
	// input: handle, N, A, F, uwData
	// uwdata è (forse) il dato da scrivere nei registri

	//----!!!!-------perchè stiamo azzerando queste stations? non sense
	cc32_write_word(handle,0,0,16,0);
	cc32_write_word(handle,0,1,16,0);

	// Invia un comando al TDC e all'ADC
	// input: handle, N, A, F
	k = cc32_read_long(handle,TDC_SLOT,TDC_CHAN,9); 	//pulizia registri
	z = cc32_read_long(handle,ADC_SLOT,ADC_CHAN0,9);	//pulizia registri
	z = cc32_read_long(handle,ADC_SLOT,ADC_CHAN1,9);	//pulizia registri
	k = cc32_read_long(handle,TDC_SLOT,0,10); 	//pulizia LAM
	z = cc32_read_long(handle,ADC_SLOT,0,10);		//pulizia LAM

	while (j < NEVT){
		time++;
		if(time % 100 == 0){
			// ogni cento cicli stampa:
			// tempo
			// eventi (ovvero numero cicli con risposta)
			// rate (eventi / tempo)
			//-----!!!----unità di misura del rate sono casuali
			printf("time = %lf, events = %ld, rate = %lf\n",1.*time/100,j,10.*j/time);
		}
		usleep(WAIT);
		// la funzione legge e pulisce i registri:
		// input: handle, N, A, F, &Q, &X
		//---!!!--- forse meglio F=0 che non cancella i registri
		w = cc32_read_long_qx(handle, TDC_SLOT,0,2,&Q,&X);
		t = cc32_read_long_qx(handle, ADC_SLOT,0,2,&Q1,&X1);
		// se i comandi sono accettati sia da ADC che da TDC entra nell'if
		if (Q == 1 && X == 1 && Q1 == 1 && X1 == 1){
			j++;
			//la funzione 2 legge e pulisce i registri
			//input: handle, N, A, F
			tdcData = cc32_read_word(handle, TDC_SLOT,TDC_CHAN,2);
			adcData0 = cc32_read_word(handle,ADC_SLOT,ADC_CHAN0,2);
			adcData1 = cc32_read_word(handle,ADC_SLOT,ADC_CHAN1,2);

			//Stampo in file e a terminale
			fprintf(dati, "%ld %ld %ld\n", tdcData, adcData0, adcData1);
			printf("%ld %ld %ld\n", tdcData, adcData0, adcData1);
		}
	}
	// Stampa del rate (unità di misura casuali)
	double rate = 1.*NEVT/time*10;
	printf("RATE = %lf\n",rate);


	// Chiusura comunicazioni
	cc32_close(handle);
	printf("%s: close done.\n", cszPrgName);
	fclose(dati);

	return 0;
}
