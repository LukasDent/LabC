#include "header.h"
#include <sys/types.h> /* For portability */
#include <sys/msg.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#define KEY 12345

struct responseMsg { // Risposta server --> client
 long mtype; // PID client
 char data[30]; // contenuto del file richiesto
};

struct requestMsg { // Richiesta (client --> server)
 long mtype; // inutilizzato!
 int clientId; // ID della coda del client!
 char pathname[30];  
}; // F

int main(int argc, char** argv) {
  int serverId;
  int clientId;
  struct requestMsg richiesta;;
  struct responseMsg risposta;
  
  
  /*----------------------------------------------------*/
  /*Cerco di collegarmi alla coda di messaggi del server*/
  /*----------------------------------------------------*/
  
   if((serverId = msgget(KEY,0660))<0){
	printf("Errore di connesione al server \t%s\n",strerror(errno)); 
	exit(1);
    }
 /* ---------------------------------------------------------*/ 
 /*Sono connesso alla coda messaggi, setto la struttura dati*/
 /* --------------------------------------------------------*/
 
  richiesta.mtype=1;
  richiesta.clientId=getpid();
  strcpy(richiesta.pathname,"file");
  
  /*------------------------------------------*/
  /*Invio la richiesta al server con la MSGSND*/
  /*------------------------------------------*/
  
 if((msgsnd(serverId,&richiesta ,sizeof(richiesta),IPC_NOWAIT))<0){
    printf("Errore invio richiesta al server \t%s\n",strerror(errno));
    exit(1);
 }
 
 /*-----------------------------------------------------------*/
 /*Creo la coda messaggi per la risposta e mi metto in ascolto*/
 /*-----------------------------------------------------------*/
 
 if((clientId=msgget(getpid(),0660|IPC_CREAT))<0){
   printf("Errore creazione coda messaggi client\t%s\n",strerror(errno));
   exit(1);
 }
 
 /*----------------------------------------------------------------------------*/
 /*Leggo la risposta del server finchè non ricevo il segnale di fine messaggio*/
 /*--------------------------------------------------------------------------*/
 
 while(strcmp("END_MESSAGE_STRING",risposta.data)!=0){
   
      /*-----------------------------------------*/
      /*Se non riesco a leggere termino il client*/
      /*-----------------------------------------*/
      
      if((msgrcv(clientId,&risposta,sizeof(risposta),0,0 ))<0){
	  printf("Errore ricezione messaggio dal server \t%s\n",strerror(errno));
	}
	
      /*------------------------------------*/
      /*Stampo a video la risposta ottenuta */
      /*------------------------------------*/
      
      printf("%s\n",risposta.data);
 }
}