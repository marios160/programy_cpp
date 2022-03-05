#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;
void printTab(int * tab, unsigned int size){
    for(int i=0; i<size;i++){
        cout<<tab[i]<<endl;
    }
    cout<<endl;
};

void reverseTab(int * tab, unsigned int size){
    int tmpTab[size];
    for(int i=0,j=size-1;i<size;i++,j--){
        tmpTab[i]=tab[j];
    }
    for(int i=0; i<size;i++)
        tab[i]=tmpTab[i];
};

int main()
{
    srand (time(NULL));
    unsigned int rozmiar;
    cout<<"Podaj ilosc elementow w tablicy"<<endl;
    cin>>rozmiar;
    int tablica[rozmiar];
    for(int i=0;i<rozmiar;i++){
        tablica[i]=rand()%100+1;
    }
    printTab(tablica,rozmiar);
    reverseTab(tablica,rozmiar);
    printTab(tablica,rozmiar);

    return 0;
}
