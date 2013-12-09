#include "header.h"
#include <sys/types.h> /* For portability */
#include <sys/msg.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
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
 struct requestMsg richiesta;
 struct responseMsg risposta;
 int clientId;
FILE *fp;
 // creazione della coda server server message queue!
 if((serverId = msgget(KEY,IPC_CREAT|0660))==-1){
   printf("MSGGET");
   exit(1);
 }
 printf("%d\n",serverId);
 // lettura richieste e gestione di ciascuna !
 // in un processo figlio separato !
 for(;;) {
  printf("Attesa msg\n");
 switch((msgrcv(serverId,&richiesta,sizeof(richiesta),0,0 ))){ 
case -1: //printf("Errore nella gestione del messaggio");
	  break;
 default: 
	  switch(fork()){
	    case -1: printf("Errore nella generazione del figlio");
		      break;
	    case 0:	      
		     /*Mi collego alal coda messaggi del client*/
		      clientId=msgget(richiesta.clientId,0660);
		     /*apro il file richiesto in lettura e leggo il suo contenuto*/
		     
		     fp=fopen(richiesta.pathname,"r");
		     if(!fp){
		       printf("Impossibile aprire il file richiesto dal client in lettura");
			break;}
			int ct=1;
			while(!feof(fp)){
			  fgets(risposta.data,100,fp);
			  /*invio il contenuto del file al client*/
			  if((msgsnd(clientId,&risposta,sizeof(risposta),IPC_NOWAIT))==-1)
			      printf("%s\n",strerror(errno));
			      printf("Invio n%d\n",ct);
			      ct++;
			}
			strcpy(risposta.data,"END_MESSAGE_STRING");
			msgsnd(clientId,&risposta,sizeof(risposta),IPC_NOWAIT);
			    break;
		    
	    default:	break;
	  }
 }
 }
}
