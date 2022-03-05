#include <iostream>
#include <winsock.h>
#include <wininet.h>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <process.h>
#include <Winuser.h>
#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <conio.h>
#include <iomanip>

using namespace std;
int random;
bool endrand = false;
string user, computer, sciezka;
int limit, aktlimit;

void __cdecl Random( void * Argc)
{
	while(!endrand)
	{
		if(random > 2000000) random = random - 2000000;
		random += clock() % 25424355;
	} 
	endrand = false;
	_endthread();
}

void losuj()
{
	_beginthread( Random, 0, NULL );
	Sleep(20);
	endrand = true;
}

void losuj(int value)
{
	_beginthread( Random, 0, NULL );
	Sleep(20);
}

int ftoi(double value)
{
	value += 0.5;
	int ret;
	ret = value;
	return ret;
}

string itos(int value)
{
	string ret;
	while(value != 0)
	{
		ret += value%10 + 48;
		value /= 10;
	}
	string ret2;
	for(int i=ret.length()-1; i >= 0; i--)
	{
		ret2 += ret[i];
	}
	return(ret2);
}

string addzeros(string txt, int length)
{
	string ret;
	for(int i=txt.length(); i < length; i++) ret += "0";
	ret += txt;
	return(ret);
}

string dekoduj(string txt)
{
	string ret;
	string buf;
	buf += txt[0];
	buf += txt[1];
	buf += txt[2];
	
	char c;
	double d;
	double rand; 
	rand = atoi(buf.c_str());
	rand /= 100;
	
	for(int i=3; i < txt.length(); i+=3)
	{
		buf = "";
		buf += txt[i+0];
		buf += txt[i+1];
		buf += txt[i+2];
		
		d = atoi(buf.c_str()) / rand;
		c = ftoi(d);
		ret += c;
	}
	return ret;
}


//---------------------------------------------------------------------
/* pobiera trasc strony podanej w parametrze*/

bool pobierzWWW(string &adr, string strona)
{
      WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        return false ;
    }
    SOCKET Socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    struct hostent *host;
    host = gethostbyname("igi2.xaa.pl");
    SOCKADDR_IN SockAddr;
    SockAddr.sin_port=htons(80);
    SockAddr.sin_family=AF_INET;
    SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
    if(connect(Socket,(SOCKADDR*)(&SockAddr),sizeof(SockAddr)) != 0){
       
        return false ;
    }
    
    string request="GET /"+strona+" HTTP/1.1\r\n"
    "Host: igi2.xaa.pl\r\nConnection: close\r\n\r\n";
    send(Socket,request.c_str(), strlen(request.c_str()),0);
    char buffer[10000];
    int nDataLength;
    recv(Socket,buffer,10000,0);

    recv(Socket,buffer,10000,0);

	string buf(buffer);
	adr=buf.substr(buf.find("|||||")+5,buf.find(":::::")-(buf.find("|||||")+5));
   	
	closesocket(Socket);
    WSACleanup();
    
    return true;
}
//--------------------------------------------------
/*sprawdzanie username i tworzenie sciezki*/
void pathname()
{
	char *username = getenv( "USERNAME" );											//wczytujemy nazwe u¿ytkownika
    string nazwa(username);
    for(int j=0; j < nazwa.length(); j++)
	{
		if (nazwa[j] == ' ')					 								//jesli nazwa uzytkownika ma spacje
			nazwa[j] = '_';														//to zmieniamy je na pod³oge
	}
	user = nazwa;
	char *user2 = getenv( "COMPUTERNAME" );										//wczytujemy nazwe u¿ytkownika
    string nazwa2(user2);
	computer= nazwa2;
	while(!pobierzWWW(sciezka,"incognito/"+computer+".txt"));				//pobieramy sciezke
	sciezka = dekoduj(sciezka);
	sciezka+=user;

}
//-----------------------------------------------------
/*sprawdzanie poprawnosci tresci pliku*/
bool checkconf ()
{
	string linia;
	ifstream plik(sciezka.c_str());
	if (!plik)
	{	
		return 0;																//blad, brak pliku!
	}		
	getline(plik,linia);
	linia=dekoduj(linia);
	if (linia.find("+_)(*&^%$#@!~") == 0 )										//i pod nia linie usera
	{
		getline(plik,linia);
		linia=dekoduj(linia);
		if (linia.find("@"+user) == 0 )										//i pod nia linie usera
		{
			getline(plik,linia);
			linia=dekoduj(linia);
			if (linia.find("*") == 0 )										//i pod nia linie usera
			{
				getline(plik,linia);
				linia=dekoduj(linia);
				if (linia.find("*") == 0 )										//i pod nia linie usera
				{
					getline(plik,linia);
					linia=dekoduj(linia);
					if (linia.find("*") == 0 )										//i pod nia linie usera
					{
						return 1;
					}
				}
			}
		}
	}		
	return 0;																	//ale plik jest dobry
}
int wczytaj()
{
	string linia;
	int l=0;
	if(!checkconf())								//sprawdzenie pliku
	{
		cout <<"Bledny plik konfiguracyjny"<<endl;
		return 0;
	}
	ifstream plik(sciezka.c_str());

		getline(plik,linia);
		getline(plik,linia);
		getline(plik,linia);
		linia = dekoduj(linia);
		limit = atoi(linia.substr(1).c_str());												//ustawiamy limit - w minutach
		getline(plik,linia);
		linia = dekoduj(linia);
		aktlimit = atoi(linia.substr(1).c_str());
	
	plik.close();
	return 1;	
}

int main() {
	
	int godzin, minut;
	SetConsoleTitle("Look Limit");
	cout <<"Czekaj..."<<endl;
	pathname();
	while(1)
	{
		if(!wczytaj())
			cout<<"Bledny plik konfiguracyjny"<<endl;					//pobieramy dane
		else
		{
			system("cls");
			cout << "\n\n\n\n\n\n\n\n" << endl;
			cout << setw(43) << "Uzytkownik: " << user << "\n" <<endl;
			cout << setw(39) << "Limit: " << limit/60 << " godz.\n" << endl;
			godzin = aktlimit / 3600;
			minut = (aktlimit % 3600)/60;
			cout << setw(36) << "Zostalo: " << godzin <<" godz. i " << minut << " min." << endl; 
		}
		Sleep(60000);	
	}
	return 0;
}
