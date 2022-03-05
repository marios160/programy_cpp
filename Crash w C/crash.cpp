#include <stdio.h>
#include <string.h>

int main()
{
int i;
char linia[100],wynik[100]="wine bugs.exe 2 ";

    FILE *strumien;
//--------------------------------------------------------------
//                 pobieranie linii z pliku

for(;;)
{
    if ( (strumien = fopen( "crash.txt" , " rt " ) ) != NULL ) //jesli plik ist$
    { 
        for(  i = 0; ; i++ )                //petla cisnie do$
        { 
            
                                
                fgets(linia,strlen(linia),strumien);
                strcat(wynik,linia);    
                //system(wynik);
          if (i=feof(strumien))
            i=0
        } 
        fclose( strumien ); 
    }  
    else
        printf("Nie ma pliku \"crash.txt\"\n"); 
        fclose( strumien );
}

return 0;
}
   
