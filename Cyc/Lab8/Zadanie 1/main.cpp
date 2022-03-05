#include <iostream>

using namespace std;

int main()
{
    int stos1=1;
    int* sterta1=new int(1);
    int stos2=1;
    int* sterta2=new int(1);
    int stos3=1;
    int* sterta3=new int(1);
    int stos4=1;
    int* sterta4=new int(1);
    cout  << &stos1  <<endl;
    cout  << &stos2  <<endl;
    cout  << &stos3  <<endl;
    cout  << &stos4  <<endl;

    cout << endl;




    cout<<&(*sterta1)<<endl;
    cout<<&(*sterta2)<<endl;
    cout<<&(*sterta3)<<endl;
    cout<<&(*sterta4)<<endl;

    return 0;

/*
0x22fe2c
0x22fe28
0x22fe24
0x22fe20  - adresy stosu

0x46a670
0x46a690
0x46a9e0
0x46aa00-  adresy sterty

Z obserwacji wynika, ¿e stos alokuje siê "wy¿ej" w pamiêci ni¿ sterta.
Na³o¿enie siê na siebie obu segmentów jest teoretycznie niemo¿liwe, poniewa¿ stos alokuje siê coraz wy¿ej, a sterta coraz ni¿ej.
*/

}
