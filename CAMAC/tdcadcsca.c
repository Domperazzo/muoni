//********************************************************************************************************************************
/*    TDC - SCALER - ADC
gcc tdcadcsca.c -g -I. -I/usr/local/pcicc32-6.9/lib/ -I/usr/include -Wall -o tdcadcsca /usr/local/pcicc32-6.9/lib/libcc32.so.3.0.0

Descrizione:
Il programma legge e stampa (su terminale e su file) il registro del TDC corrispondente al canale TDC_CHAN posto nella station 
numerata con TDC_SLOT e due registri dell'ADC (posto in ADC_SLOT) e corrispondenti ai canali ADC_CHAN0 e ADC_CHAN1.
Per ogni singola misura si aspetta WAIT e in totale il tempo di misura è TIME.

N.B. In realtà, il tempo impiegato per la misura è pari a TIME * (op_time + WAIT), dove op_time è il tempo necessario perché il
calcolatore completi le operazioni nel ciclo for. Calcoliamo questo tempo con CLOCK: a ogni ciclo calcoliamo il tempo necessario
per completarlo (incluso il WAIT di attesa).
*/
//********************************************************************************************************************************

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
#define WAIT (unsigned long int) 100000   //wait time in microsecondi
#define TIME (unsigned long int) 10*60*60*20*10

#define STAMPSCA 20	//tempo di stampa dello scaler 

#include <time.h> 
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
  //Variabili necessarie
  char *fname = DEVICE_NAME;
  int Q = 0;
  int X = 0;
  long int i;
  long int j;
  long int w;
  long int h;
  double adcData0, adcData1;
  int z;
  int c;
  double tdcData;
  int error;
  int ich;
  long int k;
  long int count = 0;   // eventi TDC
  long int count0 = 0;  // eventi ADC0
  long int count1 = 0;  // eventi ADC1
//  time_t start_t, stop_t;

  //----------------------------------------APERTURA DEI FILE DI OUTPUT DEI DATI----------------------------------------
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
  if (datisca == NULL){
    perror("Errore apertura");
    exit(1);
  }

  FILE *datitime = fopen("./Dati/datitime.txt","w");
  if (datitime == NULL){
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


  //-------------------------------------------PULIZIA CANALI DEI VARI MODULI-------------------------------------------
  for (i = 0; i < 12; i++) {
     w = cc32_read_long(handle,ADC_SLOT,i,2);
    }

  for (j = 0; j < 8; j++) {
     k = cc32_read_long(handle,TDC_SLOT,j,2);    
    }

  for (i = 0; i < 12; i++) {
     k = cc32_read_long(handle,SCA_SLOT,i,2);
  }
  
  // Invia un comando a TDC e ADC nel sottocanale A
  // input: handle, N, A, F
  k = cc32_read_long(handle,TDC_SLOT,TDC_CHAN,9);         // Pulizia registri
  z = cc32_read_long(handle,ADC_SLOT,ADC_CHAN0,9);        // Pulizia registri
  z = cc32_read_long(handle,ADC_SLOT,ADC_CHAN1,9);        // Pulizia registri
  h = cc32_read_long(handle,TDC_SLOT,0,10);               // pulizia LAM
  c = cc32_read_long(handle,ADC_SLOT,0,10);               // pulizia LAM


  //---------------------------------------------LETTURA E SALVATAGGIO DATI---------------------------------------------
  Q = 0;

  for (j = 0; j < TIME; j++) 
   {
//     start_t = time(NULL);

     printf ("iteration j = %li\n", j);

     //Si fa una richiesta per verificare il comando sia capito
     //input: handle, N, A, F, &Q, &X
     w = cc32_read_long_qx(handle, TDC_SLOT,0,0,&Q,&X);

     usleep(WAIT);  // aspetta WAIT usecondi 

     //------------------------------SE IL COMANDO AL TDC È STATO ACCETTATO, ENTRA NELL'IF------------------------------
     if (Q == 1 && X == 1)
       {
         // la funzione legge il canale TDC_CHAN e pulisce i registri
	 for(i = 0; i < 8; i++){
	   Q = cc32_read_word(handle,TDC_SLOT,i,2);
	   if (i == TDC_CHAN) tdcData = Q;
          }

         //----------------STAMPATI DATI SE tdcData != 0----------------
	 if (tdcData != 0)
          {
            //se il canale del tdc non è vuoto, incrementa il conteggio
	    count += 1;

	    //stampa su terminale e su file i dati di TDC 
	    printf("%f",tdcData);
            fprintf(dati, "%f \n", tdcData);

            for (ich = 0; ich < 12; ich++){
               //la funzione legge il canale 0 dell'ADC e pulisce i registri
               adcData0 = cc32_read_word(handle,ADC_SLOT,ich,2);
               //stampa su terminale e su file i dati di ADC0
               if (ich == ADC_CHAN0){
	         printf("	%f",adcData0);
	         fprintf(datiADC0, "%f \n", adcData0);
                }

               //la funzione legge il canale 1 dell'ADC e pulisce i registri
               adcData1 = cc32_read_word(handle,ADC_SLOT,ich,2);
               //stampa su terminale e su file i dati di ADC1
               if (ich == ADC_CHAN1){
	         printf("	%f",adcData1);
	         fprintf(datiADC1, "%f \n", adcData1);
	        }
              }

	    printf ("\n");
          }
         //-------------------------------------------------------------	
		  
         //se il canale 0 non è vuoto, incrementa il conteggio
         if (adcData0 != 0) count0 += 1;
         		  
         // Se il canale 1 non è vuoto, incrementa il conteggio
         if (adcData1 != 0) count1 += 1;       
       }
     //-----------------------------------------------------------------------------------------------------------------

     // Invia un comando al TDC nel sottocanale A
     // input: handle, N, A, F
     h = cc32_read_long(handle,TDC_SLOT,0,10);   // pulizia LAM
     c = cc32_read_long(handle,ADC_SLOT,0,10);   // pulizia LAM

     //--------------------------------STAMPA SU TERMINALE E SU FILE I DATI DELLO SCALER--------------------------------
     //----------------------------CHIUDO E RIAPRO FILE CON DATI PER SALVARLI PERIODICAMENTE----------------------------
     if (j % STAMPSCA == 0)
       {
	 printf("\nScaler counts: ");  
         for (i = 0; i < 3; i++)  // per i 3 registri dello scaler
          {                
            k = cc32_read_long(handle,SCA_SLOT,i,0); // lettura registri e stampa
            printf("%ld ",k);
            fprintf(datisca, "%ld ", k);
          }  
	 printf("\n");     
	 fprintf(datisca, "\n");
/*
         fclose(dati);
         fclose(datiADC0);
         fclose(datiADC1);
         fclose(datisca);
         fclose(datitime);

         dati = fopen("./Dati/datiTDC.txt","a");
  	 if (dati == NULL){
           perror("Errore apertura");
           exit(1);
          }

         datiADC0 = fopen("./Dati/datiADC0.txt","a");
         if (datiADC0 == NULL){
           perror("Errore apertura");
           exit(1);
          }

         datiADC1 = fopen("./Dati/datiADC1.txt","a");
         if (datiADC1 == NULL){
           perror("Errore apertura");
           exit(1);
          }

         datisca = fopen("./Dati/datiscaler.txt","a");
         if (datisca == NULL){
           perror("Errore apertura");
           exit(1);
          }

         datitime = fopen("./Dati/datitime.txt","a");
         if (datitime == NULL){
           perror("Errore apertura");
           exit(1);
          }
*/
       }
     //-----------------------------------------------------------------------------------------------------------------
/*
     stop_t = time(NULL);
     fprintf (datitime, "%f\n", (float)((stop_t - start_t)*1000000));   //in microsecondi
     printf ("%f\n", (float)((stop_t - start_t)*1000000));   //in microsecondi
*/
   }

  // Chiusura comunicazioni
  cc32_close(handle);
  printf("%s: close done.\n", cszPrgName);
  fclose(dati);
  fclose(datiADC0);
  fclose(datiADC1);
  fclose(datisca);
  fclose(datitime);

  return 0;
}
