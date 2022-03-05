#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    srand((int)time(NULL));
    for(;;)
    {   
        
        
        printf("%c",rand()% 28 + 97 );
        sleep (1);
    }
  
  system("PAUSE");	
  return 0;
}
