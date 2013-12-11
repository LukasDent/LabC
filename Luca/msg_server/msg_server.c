//#include "header.h"
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
 
 
 /*---------------------------------*/
 /*Creo la coda messaggi del server*/
 /* ------------------------------*/
 
 if((serverId = msgget(KEY,IPC_CREAT|0660))==-1){
   printf("Errore msgget server \t%s\n",strerror(errno));
   exit(1);
 }
 
 
 
 /*-----------------------------------------------------*/
 /*Entro in loop per attendere le richieste dei client */
 /* --------------------------------------------------*/
 

 for(;;) {
   
    printf("Attesa msg\n");
    switch((msgrcv(serverId,&richiesta,sizeof(richiesta),0,0 ))){ 
	  case -1: printf("Errore msgrcv server\t%s\n",strerror(errno));
		   break;
		   
	  default: 
	    
		  /*------------------------------------------------*/
		  /*Genero il figlio per la gestione della richiesta*/
		  /*------------------------------------------------*/
		  
		  switch(fork()){
		    
		    case -1: printf("Errore Fork:\t%s\n",strerror(errno));
					   break;
				  
				  case 0: /*----------------------------------------*/	      
					  /*Mi collego alal coda messaggi del client*/
					  /*----------------------------------------*/
					  
					  /*-----------------------------------------------------------------------------*/
					  /*Se fallisce la connesione alla cosa messaggi del client annullo la richiesta */
					  /*----------------------------------------------------------------------------*/
					  
					  if((clientId=msgget(richiesta.clientId,0660))==-1){
					      printf("Errore msgget client \t%s\n",strerror(errno));
					      break;
					  }
					  
					  
					  /*----------------------------------------------------------*/
					  /*apro il file richiesto in lettura e leggo il suo contenuto*/
					  /*----------------------------------------------------------*/
					  
					  fp=fopen(richiesta.pathname,"r");
					  
					  /*----------------------------------------------------------------*/
					  /*Se fallisce l'apertura del file annullo la richiesta del client */
					  /*---------------------------------------------------------------*/
					  
					  if(!fp){
						  printf("Errore fopen \t%s\n",strerror(errno));
						   break;
					  }
					  
					  /*-----------------------------------------------*/
					  /*Inizio la lettura del file richiesto dal client*/
					  /*-----------------------------------------------*/
					  
					  int ct=1;/*contatore per indicare numero di letture effettuate sul file*/
					  
					   while(!feof(fp)){
					     
					    /*------------------------------------------------------------------------------------------------------*/
					    /* Prendo in lettura con la fgets tanti caratteri quando è grande il campo data della struttura risposta*/
					    /*------------------------------------------------------------------------------------------------------*/
					    
					    fgets(risposta.data,100,fp);
					    
					    /*-------------------------------------*/
					    /*invio il contenuto del file al client*/
					    /*-------------------------------------*/
					    
					    if((msgsnd(clientId,&risposta,sizeof(risposta),IPC_NOWAIT))==-1)
					      printf("Errore msgsnd client \t%s\n",strerror(errno));
					    
					    
					    printf("Invio numero:\t%d\n",ct);
						  ct++;
					  
					     
					  }
					  /*-------------------------------------------*/
					  /*Invio il messagio di fine lettura al client*/
					  /*-------------------------------------------*/
					  strcpy(risposta.data,"END_MESSAGE_STRING");
					  if(msgsnd(clientId,&risposta,sizeof(risposta),IPC_NOWAIT)==-1)
					    printf("Errore invio END_STRING \t%s\n",strerror(errno));
					  break;
		    
	    default:	/*Padre si rimette in attesa di altre richieste*/ break;
	  }
 }
 }
}

