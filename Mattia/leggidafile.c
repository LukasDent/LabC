#include <stdlib.h>
#include <stdio.h>

char** leggi_lista(FILE* file_in,int* nof_elements);
void stampa_lista(char** mio_ar, int n_elems);

void main(){
FILE* fp;
int elements;
if((fp=fopen("lista_nomi.txt","r"))==NULL){
printf("Errore \n");
}
else
{
printf("File aperto inizio lettura \n");
leggi_lista(fp,&elements);
}
fclose(fp);

}
char** leggi_lista(FILE* file_in,int* nof_elements){
 char buf;
 int righe=0;
 int i=0;

char** nomi= (char**) malloc( sizeof(char*) * 0 );;
while(buf!=EOF){
	i++;
    realloc(*(nomi[righe]), sizeof(char));
    buf=getc(file_in);
    
 
    if(buf=='\n' || buf==EOF)
	righe++;
}
printf("%d \n",righe);




}



