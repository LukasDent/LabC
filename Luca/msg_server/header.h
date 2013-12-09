#include <sys/types.h> /* For portability */
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define KEY 1234567890



struct responseMsg { // Risposta server --> client
 long mtype; // PID client
 char data[30]; // contenuto del file richiesto
};

struct requestMsg { // Richiesta (client --> server)
 long mtype; // inutilizzato!
 int clientId; // ID della coda del client!
 char pathname[30];  
}; // File da restituire