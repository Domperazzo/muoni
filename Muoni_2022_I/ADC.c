//****************************************************************************
/*    ADC
gcc ADC.c -g -I. -I/usr/local/pcicc32-6.9/lib/ -I/usr/include -Wall -o ADC /usr/local/pcicc32-6.9/lib/libcc32.so.3.0.0

Descrizione:
Il programma legge e stampa (a terminale e su file) gli 8 registri dell'
ADC posto nella station numerata con ADC_SLOT.
La misura dura in totale TIME, mentre il singolo dato ha a disposizione
una finestra temporale di WAIT.

*/
//****************************************************************************

// N è il numero della stazione nel crate (0-24)
// A è il subadress (usato per moduli a più sezioni)
// F è il numero della funzione

#define DEVICE_NAME "/dev/cc32_1"
#define ADC_SLOT 11
#define TIME 10*WAIT*60           //tempo totale di misura in microsecondi
#define WAIT 100000                //tempo per ogni singola misura

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
  //variabili necessaria
  char *fname = DEVICE_NAME;
  long int i , j;
  int error;
  long int k;
  long int w;
  int Q;
  int X;

  // apertura del file di output dei dati
  FILE *dati = fopen("./Dati/datiADC.txt","w");
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





  for (i = 0; i < (2*TIME); i++){
    w = cc32_read_long_qx(handle, ADC_SLOT,0,0,&Q,&X);
    if (Q == 1 && X == 1){
      usleep(WAIT);
      printf("ADC counts: ");
      for (j = 0; j < 12; j++) {             //per ciascun canale stampa
        k = cc32_read_word(handle,ADC_SLOT,j,2);
        printf("%ld ",k);
        fprintf(dati, "%ld ", k);
      }
      if (i*WAIT >= TIME) break;  //se si è superato TIME si esce
      printf("\n");
      fprintf(dati, "\n");
    }
	k = cc32_read_long(handle,ADC_SLOT,0,10);
  }

  //stampa dei risultati
  printf("\n");


  //chiude il canale di comunicazione e il file
  cc32_close(handle);
  printf("%s: close done.\n", cszPrgName);
  fclose(dati);

  return 0;
}
