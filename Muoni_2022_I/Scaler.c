//****************************************************************************
/*    SCALER
gcc Scaler.c -g -I. -I/usr/local/pcicc32-6.9/lib/ -I/usr/include -Wall -o Scaler /usr/local/pcicc32-6.9/lib/libcc32.so.3.0.0

Descrizione:
Il programma legge e stampa (a terminale e su file) i 12 registri dello
scaler posto nella station numerata con SCA_SLOT.
La misura dura TIME e stampa ogni TIME_STAMP.

*/
//****************************************************************************

// N è il numero della stazione nel crate (0-24)
// A è il subadress (usato per moduli a più sezioni)
// F è il numero della funzione

#define DEVICE_NAME "/dev/cc32_1"
#define SCA_SLOT 21
#define TIME 60*3         //tempo di misura in secondi
#define TIME_STAMP 10          //ogni quanto tempo il programma stampa

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

  // apertura del file di output dei dati
  FILE *dati = fopen("./Dati/datiscaler_modified.txt","w");
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


  // Invia un comando allo scaler nel sottocanale A
  // input: handle, N, A, F

  for (i = 0; i < 12; i++) k = cc32_read_long(handle,SCA_SLOT,i,2);


  // Lettura dallo scaler

  // i va da zero a un numero alto che non dovrebbe essere raggiunto

  for (i = 1; i < (2*TIME/TIME_STAMP); i++) {
    sleep(TIME_STAMP);
    printf("%ld Scaler counts: ",TIME-i*TIME_STAMP);   // print time left
    for (j = 0; j < 3; j++) {                 // per i 12 registri dello scaler

      	k = cc32_read_long(handle,SCA_SLOT,j,0); // lettura registri e stampa
      	printf("%ld ",k);
      	fprintf(dati, "%ld ", k); 
     //		if ( k >= 851) break;        //eventuale blocco a tot. conteggi
    }
    if (i*TIME_STAMP >= TIME) break;  //se si è superato TIME si esce
    printf("\n");
    fprintf(dati, "\n");

  }

  //stampa dei risultati

  fprintf(dati," \nTempo di aquisizione: %ld secondi",i*TIME_STAMP);
  printf("Tempo di aquisizione: %ld secondi",i*TIME_STAMP);
  printf("\n");


  //chiude il canale di comunicazione e il file
  cc32_close(handle);
  printf("%s: close done.\n", cszPrgName);
  fclose(dati);

  return 0;
}
