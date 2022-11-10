//****************************************************************************
/*    TDC - ADC
gcc tdcadcsca_original.c -g -I. -I/usr/local/pcicc32-6.9/lib/ -I/usr/include -Wall -o tdcadcsca_original /usr/local/pcicc32-6.9/lib/libcc32.so.3.0.0

Descrizione:
Il programma legge e stampa (a terminale e su file) il registro del TDC
corrispondente al canale TDC_CHAN  posto nella station numerata con TDC_SLOT
e due registri dell'ADC (posto in ADC_SLOT) e corrispondenti ai canali
ADC_CHAN0 e ADC_CHAN1.
Per ogni singola misura si aspetta WAIT e in totale il tempo di misura
è TIME.

*/
//***************************************************************************

// N è il numero della stazione nel crate (0-24)
// A è il subadress (usato per moduli a più sezioni)
// F è il numero della funzione
// Q è lo status della risposta della station (0 o 1)
// X è lo status dell'accettazione del comando (0 o 1)

#define DEVICE_NAME "/dev/cc32_1"
#define ADC_SLOT 12
#define TDC_SLOT 16
#define SCA_SLOT 21 
#define TDC_CHAN 0
#define ADC_CHAN0 0
#define ADC_CHAN1 1
#define WAIT (unsigned long int) 10000     	            //wait time in usecondi
//#define WAITSEC (unsigned long int)WAIT*10 //un secondo in usec 
#define TIME (unsigned long int) 10*60*40 //tempo di misura in usecondi divido 1e4 5 MINUTI

#define STAMPSCA 10	//tempo di stampa dello scaler 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>

#include "../lib/libcc32.h"

char *cszPrgName;
CC32_HANDLE handle;

int main(int argc, char **argv){
  //Variabili necessarie
  char *fname = DEVICE_NAME;
  int Q = 0;
  int X = 0;
  long int i;
  long int j;
  long int w;
  long int h;
  double adcData;
  int z;
  int c;
  double tdcData;
  int error;
  int ich1;
  long int k;
  long int count = 0;   // eventi TDC
  long int count1 = 0;  // eventi ADC0
  long int count2 = 0;  // eventi ADC1

  // apertura dei file di output dei dati
  // Uno per il TDC, due per i due canali dell'ADC e uno per lo scaler
  FILE *dati = fopen("./Dati/datiTDC.txt","w");
  if (dati == NULL){
    perror("Errore apertura");
    exit(1);
  }
  FILE *datiADC0 = fopen("./Dati/datiADC0.txt","w");
  if (datiADC0 == NULL){
    perror("Errore apertura");
    exit(1);
  }
  FILE *datiADC1 = fopen("./Dati/datiADC1.txt","w");
  if (datiADC1 == NULL){
    perror("Errore apertura");
    exit(1);
  }

  FILE *datisca = fopen("./Dati/datiscaler.txt","w");
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

  //resetta tutti i canali dell'ADC
  for(i = 0; i < 12; i++){
    w = cc32_read_long(handle,ADC_SLOT,i,2);
  }

  for(j = 0; j < 8; j++){
    k = cc32_read_long(handle,TDC_SLOT,j,2);    //pulizia registri
  }
  for (i = 0; i < 12; i++) k = cc32_read_long(handle,SCA_SLOT,i,2);
  // Invia un comando a TDC e ADC nel sottocanale A
  // input: handle, N, A, F
  k = cc32_read_long(handle,TDC_SLOT,TDC_CHAN,9);         // Pulizia registri
  z = cc32_read_long(handle,ADC_SLOT,ADC_CHAN0,9);        // Pulizia registri
  z = cc32_read_long(handle,ADC_SLOT,ADC_CHAN1,9);        // Pulizia registri
  h = cc32_read_long(handle,TDC_SLOT,0,10);               // pulizia LAM
  c = cc32_read_long(handle,ADC_SLOT,0,10);               // pulizia LAM

  Q = 0;
  int tempo = 0;

  clock_t t; //per misurare il tempo impiegato dal programma per campionare i dati
  t = clock();

  for (j = 0; j < 2*TIME*WAIT; j++) 
{
    // Si fa una richiesta per verificare il comando sia capito:
    // input: handle, N, A, F, &Q, &X

    //---!!!---- di fatto basta un canale qualsiasi, anzi forse se
    //           è diverso da quello utile è meglio.
    //           Eventualmente F=0 non cancella i registri
    w = cc32_read_long_qx(handle, TDC_SLOT,0,0,&Q,&X);

	if (tempo == 10*STAMPSCA ){
	  printf("\nScaler counts: ");   
      for (i = 0; i < 3; i++) {                 // per i 12 registri dello scaler
        k = cc32_read_long(handle,SCA_SLOT,i,0); // lettura registri e stampa
        printf("%ld ",k);
        fprintf(datisca, "%ld ", k);
      }
      tempo = 0;
	  fprintf(datisca, "\n");
    }
	tempo = tempo + 1;
    usleep(WAIT);  // aspetta WAIT usecondi 

    // se il comando al TDC è stato accettato entra nell'if
    if (Q == 1 && X == 1){
      // la funzione legge il canale 0 e pulisce i registri:
      // input: handle, N, A, F, &Q, &X
	for(i = 0; i < 8; i++){
		Q = cc32_read_word(handle,TDC_SLOT,i,2);
		if (i == TDC_CHAN) tdcData = Q;
	}
       if (tdcData != 0) printf("\ntdc:%f \n",tdcData);
      //incrementa il conteggio del TDC se il canale è non nullo
      if (tdcData != 0){
	  count += 1;
      fprintf(dati, "%f ", tdcData);
	  }

      //legge tutti i canali dell'ADC
       if (tdcData != 0) printf("adc: ");
      for (ich1 = 0; ich1<12; ich1++){
        adcData = cc32_read_word(handle,ADC_SLOT,ich1,2);
        // Stampa a terminale ogni canale dell'ADC
        if(ich1 == ADC_CHAN0){
		   if (tdcData != 0){
			printf("%f ",adcData);
			fprintf(datiADC0, "%f ", adcData);
			}			
          // Se il canale 0 non è vuoto incrementa il conteggio
          if (adcData != 0) count1 += 1;
        }
        if(ich1 == ADC_CHAN1){
           if (tdcData != 0){
			 printf("%f ",adcData);
			 fprintf(datiADC1, "%f ", adcData);
			}
          // Se il canale 4 non è vuoto incrementa il conteggio
          if (adcData != 0) count2 += 1;
        }
      }

      if (tdcData != 0) fprintf(dati, "\n");
      if (tdcData != 0) fprintf(datiADC0, "\n");
      if (tdcData != 0) fprintf(datiADC1, "\n");
    }

    // Invia un comando al TDC nel sottocanale A
    // input: handle, N, A, F
    h = cc32_read_long(handle,TDC_SLOT,0,10);   // pulizia LAM
    c = cc32_read_long(handle,ADC_SLOT,0,10);   // pulizia LAM

	printf("\nTempo rimanente %li s", (TIME-j)/10);
    if (j >= TIME) break;  //se si è superato TIME si esce

  }

  t = clock() - t;  //fine misura tempo
  printf ("\nit took us (%f clicks) (%f seconds) to acquire data\n", (float)t, ((float)t)/CLOCKS_PER_SEC);

  // Chiusura comunicazioni
  cc32_close(handle);
  printf("%s: close done.\n", cszPrgName);
  fclose(dati);
  fclose(datiADC0);
  fclose(datiADC1);

  return 0;
}
