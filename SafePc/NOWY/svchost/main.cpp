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
using namespace std;

int i=1;
int random;
bool endrand = false;
string sciezka, user, computer,path;

bool pobierzWWW(string &adr, string strona);

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

string koduj(string txt)
{
	string ret;
	string buf2;
	int buf;
	char c;
	double rand;
	rand = (random%693 + 100);
	ret += addzeros(itos(rand), 3);
	rand /= 100;

	for(int i=0; i < txt.length(); i++)
	{
		c = txt[i];
		buf = ftoi(c * rand);
		buf2 = itos(buf);
		buf2 = addzeros(buf2, 3);
		ret += buf2;
	}
	
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

//---------------------------------------------------
/* konwertuje INT na STRING*/
string intconv(int value)
{
	ostringstream ss ;
	ss << value;
	return ss.str();
}

//---------------------------------------------------
/* konwertuje STRING na INT*/
int strconv(string value)
{
	int i;
	istringstream iss(value);
	iss >> i;
	return i; 
}

//------------------------------------------------
/* wlacza program w tle*/

void ukryj_okno() 
{
    HWND okno = FindWindow("ConsoleWindowClass", 0);            //uchwyt bierze to okno
    ShowWindow(okno, SW_HIDE);                                  //i wlacza w tle
}
void pokaz_okno()
{
    HWND okno = FindWindow("ConsoleWindowClass", 0);            //uchwyt bierze to okno
    ShowWindow(okno, SW_SHOW);                                  //i wlacza w tle
}
//------------------------------------------------
/*pobiera aktualy czas i zwraca godzine lub date*/

string czas(int var=0)
{
	string time;
	int i=0;
	for(i; i<3; i++)
			if(pobierzWWW(time,"incognito/h.php"))                                 				
				break;
	if(i>2)
		time = "ERROR.";
	if(var)
		return "["+time.substr(8,2)+":"+time.substr(10,2)+":"+time.substr(12,2)+"]";
	else
		return time.substr(0,8);
}
//------------------------------------------------
/*tworzy loga programu*/
int loguj(string linia)
{
	string linijka=czas(1)+"<svchost>"+linia+"\n";
	string path = sciezka+user+".log";
	ofstream log(path.c_str(),ios::out|ios::app);
	log << linijka;
	log.close();
}

//---------------------------------------------------------------------
/* pobiera trasc strony podanej w parametrze*/

bool pobierzWWW(string &adr, string strona)
{
      WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        loguj("Blad WSADATA ("+strona+")");
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
       
	   	loguj("Brak polaczenia ("+strona+")"); 
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
}

//---------------------------------------------------
int perror (int var)
{
	string path = sciezka+"dec.exe";
	switch (var)
	{
		case 1:
			loguj("perror(1): Pierwsza proba wylaczenia limitu");
			break;
		case 2:
			loguj("perror(2): Druga proba wylaczenia limitu");		
			break;			
		case 3:
			loguj("perror(3): Trzecia proba wylaczenia limitu");	
			break;										
		case 4:
			loguj("perror(4): Czwarta proba wylaczenia limitu, zerowanie limitu");			
			break;
		default:ShellExecuteA(0, 0 ,path.c_str(),			
			"30 \"Nieznany blad CP, Zawolaj Mateusza!\" 1",0,SW_SHOW);
			loguj("perror(default): Nieznany blad!");
			Beep(2000,5000);
			Sleep(30000);											
			break;
	}
}

//--------------------------------------------------
/*zabija procesy*/
int process()
{	
		string linia;
		string path2 = sciezka+"look";												///wysylamy killa do cmd 
		string cmd="tasklist /NH /FI \"IMAGENAME eq Incognito.exe\" /FI \"USERNAME eq "+user+"\" > "+path2;	//zapisuje polecenie dodajac zdekodowana linie
		string cmd2="tasklist /NH /FI \"IMAGENAME eq UpdaterInc.exe\" /FI \"USERNAME eq "+user+"\" >> "+path2;
		// FI - filtr, IMAGENAME - nazwa okna, username - nazwa uzytkownika
		system(cmd.c_str());
		system(cmd2.c_str());	
		ifstream plik(path2.c_str());
		if(!plik)
		{
			loguj("Nie ma pliku look");
			//cout << "niema"<<endl;
			//ShellExecuteA(0, 0 ,path.c_str(),0,0,SW_HIDE);
			return 0;
		}
		while(!plik.eof())
		{
			getline(plik,linia);
			if (linia.find("Incognito.exe")<linia.length())
			{
				return 1;
			}
			else if (linia.find("UpdaterInc.exe")<linia.length())
				{
					loguj("Znaleziono updater, czekam 1 min");
					Sleep(60000);
					return 1;
				}	
		}
			
		if (i==4)
		{
			perror(i);
 			ShellExecuteA(0, 0 ,path.c_str(),"0",0,SW_HIDE);
			Sleep(60000);	
		}
		else
		{
			ShellExecuteA(0, 0 ,path.c_str(),0,0,SW_HIDE);
			perror(i);
			i++;
		}		
			
		
		plik.close();
		return 0;
}

//--------------------------------------------------

int main(int argc, char** argv) 
{
	SetConsoleTitle("svchost");   												//ustawiamy nazwe okna
    ukryj_okno();	
	pathname();
	path = sciezka+"Incognito.exe";
	loguj("\n");
    loguj("====================["+czas().substr(0,2)+"-"+czas().substr(2,2)+"-"+czas().substr(4,2)+"]=======================");
    loguj("svchost uruchomiony!");  															//ukrywamy okno
	ShellExecuteA(0, 0 ,path.c_str(),0,0,SW_HIDE);
	Sleep(1000);
	while(1)
	{
		process();
		Sleep(60000);
	}
	
	
	
	return 0;
}
