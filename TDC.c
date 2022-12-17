//**************************************************************************
/*    TDC
gcc TDC.c -g -I. -I/usr/local/pcicc32-6.9/lib/ -I/usr/include -Wall -o TDC /usr/local/pcicc32-6.9/lib/libcc32.so.3.0.0

Descrizione:
Il programma legge e stampa (a terminale e su file) gli 8 registri del TDC
posto nella station numerata con TDC_SLOT.
La misura dura un massimo tempo TIME o qualora si raggiunga un numero
di misure riuscite superiore a MAXEV.
Ogni WAIT microsecondi vengono letti e azzerati i registri perciò è
importante che WAIT sia > dei ritardi in gioco.

*/
//***************************************************************************

// N è il numero della stazione nel crate (0-24)
// A è il subadress (usato per moduli a più sezioni)
// F è il numero della funzione
// Q è lo status della risposta della station (0 o 1)
// X è lo status dell'accettazione del comando (0 o 1)

#define DEVICE_NAME "/dev/cc32_1"
#define TDC_SLOT 16			
#define TIME (unsigned long int)600 // espresso in usecondi divido 1e-5
#define WAIT (unsigned long int)1   // espresso in usecondi divido 1e-5
#define MAXEV 2000000    // numero massimo di eventi

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
  long int j ;
  long int w;
  long int h;
  int tdcData;
  int error;
  int ich;
  long int k;
  long int count = 0;

  // apertura del file di output dei dati
  FILE *dati = fopen("./Dati/datitdc.txt","w");
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
  // Invia un comando al TDC nel sottocanale A
  cc32_write_word(handle,0,1,16,0);

  // input: handle, N, A, F
  for(j = 0; j < 8; j++){
    k = cc32_read_long(handle,TDC_SLOT,j,9);    //pulizia registri
  }
  h = cc32_read_long(handle,TDC_SLOT,0,10);   //pulizia test LAM

  long int time = 0;
  j = 0;

  while(time < 2*TIME/WAIT && count < 2*MAXEV){
    time++;
    if(time % 100 ==  0){
      // ogni cento cicli stampa:
      // tempo
      // eventi (ovvero numero cicli con risposta)
      // rate (eventi / tempo)
      //-----!!!----unità di misura del rate sono casuali
    }
    usleep(WAIT*100000);
    Q = 0;
    // la funzione legge e pulisce il registro al canale 0:
    // input: handle, N, A, F, &Q, &X
    //---!!!--- Forse è meglio mettere la funzione 0
    w = cc32_read_long_qx(handle, TDC_SLOT,0,2,&Q,&X);

    // se il comando è accettato entra nell'if
    if (Q == 1 && X == 1) {
      j++;
      printf("tdc registers: ");
      for (ich = 0; ich < 8; ich++){  //per ogni canale del TDC stampa i registri 
        //la funzione 2 legge e pulisce i registri
        //input: handle, N, A, F
        tdcData = cc32_read_word(handle,TDC_SLOT,ich,2);
        printf("%d ",tdcData);
        if (tdcData != 0 && (ich == 0 || ich == 1) && tdcData < 1000){
          fprintf(dati, "%d     ", tdcData);
          count++;
        }        
      }
      fprintf(dati, "\n");
      
      printf("\n");
    }

    // Azzera il test LAM, probabilmente inutile
    h = cc32_read_long(handle,TDC_SLOT,0,10);

	printf("\nEVENTI = %li \n", count);
	printf("condizione: %li > %li \n", time*WAIT, TIME);
    if (time*WAIT >= TIME) break;  // se si è superato TIME si esce
    if (count >= MAXEV) break;         // se si è superato MAXEV si esce
  }


  // Chiude comunicazioni
  cc32_close(handle);
  printf("%s: close done.\n", cszPrgName);
  fclose(dati);

  return 0;
}
