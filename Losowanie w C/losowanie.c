#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/*
char losowanie(char *c)
{
    int i,j, liczba;
    int liczb[45];
    char ch[45][6]={{"A VS B"},{"A vs C"},{"A vs D"},{"A vs F"},{"A vs E"},{"A vs G"},{"A vs H"},{"A vs I"},{"A vs J"},{"B vs C"},
                    {"B vs D"},{"B vs E"},{"B vs F"},{"B vs G"},{"B vs H"},{"B vs I"},{"B vs J"},{"C vs D"},{"C vs E"},{"C vs F"},
                    {"C vs G"},{"C vs H"},{"C vs I"},{"C vs J"},{"D vs E"},{"D vs F"},{"D vs G"},{"D vs H"},{"D vs I"},{"D vs J"},
                    {"E vs F"},{"E vs G"},{"E vs H"},{"E vs I"},{"E vs J"},{"F vs G"},{"F vs H"},{"F vs I"},{"F vs J"},{"G vs H"},
                    {"G vs I"},{"G vs J"},{"H vs I"},{"H vs J"},{"I vs J"}};
                    
    
    
    for(i=0;i<44;i++)
    {
        liczb[i]=rand()%44;
        for(j=0;j<44;j++)
        {
            while (liczb[i]==liczb[j])
            {
                liczb[i]=rand()%44;
                j=0;
            
            }
        }       
            
      
    } 
    for(i=0;i<44;i++)
    {
        for(j=0;j<5;j++)
        {
            c[i][j]=ch[i][j];   
        }   
    }
        return c;
    
}
*/


int main(int argc, char *argv[])
{
    int i,j, liczba, zarodek;
    char c[45][6];
    int liczb[45]={};
    char ch[45][6]={{"A VS B"},{"A vs C"},{"A vs D"},{"A vs F"},{"A vs E"},{"A vs G"},{"A vs H"},{"A vs I"},{"A vs J"},{"B vs C"},
                    {"B vs D"},{"B vs E"},{"B vs F"},{"B vs G"},{"B vs H"},{"B vs I"},{"B vs J"},{"C vs D"},{"C vs E"},{"C vs F"},
                    {"C vs G"},{"C vs H"},{"C vs I"},{"C vs J"},{"D vs E"},{"D vs F"},{"D vs G"},{"D vs H"},{"D vs I"},{"D vs J"},
                    {"E vs F"},{"E vs G"},{"E vs H"},{"E vs I"},{"E vs J"},{"F vs G"},{"F vs H"},{"F vs I"},{"F vs J"},{"G vs H"},
                    {"G vs I"},{"G vs J"},{"H vs I"},{"H vs J"},{"I vs J"}};
                    
    
    
    for(i=0;i<40;i++)
    {
        sleep(657);
        
        for(j=0;j<i;j++)
        {
            
            
            if (i>0)
            if (liczb[i]==liczb[j])
            {
                //sleep(1000);
                srand(time(0)); 
                liczb[i]=(rand()%45);
                j=0;
                                          
            }
            
        }
        printf("%d %d\n",liczb[i],liczb[j]);       
            
      
    } 
    for (i=0;i<40;i++)
    {
        for (j=0;j<45;j++)
            if (liczb[i]!=j)
                war=1;
            else   
    }
    for(i=0;i<45;i++)
    {
        for(j=0;j<6;j++)
        {
            c[i][j]=ch[i][j];   
        }   
    }
    
    
    printf("===============================================\n");
    printf("=   Losowanie grup na Tournament Champions    =\n");
    printf("=             IGI 2 Covert Strike             =\n");
    printf("=                 by Mario PL                 =\n");
    printf("===============================================\n\n\n");
    
    printf("Nacisnij ENTER ¿eby rozlosowaæ grupy\n");
    for(i=0;i<45;i++)
    {
        for(j=0;j<6;j++)
        {
            
            printf("%c",c[liczb[i]][j]);
        }
        printf("\n");
          
    
    }
   
  system("PAUSE");	
  return 0;
}
