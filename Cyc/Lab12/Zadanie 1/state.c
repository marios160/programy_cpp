#include <stdio.h>
#include <stdlib.h>

#include "state.h"

void echo(char znak){
    printf("%c",znak);
}
void write(char znak){
   FILE * fptr = fopen("output.txt", "a");
    fprintf(fptr, "%c" ,znak);
    fclose (fptr);
}
