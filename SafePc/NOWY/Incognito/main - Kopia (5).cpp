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


int random;
bool endrand = false;
string user="",computer="",sciezka,*tab,path="";
int aktlimit=0, limit=0;


void dekoduj_plik(string filename);
void koduj_plik(string filename);
string koduj(string txt);
string dekoduj(string txt);
bool pobierzWWW(string &adr, string strona);
int loguj(string linia);

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


string intconv(int value)
{
	ostringstream ss ;
	ss << value;
	return ss.str();
}

//-----------------------------------------------------
/* otwiera okienko z danym bledem */
int perror (int var)
{
	string path = sciezka+"dec.exe";
	string cmd="shutdown /l ";
	switch (var)
	{											//to wylogowujemy
		case 1: ShellExecuteA(0, 0 ,path.c_str(),			
			"60 \"Limit wykorzystany!\" 1",0,SW_SHOW);
			loguj("perror(1): Koniec limitu");
			Beep(2000,1000);
			Sleep(60000);
			loguj("Wylogowuje!");
			//system(cmd.c_str());	
			break;											//to wylogowujemy
		case 2: ShellExecuteA(0, 0 ,path.c_str(),			
			"5 \"Limit wykorzystany! Sprobuj jutro.\" 1",0,SW_SHOW);
			loguj("perror(2): Proba logowania po wykorzystanym limicie");
			Beep(2000,1000);
			Sleep(20000);
			loguj("Wylogowuje!");
			//system(cmd.c_str());		
			break;										//to wylogowujemy									//to wylogowujemy												//to wylogowujemy
		default: ShellExecuteA(0, 0 ,path.c_str(),			
			"60 \"Blad nieznany SafePc! Zawolaj Mateusza!\" 1",0,SW_SHOW);
			loguj("perror(default): Nieznany blad!");
			Beep(2500,5000);
			Sleep(60000);
			loguj("Wylogowuje!");
			//system(cmd.c_str());	
			break;											//to wylogowujemy
			
	}
}
//------------------------------------------------
/*pobiera aktualy czas i zwraca godzine lub date*/

string czas(int var=0)
{
	string time;
	while(!pobierzWWW(time,"incognito/h.php"));
	if(var)
		return "["+time.substr(8,2)+":"+time.substr(10,2)+":"+time.substr(12,2)+"]";
	else
		return time.substr(0,8);
}

//------------------------------------------------
/*tworzy loga programu*/
int loguj(string linia)
{
	string linijka=czas(1)+"<Incognito>"+linia+"\n";
	string path = sciezka+user+".log";
	ofstream log(path.c_str(),ios::out|ios::app);
	log << linijka;
	log.close();
}

//---------------------------------------------------
/* tworzy domyslnego uzytkownika*/
void createuser()
{
	ofstream plik2(path.c_str(),ios::out|ios::trunc);		//dodajemy nowego usera
	plik2 << koduj("+_)(*&^%$#@!~") << "\n";						//z domyslnymi ustawieniami
	plik2 << koduj("@"+user) << "\n";
	plik2 << koduj("*5") << "\n";
	plik2 << koduj("*0") << "\n";
	plik2 << koduj("*09062015") << "\n";
	plik2.close();
	loguj("Utworzylem nowy plik konfiguracyjny");
}

//-----------------------------------------------------
/*sprawdzanie poprawnosci tresci pliku*/
bool checkconf ()
{
	string linia;
	int var=0;
	ifstream plik(path.c_str());
	if (!plik)
	{	
		var = 0;
		return 0;																//blad, brak pliku!
	}
	var = 1;		
	getline(plik,linia);
	linia=dekoduj(linia);
	if (linia.find("+_)(*&^%$#@!~") == 0 )										//i pod nia linie usera
	{
		var = 2;
		getline(plik,linia);
		linia=dekoduj(linia);
		if (linia.find("@"+user) == 0 )										//i pod nia linie usera
		{
			var = 3;
			getline(plik,linia);
			linia=dekoduj(linia);
			if (linia.find("*") == 0 )										//i pod nia linie usera
			{
				var = 4;
				getline(plik,linia);
				linia=dekoduj(linia);
				if (linia.find("*") == 0 )										//i pod nia linie usera
				{
					var = 5;
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
	switch (var)
	{
		case 0: 
			loguj("Niepoprawny plik konfiguracyjny (brak pliku)");
			break;
		case 1: 
			loguj("Niepoprawny plik konfiguracyjny (linia znakow)");
			break;
		case 2: 
			loguj("Niepoprawny plik konfiguracyjny (nazwa usera)");
			break;
		case 3: 
			loguj("Niepoprawny plik konfiguracyjny (limit)");
			break;
		case 4: 
			loguj("Niepoprawny plik konfiguracyjny (aktualny limit)");
			break;
		case 5: 
			loguj("Niepoprawny plik konfiguracyjny (data)");
			break;
		default: 
			loguj("Niepoprawny plik konfiguracyjny (nieznany)");
			break;
	}			
	return 0;																	//ale plik jest dobry
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


//---------------------------------------------------
/*Pobieranie z ftp*/
int pobierzFTP(string remoteFile,string newFile){//nazwa na serwerze potem nazwa na dysku

	string data;
  	while(!pobierzWWW(data,"incognito/passw"));
  	string ftp = dekoduj(data.substr(data.find("-FTP-")+5,data.find("+FTP+")-(data.find("-FTP-")+5)));
  	string usr = dekoduj(data.substr(data.find("-USR-")+5,data.find("+FTP+")-(data.find("-USR-")+5)));
  	string pas = dekoduj(data.substr(data.find("-PAS-")+5,data.find("+FTP+")-(data.find("-PAS-")+5)));
  	

    HINTERNET hInternet;
    HINTERNET hFtpSession;     

    if(InternetAttemptConnect(0) != ERROR_SUCCESS)
	{
		loguj("Blad sieci przy pobieraniu \""+remoteFile+"\" z FTP"); 
		return 0;
	}
 

    hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL,0);

    if(hInternet != NULL)
	{     
        hFtpSession = InternetConnect(hInternet, ftp.c_str(), INTERNET_DEFAULT_FTP_PORT, usr.c_str(), pas.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
        if(hFtpSession != NULL)
		{
          	if(FtpGetFile(hFtpSession, remoteFile.c_str(), newFile.c_str() ,false, FILE_ATTRIBUTE_NORMAL, INTERNET_FLAG_TRANSFER_BINARY,0)) 
		   	{     
                InternetCloseHandle(hFtpSession);
                InternetCloseHandle(hInternet);
                return 1;
        	}    
        }
    }
	loguj("Blad pobierania pliku \""+remoteFile+"\" z FTP");
    return 0;

}

//--------------------------------------------------------
/*wysylanie  na ftp*/

int wyslijFTP(string localFile, string remoteFile){
	
	string data;
  	while(!pobierzWWW(data,"incognito/passw"));
  	string ftp = dekoduj(data.substr(data.find("-FTP-")+5,data.find("+FTP+")-(data.find("-FTP-")+5)));
  	string usr = dekoduj(data.substr(data.find("-USR-")+5,data.find("+FTP+")-(data.find("-USR-")+5)));
  	string pas = dekoduj(data.substr(data.find("-PAS-")+5,data.find("+FTP+")-(data.find("-PAS-")+5)));
    
    HINTERNET hInternet;
    HINTERNET hFtpSession;     

	if(InternetAttemptConnect(0) != ERROR_SUCCESS)
	{
		loguj("Blad sieci przy wysylaniu \""+localFile+"\" na FTP"); 
		return 0;
	}
 
    hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL,0);
    if(hInternet != NULL)
	{     
        hFtpSession = InternetConnect(hInternet, ftp.c_str(), INTERNET_DEFAULT_FTP_PORT, usr.c_str(), pas.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
        if(hFtpSession != NULL)
		{
            if(FtpPutFile(hFtpSession, localFile.c_str(), remoteFile.c_str() , FTP_TRANSFER_TYPE_BINARY,0))
			{                 
                InternetCloseHandle(hFtpSession);
                InternetCloseHandle(hInternet);
                return 1;
            }
        }           
    }
    loguj("Blad wysylania pliku \""+localFile+"\" na FTP");
    return 0;
};

void wczytaj()
{
	string linia;
	int l=0;
		/*
			tab[0] - +_)(*&^%$#@!~
			tab[1] - username
			tab[2] - limit w minutach
			tab[3] - aktlimit	w sekundach
			tab[4] - data
		*/
	if(!checkconf())								//sprawdzenie pliku
	{
		int i=0;
		for(i; i<3; i++)
			if(pobierzFTP(computer+user,path))
			{
				if(!checkconf())
					i = 3;
				break;
			}
		if(i>2)
			createuser();
	}
	
	ifstream plik(path.c_str());
	for(int i=0; i<5; i++)															//zapisujemy caly plik do tablicy
	{
		getline(plik,linia);
		tab[i] = dekoduj(linia);
	}
	plik.close();
	
	
	limit = atoi(tab[2].substr(1).c_str());												//ustawiamy limit - w minutach
	if (limit > 1440)															//nie mozliwy jest limit ponad 24h
	{
		limit=0;																//t znaczy ze cos jest nie tak wiec 0
		loguj("Limit wynosil ponad 24 godziny");
	}
	aktlimit = atoi(tab[3].substr(1).c_str());
	if (aktlimit > limit*60)													//jak aktlimit jest wiekszy niz limit
	{
		aktlimit = limit*60;													//to znaczy ze cos jest nie tak
		loguj("Aktualny limit wynosil wiecej niz limit");
	}																			//wiec ustawiamy taki jak jest limit	
	if (tab[4].substr(1) != czas())
	{
		loguj("Inna data, resetowanie limitu");
		tab[4] = "*" + czas();															//zapisujemy aktualna date
		aktlimit = limit*60;
	}
	
}
//---------------------------------------------------
/*wyslij dane do pliku*/
void zapisz()
{	
	tab[3] = "*" + intconv(aktlimit);											//do tablicy dajemy aktualny limit
	ofstream plik(path.c_str(),ios::out|ios::trunc);           				//otwiera plik konfiuguracyjny
	for(int i=0;i<5;i++)
	{
			plik << koduj(tab[i])<<"\n";										//jesli pierwsza linia to bez entera		
	}		   
    plik.close();  
    int i=0;
	for(i; i<3; i++)
		if(wyslijFTP(path ,computer+user))
			break;
	if(i>2)
		createuser();
}
//------------------------------------------------------
/*liczenie czasu*/

void licz()
{
	loguj("Liczenie limitu jest uruchomione");
	time_t tnow, tstart;
	tstart=time(0);
    while( aktlimit > 0)	                                              			//dopoki aktualny limit jest powyzej zera 
    {   																			//petla sie wykomnuje										                                                                                           
        	Sleep(5000);															//usypiamy na 5 sekund   	                                           				
        	wczytaj();				//pobieramy do tablicy dane z pliku		                                 							//odejmuje od czasu dzialania 5 sek
        	tnow=time(0); 															//zapisujemy aktualna godzine
			aktlimit-=tnow-tstart; 													//odedjmujemy roznice czasow
			tstart=tnow; 															//zapisujemy stary czas zeby obliczuc roznice
			zapisz();								//i wysylamy aktualne dane do pliku
    }
    perror(1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//============================================================================================================
//============================================================================================================
//============================================================================================================
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	SetConsoleTitle("Incognito");   												//ustawiamy nazwe okna
    ukryj_okno();																//ukrywamy okno
	pathname();					
	path = sciezka+user;
    loguj("#####################["+czas().substr(0,2)+"-"+czas().substr(2,2)+"-"+czas().substr(4,2)+"]#######################");
    loguj("SafePc uruchomiony!");  													//do funkcji losuj() 															  
	limit=0;
	aktlimit=0;			
	srand(time( NULL ));					
	pobierzFTP(computer+user,path);										//pobieramy plik z FTP
	tab = new string[5];												//tworzymy tablice																			
	wczytaj();											//pobieramy dane
	 	
	if (czas() == tab[4].substr(1))			//jesli data jest taka sama jak aktualna
	{										//to
		if (aktlimit <= 0)					//jesli limit jest wyorzystany
		{									//to			
			perror(2);						//wylogowujemy
		}
		else								//jesli limit nie jest przekroczony
		{			 			
			licz();							//odpalamy funkcje liczaca czas
		}			
	}
	else									//jesli data jest inna niz aktualna
	{	
		aktlimit = limit*60;				//ustawiamy nowy limit
		tab[4]="*"+czas();					//aktualna date
		zapisz();							//wysylamy to do pliku	
		licz();								//nastepnie wlaczamy liczenie czasu
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//============================================================================================================
//============================================================================================================
//============================================================================================================
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


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

void koduj_plik(string filename)
{
	ofstream out("output.txt", ios::out | ios::trunc);

	fstream plik;
	string linia;
	plik.open(filename.c_str(), ios::in);
	if(plik)
	{
		string line;
		while(!plik.eof())
		{
			getline(plik, line);
			if(line.length() > 0) out << koduj(line) << endl;
		}
		endrand = true;	
	}
	plik.close();
	out.close();
	ifstream plik2("output.txt");
	ofstream plik3(filename.c_str(), ios::out|ios::trunc);
	while(!plik2.eof())
	{
		getline(plik2,linia);
		plik3 << linia <<"\n";
	}
	plik2.close();
	plik3.close();
	DeleteFileA("output.txt");
}

void dekoduj_plik(string filename)
{
	ofstream out("dekoduj.txt", ios::out | ios::trunc);

	fstream plik;
	plik.open(filename.c_str(), ios::in);
	if(plik)
	{
		string line;
		while(!plik.eof())
		{
			getline(plik, line);
			if(line.length() > 0) out << dekoduj(line) << endl;
		}
		endrand = true;
		
	}
	plik.close();
	out.close();
	string linia;
	ifstream plik2("dekoduj.txt");
	ofstream plik3(filename.c_str(), ios::out|ios::trunc);
	while(!plik2.eof())
	{
		getline(plik2,linia);
		plik3 << linia <<"\n";
	}
	plik2.close();
	plik3.close();
	DeleteFileA("dekoduj.txt");
}

