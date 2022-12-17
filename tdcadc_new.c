//****************************************************************************
/*    TDC - ADC
gcc tdcadc_new.c -g -I. -I/usr/local/pcicc32-6.9/lib/ -I/usr/include -Wall -o tdcadc_new /usr/local/pcicc32-6.9/lib/libcc32.so.3.0.0

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
#define ADC_SLOT 12
#define TDC_SLOT 16
#define NEVT 80000
#define TDC_CHAN 0
#define ADC_CHAN0 0  //S1
#define ADC_CHAN1 1  //S2
#define WAIT 100000 //wait time per una misura (in microsecondi) E' il tempo che aspetta prima di interrogare di nuovo TDC e ADC, che scaricano e si ripuliscono quindi con rate =1/WAIT. NB: memorizzano solo il primo evento dopo la pulizia.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <ctype.h>

#include "lib/libcc32.h"

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
	long int adcData0=0;
	long int adcData1=0;
	int z;
	long int tdcData=0;
	int k;
	int error;

	// apertura del file di output dei dati
	char nome_file[]= "Dati/Dati_tdcadc";
	strcat(nome_file, argv[1]);
	strcat(nome_file,".txt");
	FILE *dati = fopen(nome_file,"w");
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

	//Reset del crate
	cc32_write_long(handle,31,0,0,0); /*reset del cc32*/
	cc32_write_long(handle,0,2,0,0);  /*Clear crate + inhibit reset*/
	cc32_write_long(handle,0,1,0,0);  /*initialize crate*/
	cc32_write_long(handle,28,0,0,0); /*LAM-FF reset*/

	// Reimposta a zero i registri del camac controller (NB: la sua slot è infatto la 0)
	// input: handle, N, A, F, uwData
	// uwdata è (forse) il dato da scrivere nei registri e quindi lo mette a zero
	cc32_write_word(handle,0,0,16,0);
	//cc32_write_word(handle,0,1,16,0);


	//Reset di TDC e ADC (legge fino all' ultimo canale e poi cancella tutto)
        k=cc32_read_word(handle,TDC_SLOT,7,2);
        z=cc32_read_word(handle,ADC_SLOT,11,2);
	
	while (j < NEVT){
		time++;
                //printf("Ciclo while #:%d\n",time);
		if(time % 100 == 0){
			// ogni cento cicli stampa:
			// tempo
			// eventi (ovvero numero cicli con risposta)
			// rate (eventi / tempo)
			//-----!!!----unità di misura del rate sono casuali
		  printf("successi = %li/%d",j, NEVT);
		}

		// Il comando successivo  pulisce Q dei registri di TDC e ADC (usando la F9):
		// input: handle, N, A, F, &Q, &X
       		w = cc32_read_long_qx(handle,TDC_SLOT,0,9,&Q,&X);
		t = cc32_read_long_qx(handle,ADC_SLOT,0,9,&Q1,&X1);
		//printf("Pulisce Q \n");
		//printf("Q =%d, X=%d, Q1=%d, X1=%d\n",Q,X,Q1,X1);		


		//Ad ogni ciclo while aspetta un tempo=WAIT prima di interrogare i registri di TDC e ADC. 
		//Restituisce poi il valore che ha memorizzato (ovvero il primo dopo che è iniziato un nuovo while)
		usleep(WAIT);


		// Il comando successivo legge Q ed X dei registri di TDC e ADC (usando la F0):
		// input: handle, N, A, F, &Q, &X
       		w = cc32_read_long_qx(handle,TDC_SLOT,0,0,&Q,&X);
		t = cc32_read_long_qx(handle,ADC_SLOT,0,0,&Q1,&X1);        	
		//printf("Legge Q e X\n");
		//printf("Q =%d, X=%d, Q1=%d, X1=%d\n",Q,X,Q1,X1);		

		// Se sia ADC che TDC hanno letto qualcosa entra nell'if
		if (Q == 1 && X == 1 && Q1 == 1 && X1 == 1){	  
		  //    printf("Ha trovato Q,X,Q1,X1 ==1\n");
		  //    printf("Q =%d, X=%d, Q1=%d, X1=%d\n",Q,X,Q1,X1);
		        
			//la funzione 2 legge e pulisce i registri
			//input: handle, N, A, F
			tdcData = cc32_read_word(handle, TDC_SLOT,TDC_CHAN,2);
			adcData0 = cc32_read_word(handle,ADC_SLOT,ADC_CHAN0,2);
			adcData1 = cc32_read_word(handle,ADC_SLOT,ADC_CHAN1,2);

			//Stampo in file e a terminale
			if(tdcData||adcData0||adcData1 !=0){
			  j++;
			  //printf("Evento:%ld\n",j);
			  //printf("TDC\tADC0\tADC1\n");
			  fprintf(dati,"%ld %ld %ld\n", tdcData, adcData0, adcData1);
			  printf("%ld %ld %ld\n", tdcData, adcData0, adcData1);
			}
			
		}
	}
	// Stampa del rate di successi 
	double rate = 1.*NEVT/(time)*100;
	printf("Frequenza successi = %lf percento \n",rate);


	// Chiusura comunicazioni
	cc32_close(handle);
	printf("%s: close done.\n", cszPrgName);
	fclose(dati);

	return 0;
}
