#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>

using namespace std;
void logo()
{
 cout<<"###########################################\n"
       "##      WebBlock by Mateusz Blaszczak    ##\n"
       "##       Program do blokowania stron     ##\n"
       "##                  2014                 ##\n"
       "###########################################\n\n\n";  
}

void menu()
{
    cout<<"Wybierz co chcesz zrobic:\n"
            "1. Wczytaj baze zablokowanych stron\n"
            "2. Zablokuj strone\n"
            "3. Odblokuj strone\n"
            "0. Wylacz program\n"
            ;   
}
int wczytaj_baze()
{
    string linia;
    ifstream baza("base.bzk");
    system("attrib -R C:\\Windows\\System32\\drivers\\etc");
    ofstream hosts("C:\\Windows\\System32\\drivers\\etc\\hosts",ios::out|ios::app);
    if(!baza)
    {
        cout<<"Nie ma bazy !"<<endl;
        return 0;   
    }
    if(!hosts)
    {
        cout<<"Nie ma hosts!"<<endl;
        return 0;   
    }
    while(!baza.eof())
    {
        getline(baza,linia);
        linia+="\n";
        cout<<linia<<endl;
        hosts << linia;   
    }
    baza.close();
    hosts.close();   
    cout<<"Baza zostala poprawnie wczytana!"<<endl;
    getch();
}
void dodaj_strone()
{
    string strona,linia="127.0.0.1  ";
    cout<<"Podaj adres strony do zablokowania\n"
            "bez \"http\" np. www.google.pl\n";
    cin>>strona;   
    ofstream hosts("C:\\Windows\\System32\\drivers\\etc\\hosts",ios::app);
    linia+=strona;
    linia+="\n";
    hosts << linia;
    hosts.close();
    cout<<"Strona "<<linia<<" zostala dodana!\n";
    getch();
}

void odblokuj_strone()
{
    cout<<"Zdrowas Maryjo, \n"
"laski pelna, Pan z Toba, \n"
"blogoslawionas Ty miedzy niewiastami\n"
"i blogoslawiony owoc zywota Twojego, Jezus. \n"
"Swieta Maryjo, Matko Boza,\n"
"modl sie za nami grzesznymi \n"
"teraz i w godzine smierci naszej.\n"

"   Amen.    \n\n\n";

cout<<"Zglos sie do Mateusza Blaszczaka! Albo do katechistow!"<<endl;
getch();
}

int main()
{
    int wybor;
    do
    {
    system("cls");
    logo();
    menu();
    cin>>wybor;
    switch(wybor)
    {
     case 1:
            system("cls");
            logo();
            wczytaj_baze();
            break;
     case 2:
            system("cls");
            logo();
            dodaj_strone();
            break;
     case 3:
            system("cls");
            logo();
            odblokuj_strone();
            break;
     default:
            system("cls");
            logo();
            break;   
    }
    }while (wybor != 0);
    system("PAUSE");
    return EXIT_SUCCESS;
}
