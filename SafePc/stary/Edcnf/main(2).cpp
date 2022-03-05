#include <cstdlib>
#include <iostream>
#include <ctime>
#include <sstream>
#include <cstdio>
#include <conio.h>
#include <string>
#include <windows.h>
#include <fstream>
#include "stdafx.h"
#include <wininet.h>
#pragma comment (lib, "wininet.lib")



using namespace std;

string *tab,sciezka;															//zmienne globalne
int rozm,lusr,*tabpoz;
bool FTP=0;

//---------------------------------------------------
/* konwertuje INT na STRING*/
string intconv(double value)
{
	ostringstream ss ;
	ss << value;
	return ss.str();
}

//---------------------------------------------------
/* konwertuje STRING na INT*/
double strconv(string value)
{
	double i;
	istringstream iss(value);
	iss >> i;
	return i; 
}


//---------------------------------------------------
/* losuje string do zakodowania*/

string losuj()
{	
	string kod="";																
	for (int i=0;i<20;i++)
	{
		kod+=char((rand()%94)+33);												//petla dodaje do stringa losowy znak
	}
	return kod;
}

//---------------------------------------------------
/*koduje string*/
string code(string linia)
{
	int i=0;									
	do																			//w petli zmieniamy znaki
	{
		linia[i]=linia[i]+97;													//na 96 do przodu w tablicy ascii
		i++;
	}while(i<linia.length());													//dopoki nie dojdziemy do konca stringa
	linia=losuj()+linia+losuj()+linia+losuj();									//dodajemy kodzik zeby wiecej szyfru bylo
	return linia;
}


//---------------------------------------------------
/* rozkodowuje string*/

string decode(string linia)
{
	int i=0;
	string delinia = linia.substr(20,(linia.length()-60)/2);					//wyciagamy naszego stringa z kodu
	do
	{
		delinia[i]=delinia[i]-97;												//zmieniamy na 96 do tylu z tablicy ascii
		i++;
	}while(i<delinia.length());													//dopoki nie dojdziemy do konca stringa
	return delinia;
}
//--------------------------------------------------------
/*funkcja zmienia sekundy na minuty*/
string minuty(string linia)
{
	double liczba;
	liczba = strconv(linia);
	liczba=liczba/60;
	
	return intconv(liczba);
}
//-------------------------------------------------------
/*funkcja zmienia minuty na sekundy*/
string sekundy(string linia)
{
	int liczba;
	liczba = strconv(linia);
	liczba=liczba*60;
	
	return intconv(liczba);
}
//---------------------------------------------------
/*funkcja okreslajaca sciezke do pliku*/
string path()
{
	int i=1,rozm=0 ,wybor;
	string linia,sciezka="";
	string *sciezki;													
	
	ifstream plik("conf");
	while(!plik.eof())														//otwieramy plik konfiguracyjny
	{
		getline(plik,linia);
		rozm++;
	}
	plik.close();
	sciezki = new string [rozm];
	sciezki[0]="cnf";
	plik.open ("conf");
	getline(plik,linia);														//pierwsza linia to haslo
	while(!plik.eof())
	{	
		getline(plik,linia);													//kolejne linie zapisujemy do tablicy
		sciezki[i] = decode(linia);												//dekodujac tresc
		i++;															
	}
	system("cls");
	cout << "Wybierz sciazke:" << endl;											//wybieramy ktora sciezka nas interesuje
	cout << "0. Aktualny katalog" << endl;
	for (int j=1;j<i;j++)
		cout << j << ". " << sciezki[j] << endl;	
	cin >> wybor;
	if (sciezki[wybor].find("FTP") == 0)										//jesli wybierzemy sciezke z ftp
	{
		sciezka="/SafePc/"+sciezki[wybor].substr(3);										//trzeba wyciac nazwe pliku
		FTP = 1;																//oraz poinformowac o pobieraniu z ftp
	}
	else
	{
		sciezka=sciezki[wybor];													//zapisujemy wybrana sciezke
		FTP = 0;
	}
	delete [] sciezki;															//kasujemy tablice
	return sciezka;																//zwracamy gotowa sciezke
}
//---------------------------------------------------------
/*oblicza rozmiar tablicy*/
void tablice()
{
	string linia;
	rozm=0;
	ifstream plik(sciezka.c_str());												//otwieramy plik konfiguracyjny
    if (!plik)																	//jesli go nie ma
 		cout << "Nie ma pliku cnf.dll!" << endl;									
	while (!plik.eof())
	{
		getline(plik,linia);													//pobieramy linie z pliku
		rozm++;
	}
	plik.close();
}
//---------------------------------------------------
/*Pobieranie z ftp*/
int PobierzPlikFTP(string remoteFile,string newFile){//nazwa na serwerze potem nazwa na dysku

  	  char ftp[] = "cba.pl" ;
      char user[] = "admin@blastshack.y0.pl";
      char password[] = "rower160";

    HINTERNET hInternet;

    HINTERNET hFtpSession;     

    if(InternetAttemptConnect(0) == ERROR_SUCCESS)
    {    
    	cout << "Pobieranie pliku!"<<endl;
    }
    else 
	{
		cout << "Brak polaczenia z internetem!" << endl;
		getch();
		return 0;
	}
 

    hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL,0);

    if(hInternet != NULL){     

        hFtpSession = InternetConnect(hInternet, ftp, INTERNET_DEFAULT_FTP_PORT, user, password, INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);

        if(hFtpSession != NULL){
				
           if(FtpGetFile(hFtpSession, remoteFile.c_str(), newFile.c_str() ,false, FILE_ATTRIBUTE_NORMAL, INTERNET_FLAG_TRANSFER_BINARY,0)) {     
                    
                InternetCloseHandle(hFtpSession);

                InternetCloseHandle(hInternet);
				cout << "Plik zostal Pobrany!" << endl;
                }
            else {                                
                cout << "Blad podczas pobierania pliku!" << endl;
                getch();
                return 0;
            }    
            

        }
        else return 0;
        
    }

    else  return 0;


    return 0;

}

//--------------------------------------------------------
/*wysylanie  na ftp*/

int WyslijPlikFTP(string localFile, string remoteFile){

    char ftp[]      = "cba.pl";
    char user[]     = "admin@blastshack.y0.pl";
    char password[] = "rower160";
    
  
    HINTERNET hInternet;

    HINTERNET hFtpSession;     

    if(InternetAttemptConnect(0) == ERROR_SUCCESS) 
		cout<<"Wysylanie pliku!"<<endl;

    else 
	{
		cout << "Brak polaczenia z internetem!" << endl;
		getch();
		return 0;
	}
 

    hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL,0);

    if(hInternet != NULL){     

        hFtpSession = InternetConnect(hInternet, ftp, INTERNET_DEFAULT_FTP_PORT, user, password, INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);

        if(hFtpSession != NULL){

            if(FtpPutFile(hFtpSession, localFile.c_str(), remoteFile.c_str() , FTP_TRANSFER_TYPE_BINARY,0)){                 

                InternetCloseHandle(hFtpSession);

                InternetCloseHandle(hInternet);
				cout << "Plik zostal wyslany!" << endl;
                }
            else {                                
                cout << "Blad podczasz wysylania pliku!" << endl;
                getch();
                return 0;
            }    
            

        }
        
        else 
			return 0;
        

    }

    else  
		return 0;  

    return 0;

};

//--------------------------------------------------
/* funkcja wysyla dane do pliku*/
void wyslij()
{	
	int lusr=1;
	int *tabpozp = new int [10];
	ofstream plik(sciezka.c_str(),ios::out|ios::trunc);           				//otwiera plik konfiuguracyjny
	for(int i=0;i<rozm;i++)
	{	
		
		if (tab[i].find("@")==0)												//jesli znajdziemy znak uzytkownika @
		{
			tabpozp[lusr]=i;													//zapisujemy na jakiej pozycji jest
			lusr++;																//liczba uzytkownikow
		}	
		if (i == tabpozp[lusr-1]+2)
		{
			tab[tabpozp[lusr-1]+2]=sekundy(tab[tabpozp[lusr-1]+2]);
		}	
		if (i == 0)
			plik << code(tab[i]);												//jesli pierwsza linia to bez entera
		else																	//zapobiega to bezsensownym enterom w pliku
			plik << "\n" << code(tab[i]);	
			
	}		   
    plik.close();  
    delete [] tab;
    if (FTP)
	{
		WyslijPlikFTP(sciezka,"/SafePc/"+sciezka);
		FTP=0;
	}
}

//-----------------------------------------------------
/* funkcja wczytuj¹ca dane do tablicy*/
void pobierz()
{
	string linia;
	lusr=1;
	if (FTP)
	{
		PobierzPlikFTP(sciezka,sciezka.substr(11));
	}
		
	tablice();																	//obliczamy rozmiar pliku
	tab = new string [rozm];													//tworzymy tablice
	ifstream plik(sciezka.c_str());
	if (!plik)
		cout << "Nie ma pliku cnf.dll!" << endl;
	int *tabpozp = new int [10];
	for (int i=0; i<rozm; i++)
	{
		getline(plik,linia);													//pobieramy linie z pliku
		tab[i]=decode(linia);													//dekodujemy
		if (tab[i].find("@")==0)												//jesli znajdziemy znak uzytkownika @
		{
			tabpozp[lusr]=i;													//zapisujemy na jakiej pozycji jest
			lusr++;																//liczba uzytkownikow
		}	
		if (i == tabpozp[lusr-1]+2)
		{
			tab[tabpozp[lusr-1]+2]=minuty(tab[tabpozp[lusr-1]+2]);
		}	
		if (lusr >= 10)															//jak za duzo uzytkownikow to wychodzimy
		{
			cout << "Za duzo uzytkownikow" << endl;
			break;
		}
	}
	
	
	tabpoz = new int [lusr];													//tworzymy odpowiednia tablice
	for (int i=0;i<lusr;i++)													//przepisujemy
		tabpoz[i]=tabpozp[i];
	delete [] tabpozp;															//kasujemy duza tablice
	tabpoz[lusr]=rozm;															//rozm to ostatni element tablicy
	plik.close();

}


//--------------------------------------------------------
/* funkcja edytuje aplikacje wyznaczone do zamykania*/

void edytuj_apk(int usr)
{
	ofstream plik("tmp",ios::out|ios::trunc);
	int ileb=0;
	for (int i=tabpoz[usr]+7; i<tabpoz[usr+1]-1; i++)							//wpisujemy do pliku aplikacje
	{
		if (i == tabpoz[usr]+7)
			plik << tab[i];														//jesli pierwsza linia to bez entera
		else																	//zapobiega to bezsensownym enterom w pliku
			plik << "\n" << tab[i];
		ileb++;
	}
	plik.close();
	system("cls");
	cout << "Po nacisnieciu ENTERA program uruchomi\n"
		<< "notatnik z lista aplikacji. Nalezy zedytowac\n"
		<< "odpowiednio liste a nastepnie zapisac i zamknac\n\n";
	system("pause");
	
	system("notepad.exe tmp");													//otwieramy w notatniku plik
	system("cls");
	cout << "Jesli skonczyles edycje zapisz i zamknij plik" << endl;
	cout << "Nacisnij ENTER" << endl;
	string linia;
	ifstream plik2("tmp");
	int ile=0;
	while (!plik2.eof())
	{
		getline(plik2,linia);													//czytamy zmiany w aplikacjach
		if (!linia.empty())
			ile++;																//liczymy ile jest aplikacji
	}
	plik2.close();
	
	int i;
	string *tabtmp;
	rozm+=(ile-ileb);															//zmieniamy rozmiar
	tabtmp = new string [rozm];													//tymczasowa wieksza tablica
	for (i=0; i<tabpoz[usr]+7;i++)
		tabtmp[i]=tab[i];														//przepisujemy od poczatku do aplikacji


	plik2.open("tmp");	
	for(int j=0;j<ile;j++,i++)
	{
		getline(plik2,linia);													//z pliku przepisujemy  nowe aplikacje
		//if (!linia.empty())
			tabtmp[i]=linia;
	}
	plik2.close();
	for(int j=tabpoz[usr+1]-1;i<rozm;j++,i++)									//przepisujemy reszte tablicy
		tabtmp[i]=tab[j];

	delete [] tab;																//kasujemy stara za mala tablice
	tab = new string[rozm];														//robimy wieksza z odpowiednim rozmiarem
	for(int i=0;i<rozm;i++)
		tab[i]=tabtmp[i];														//przepisujemy

	delete [] tabtmp;															//kasujemy tymczasowa
	remove("tmp");																//usuwamy plik tymczasowy
}

//---------------------------------------------------------
/*Funkcja wyswietlajaca dane na ekran*/
void wyswietl_dane()
{
	system("cls");
	for(int i=0; i<rozm ; i++)
		cout << tab[i] << endl;													//wyswietlamy dane
	cout << endl;
	system("pause");	
}
//----------------------------------------------------------
/*Funkcja zmieniaj¹ca dane  pliku*/
int zmien_dane()
{
int usr;
do
{
	system("cls");
	cout << "Wybierz uzytkownika: " <<endl;
	for (int i=1 ; i<lusr ; i++)
		cout << i << ". " << tab[tabpoz[i]] << endl;
	cout << "0. Zakoncz" << endl;
	
	cin >> usr;	
	if (usr==0)
		break;
	int wybor;
	do
	{
		system("cls");
		cout << "Wybierz co chcesz zmienic: \n"
			<< "1. Limit: " << tab[tabpoz[usr]+1] << "\n"
			<< "2. Aktualny limit: " << tab[tabpoz[usr]+2] << "\n"
			<< "3. Date: " << tab[tabpoz[usr]+3] << "\n"
			<< "4. Aktualizacja: " << tab[tabpoz[usr]+4] << "\n"
			<< "5. Process: " << tab[tabpoz[usr]+5] << "\n"
			<< "6. Aplikacje" <<  "\n"
			<< "7. Powrot do wyboru uzytkownikow"  << "\n";
		cout << "0. Zakoncz zmiany" << endl;
	
		
		string data;
		
		cin >> wybor;
		switch (wybor)
		{
			case 0:
				return 0;
			case 1:
				cout << "Podaj nowy limit w minutach:" << endl;
				cout << ">> ";
				cin >> tab[tabpoz[usr]+1];
				break;
			case 2:
				cout << "Podaj nowy aktualny limit w minutach:" << endl;
				cout << ">> ";
				cin >> tab[tabpoz[usr]+2];
				break;
			case 3:
				cout << "Podaj nowa date" << endl;
				cout << "Podaj rok: ";
				cin >> tab[tabpoz[usr]+3];
				cout << "Podaj miesiac: ";
				cin >> data;
				tab[tabpoz[usr]+3]+="_"+data;
				cout << "Podaj dzien: ";
				cin >> data;
				tab[tabpoz[usr]+3]+="_"+data;
				break;
			case 4:
				cout << "Wgrac nowa aktualizacje? (1/0)" << endl;
				cout << ">> ";
				cin >> tab[tabpoz[usr]+4];
				break;
			case 5:
				cout << "Wlaczyc kontrole procesow? (1/0)" << endl;
				cout << ">> ";
				cin >> tab[tabpoz[usr]+5];
				break;
			case 6:
				edytuj_apk(usr);
				break;
			case 7:
				break;
			default:
				cout << "Nie ma takiej opcji!" << endl;
		}
	
	}while(wybor != 7);
	
}while(usr != 0);
return 0;
}
//--------------------------------------------------------------------
/*funkcja tworzy nowy plik */
void utworz_plik()
{
	system("cls");
	cout << "Podaj nazwe nowego pliku (domyslnie cnf.dll)" << endl;
	cout << ">> ";
	string nazwa;
	cin >> nazwa;
	cout << "Ilu uzytkownikow?" << endl;
	cout << ">> ";
	int ilu;
	cin >> ilu;
	string *tabusr = new string [ilu];
	cout << "Podaj nazwy uzytkownikow" << endl;
	for (int i=0;i<ilu;i++)
	{	
		cout << ">> ";
		cin >> tabusr[i];
	}
	tab = new string [ilu*9];
	int j=0;
	for(int i=0;i<ilu;i++)
	{		
			tab[j]="!@#$%^&*()";
			j++;
			tab[j]="@"+tabusr[i];
			j++;
			tab[j]="5";
			j++;
			tab[j]="0";
			j++;
			tab[j]="2014_06_30";
			j++;
			tab[j]="0";
			j++;
			tab[j]="0";
			j++;
			tab[j]="startapk_"+tabusr[i];
			j++;
			tab[j]="endapk_"+tabusr[i];
			j++;
	}
	ofstream plik(nazwa.c_str(),ios::out|ios::trunc);           					//otwiera plik konfiuguracyjny
	for(int i=0;i<ilu*8;i++)
	{
		if (i == 0)
			plik << code(tab[i]);												//jesli pierwsza linia to bez entera
		else																	//zapobiega to bezsensownym enterom w pliku
			plik << "\n" << code(tab[i]);			
	}		   
    plik.close();  
	delete [] tab;
}



//####################################################################

//####################################################################
int main(int argc, char *argv[]) {
	
	string linia,passwd="";
	char znak;
	
	
	ifstream plik("passwd");													//sprawdzamy czy jest odpowiedni plik
	if (!plik)
		return 0;
	getline(plik,linia);															//wczytujemy haslo
	plik.close();
	rename("passwd","conf");
	linia=decode(linia);
	while(true) { 
                znak = getch(); 
                if(znak == '\n' || znak == '\r' || znak == -1) 
                        break; 

                passwd += znak; 
                putch('*'); 
        } 														//dekodujemy																//pobeiramy od uzytkownika haslo
	system("cls");
	if (passwd != linia)
		return 0;															//jak niepoprawne to wychodzimy
	int wybor;
do
{
	system("cls");
	cout << "Wybierz co chcesz zrobic:\n"
		<< "1. Czytaj dane\n"
		<< "2. Zmien dane\n"
		<< "3. Utworz domyslny plik\n"
		<< "0. Zakoncz\n"
		<< ">> ";
	
	cin >> wybor;
	if (wybor == 1 || wybor == 2)
		sciezka = path();
	system("cls");	
	switch (wybor)
	{
		case 0:
			break;
		case 1:
			cout << "Trwa pobieranie pliku. Czekaj..." << endl;
			pobierz();
			wyswietl_dane();
			delete [] tab;
			break;
		case 2:
			pobierz();
			zmien_dane();
			wyslij();	
			break;
		case 3:
			utworz_plik();
			break;
		default:
			cout << "Nie ma takiej opcji" <<endl;
			break;	
	}
			
}while (wybor != 0 );

	
	return 0;
}
