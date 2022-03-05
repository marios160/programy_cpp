//program do kontroli czasu komputera

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <sstream>
#include <string>
#include <windows.h>
#include <fstream>
#include "stdafx.h"
#include <wininet.h>
#pragma comment (lib, "wininet.lib")

using namespace std;

int loguj(string linia);
void ukryj_okno();
void pokaz_okno();
void pathname(string &username, string &sciezka, string &namecomputer);
string intconv(int value);
int strconv(string value);
string jaka_data(int czas=0);
string losuj();
string code(string linia);
string decode(string linia);
int perror (int var);
int PobierzPlikFTP(string remoteFile,string newFile);
int WyslijPlikFTP(string localFile, string remoteFile);
int update(string sciezka, string sciezkaFTP, int aktlimit, string tab[],int rozm);
int createuser(string username, string sciezka);
int checkconf (string sciezka, string username);
int tablice(string sciezka,string username, int &rozm);
void process(string tab[], int rozm);
int pobierz(string sciezka, string sciezkaFTP, int &limit, int &aktlimit, string tab[],int rozm);
void wyslij(string sciezka, int aktlimit, string tab[],int rozm );
int stop_czas(string sciezka, string sciezkaFTP,string username, int limit, int &aktlimit, int rozm);
void licz_czas(string sciezka, string sciezkaFTP, string username, int limit, int &aktlimit, int rozm);
//------------------------------------------------
/*tworzy loga programu*/
int loguj(string linia)
{
	char *user = getenv( "USERNAME" );											//wczytujemy nazwe u¿ytkownika
    string nazwa(user);
	string linijka=jaka_data(1)+"<SafePc>"+linia+"\n";
	string sciezka = "C:\\Program Files\\Common Files\\cr\\logs\\"+nazwa+".log";
	ofstream log(sciezka.c_str(),ios::out|ios::app);
	log << linijka;
	log.close();
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
//--------------------------------------------------
/*sprawdzanie username i tworzenie sciezki*/
void pathname(string &username, string &sciezka, string &namecomputer)
{
	char *user = getenv( "USERNAME" );											//wczytujemy nazwe u¿ytkownika
    string nazwa(user);
    for(int j=0; j < nazwa.length(); j++)
	{
		if (nazwa[j] == ' ')					 								//jesli nazwa uzytkownika ma spacje
			nazwa[j] = '_';														//to zmieniamy je na pod³oge
	}
	username = nazwa;
	char *user2 = getenv( "COMPUTERNAME" );										//wczytujemy nazwe u¿ytkownika
    string nazwa2(user2);
	namecomputer= nazwa2;
    sciezka = "C:\\Program Files\\Common Files\\cr\\conf\\"+username;			//zapisujemy sciezke
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

//----------------------------------------------------------
/* konwertuje date do wygodnego typu string
jesli w argumencie damy 1 funkcja zwroci date i czas*/

string jaka_data(int czas)
{
	string data;
    time_t now = time(0);
    tm *ltm = localtime(&now);
    
    
    /*int rok=1900 + ltm->tm_year;
    int msc=1 + ltm->tm_mon;
    int dzien=ltm->tm_mday;
    int godz = ltm->tm_hour;
    int min = ltm->tm_min;
    int sek = ltm->tm_sec;*/
    int data_czas[6];
    string data_czas2[6];
    data_czas[0]= 1900 + ltm->tm_year;
    data_czas[1]= 1 + ltm->tm_mon;
    data_czas[2]= ltm->tm_mday;
    data_czas[3]= ltm->tm_hour;
    data_czas[4]= ltm->tm_min;
    data_czas[5]= ltm->tm_sec;
    for(int i=0 ; i<6 ;i++)
    {
    	if (data_czas[i]<10)
    		data_czas2[i]="0"+intconv(data_czas[i]);
    	else 
    		data_czas2[i]=intconv(data_czas[i]);
 	}	
    if (czas)
    	data="["+data_czas2[3]+":"+data_czas2[4]+":"+data_czas2[5]+"]";
    else
    	data = data_czas2[0]+"-"+data_czas2[1]+"-"+data_czas2[2];
    return data;
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
	linia=losuj()+linia+losuj()+linia+losuj();											//dodajemy kodzik zeby wiecej szyfru bylo
	return linia;
}


//---------------------------------------------------
/* rozkodowuje string*/

string decode(string linia)
{
	int i=0;
	string delinia = linia.substr(20,(linia.length()-60)/2);						//wyciagamy naszego stringa z kodu
	do
	{
		delinia[i]=delinia[i]-97;												//zmieniamy na 96 do tylu z tablicy ascii
		i++;
	}while(i<delinia.length());													//dopoki nie dojdziemy do konca stringa
	return delinia;
}
//-----------------------------------------------------
/* otwiera okienko z danym bledem */
int perror (int var)
{
	string username, sciezka, namecomputer;
	string cmd="shutdown /l ";
	switch (var)
	{
		case 1: ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\dec.exe",			
			"10 \"Nie ma pliku konfiguracyjnego!\nTworze nowy\" 0",0,SW_SHOW);
			loguj("perror(1): Brak pliku konfiguracyjnego");
			Beep(2000,5000);
			loguj("Proba pobrania pliku z FTP");			
			pathname(username,sciezka, namecomputer);
			if(!PobierzPlikFTP("/SafePc/"+namecomputer+username,sciezka))	
				createuser(username,sciezka);	
			break;											//to wylogowujemy
		case 2: ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\dec.exe",			
			"60 \"Limit wykorzystany!\" 1",0,SW_SHOW);
			loguj("perror(2): Koniec limitu");
			Beep(2000,1000);
			Sleep(60000);
			loguj("Wylogowuje!");
			system(cmd.c_str());	
			break;											//to wylogowujemy
		case 3: ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\dec.exe",			
			"5 \"Limit wykorzystany! Sprobuj jutro.\" 1",0,SW_SHOW);
			loguj("perror(3): Proba logowania po wykorzystanym limicie");
			Beep(2000,1000);
			Sleep(20000);
			loguj("Wylogowuje!");
			system(cmd.c_str());		
			break;										//to wylogowujemy
		case 4: ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\dec.exe",			
			"10 \"Plik konfiguracyjny jest niepoprawny!\nTworze nowy\" 0",0,SW_SHOW);
			loguj("perror(4): Niepoprawny plik konfiguracyjny");
			Beep(1000,1000);
			Sleep(10000);
			//loguj("Wylogowuje!");
			//system(cmd.c_str());		
			break;										//to wylogowujemy
		case 5: ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\dec.exe",			
			"10 \"Brak polaczenia z internetem!\" 0",0,SW_SHOW);
			loguj("perror(5): Brak polaczenia z internetem podczas wysylania\\pobierania pliku przez FTP");
			Beep(1000,1000);
			//Sleep(10000);
			//system(cmd.c_str());		
			break;	
		case 6: ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\dec.exe",			
			"10 \"Nie udalo sie pobrac pliku konfiguracyjnego!\" 0",0,SW_SHOW);
			loguj("perror(6): Blad pobierania pliku z FTP");
			Beep(1000,1000);
			Sleep(10000);
			//system(cmd.c_str());		
			break;	
		case 7: ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\dec.exe",			
			"10 \"Nie udalo sie wyslac pliku konfiguracyjnego!\" 0",0,SW_SHOW);
			loguj("perror(7): Blad wysylania pliku na FTP");
			Beep(1000,1000);
			Sleep(10000);
			//system(cmd.c_str());		
			break;											//to wylogowujemy
		default: ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\dec.exe",			
			"60 \"Blad nieznany SafePc! Zawolaj Mateusza!\" 1",0,SW_SHOW);
			loguj("perror(default): Nieznany blad!");
			Beep(2500,5000);
			Sleep(60000);
			loguj("Wylogowuje!");
			system(cmd.c_str());	
			break;											//to wylogowujemy
			
	}
}



//---------------------------------------------------
/*aktualizacja oprogramowania*/
int update(string sciezka, string sciezkaFTP, int aktlimit, string tab[],int rozm )
{
	loguj("Aktualizacja oprogramowania");
	tab[4]="0";	
    wyslij(sciezka, aktlimit, tab, rozm);								//wysylamy to do pliku
    WyslijPlikFTP(sciezka,sciezkaFTP);
	ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\updater.exe",0,0,SW_SHOW);
	exit (0);
	return 0;
}

//---------------------------------------------------
/* sprawdza czy uzytkownik jest w pliku conf*/
int createuser(string username, string sciezka)
{
	ofstream plik2(sciezka.c_str(),ios::out|ios::trunc);		//dodajemy nowego usera
	plik2 << code("@"+username) << "\n";						//z domyslnymi ustawieniami
	plik2 << code("5") << "\n";
	plik2 << code("0") << "\n";
	plik2 << code("2014_06_30") << "\n";
	plik2 << code("0") << "\n";
	plik2 << code("0") << "\n";
	plik2 << code("startapk") << "\n";
	plik2 << code("endapk");
	plik2.close();
	loguj("Utworzylem nowy plik konfiguracyjny");
	return 0;
}

//-----------------------------------------------------
/*sprawdzanie poprawnosci tresci pliku*/
int checkconf (string sciezka, string username)
{
	string linia;
	bool var=0;
	ifstream plik(sciezka.c_str());
	if (!plik)
	{	
		perror(1);																//blad, brak pliku!
	}		
			getline(plik,linia);
			linia=decode(linia);
			if (linia.find("@"+username) == 0 )										//i pod nia linie usera
			{
				return 1;
			}
			else
			{
				loguj("Niepoprawny plik konfiguracyjny");
				perror(4);														//jesli nie znajdziemy naszego usera
				createuser(username, sciezka);									//trzeba go bedzie stworzyc														
			}	
					
	return 1;																	//ale plik jest dobry
}
//--------------------------------------------------
/*zabija procesy*/
void process(string tab[],int rozm)
{
	for (int i=7;i<rozm-1;i++)
	{
		string cmd="taskkill /FI \"IMAGENAME eq "+tab[i]+"\" /T /F";		//zapisuje polecenie dodajac zdekodowana linie
		// FI - filtr, IMAGENAME - nazwa okna, T - wy³¹czanie podprocesów, F - wymuszenie zamkniêcia
		system(cmd.c_str());												//wysylamy killa do cmd 
	}
}
//--------------------------------------------------------
/*oblicza rozmiar tablicy*/
int tablice(string sciezka,string username, int &rozm)
{
	string linia;
	rozm=0;
	ifstream plik(sciezka.c_str());												//otwieramy plik konfiguracyjny
    if (!plik)																	//jesli go nie ma
    {	
    		perror(1);															//blad, brak pliku!
	}
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

	loguj("Pobieranie pliku konfiguracyjnego z FTP");
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
		perror(5); 
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
				loguj("Pobrano plik konfiguracyjny z FTP");
                }
            else {                                
                perror(6);
                return 0;
            }    
            

        }
        else return 0;
        
    }

    else  return 0;


    return 1;

}

//--------------------------------------------------------
/*wysylanie  na ftp*/

int WyslijPlikFTP(string localFile, string remoteFile){
	
	loguj("Wysylanie pliku konfiguracyjnego na FTP");
    char ftp[]      = "cba.pl";
    char user[]     = "admin@blastshack.y0.pl";
    char password[] = "rower160";
    
  
    HINTERNET hInternet;

    HINTERNET hFtpSession;     

    if(InternetAttemptConnect(0) == ERROR_SUCCESS) 
		cout<<"Wysylanie pliku!"<<endl;

    else 
	{
		perror(5);
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
				loguj("Wyslano plik konfiguracyjny na FTP");
                }
            else {                                
                perror(7);
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
/*pobiera dane z pliku*/
int pobierz(string sciezka, string sciezkaFTP, int &limit, int &aktlimit, string tab[],int rozm)
{
	
	string linia;
	int l=0;
		/*
			tab[pozp] - username
			tab[pozp+1] - limit w minutach
			tab[pozp+2] - aktlimit	w sekundach
			tab[pozp+3] - data
			tab[pozp+4] - update
			tab[pozp+5] -  process
			tab[pozp+6] - startapk
			tab[pozp+7] - aplikacje.....
			tab[pozp+....] -  aplikacje
			tab[pozk] - endapk
		*/
	ifstream plik(sciezka.c_str());
	if (!plik)																	//jesli go nie ma
    {	
    		perror(1);															//blad, brak pliku!
	}
	while (!plik.eof())															//zapisujemy caly plik do tablicy
	{
		getline(plik,linia);
		tab[l] = decode(linia);
		l++;
	}
	plik.close();
	limit = strconv(tab[1]);												//ustawiamy limit - w minutach
	if (limit > 1440)															//nie mozliwy jest limit ponad 24h
	{
		limit=0;																//t znaczy ze cos jest nie tak wiec 0
		loguj("Limit wynosil ponad 24 godziny");
	}
	aktlimit = strconv(tab[2]);
	if (aktlimit > limit*60)													//jak aktlimit jest wiekszy niz limit
	{
		aktlimit = limit*60;													//to znaczy ze cos jest nie tak
		loguj("Aktualny limit wynosil wiecej niz limit");
	}																			//wiec ustawiamy taki jak jest limit	
	if (tab[3] != jaka_data())
	{
		loguj("Inna data, resetowanie limitu");
		tab[3] = jaka_data();															//zapisujemy aktualna date
		aktlimit = limit*60;
	}
		
	
	
	if (tab[5] != "0")													//czy wlaczyc obluge procesow
	{
		tab[5] = "1";
		process(tab,rozm);													//wlaczamy ProcesControll
	}
	if (tab[4] != "0")												//sprawdzamy czy jest nowa aktualizacja
		update(sciezka, sciezkaFTP, aktlimit, tab, rozm);                                              			
	
}
//---------------------------------------------------
/*wyslij dane do pliku*/
void wyslij(string sciezka, int aktlimit, string tab[],int rozm)
{	
	tab[2] = intconv(aktlimit);											//do tablicy dajemy aktualny limit
	ofstream plik(sciezka.c_str(),ios::out|ios::trunc);           				//otwiera plik konfiuguracyjny
	for(int i=0;i<rozm;i++)
	{
		if (i == 0)
			plik << code(tab[i]);												//jesli pierwsza linia to bez entera
		else																	//zapobiega to bezsensownym enterom w pliku
			plik << "\n" << code(tab[i]);			
	}		   
    plik.close();  
}

//---------------------------------------------------
/*nie liczy limitu*/
int stop_czas(string sciezka, string sciezkaFTP,string username, int limit, int &aktlimit, int rozm)
{
	loguj("Liczenie limitu jest zatrzymane");
	string *tab;
	int i=0;
	while (limit == 1440)
	{
		if (i >= 600 )
		{

			PobierzPlikFTP(sciezkaFTP,sciezka);										//pobieramy plik z FTP	co 10 min
			i=0;	
		}
		i+=10;
		tablice(sciezka,username,rozm); 										//oblicza rozmiary tablic
		tab = new string[rozm]; 												//tworzymy dynamiczna tablice		           
		pobierz(sciezka, sciezkaFTP, limit, aktlimit, tab,  rozm);	//pobieramy dane
		if (tab[3] != jaka_data())
		{
			loguj("Inna data, resetowanie limitu");
			tab[3] = jaka_data();															//zapisujemy aktualna date
			wyslij(sciezka, aktlimit, tab, rozm);	
			WyslijPlikFTP(sciezka,sciezkaFTP);
		}
		if(tab[5] != "0")
			process(tab,rozm);													//wlaczamy ProcesControll

		delete [] tab;
		Sleep(10000);
		
	}
	loguj("Wznawianie liczenia limitu");
	return 0;
	
}

//---------------------------------------------------
/*liczy limit*/

void licz_czas(string sciezka, string sciezkaFTP, string username, int limit, int &aktlimit, 
					int rozm)
{
	loguj("Liczenie limitu jest uruchomione");
	time_t tnow, tstart;
	string *tab;
	tstart=time(0);
    while( aktlimit > 0)	                                              			//dopoki aktualny limit jest powyzej zera 
    {   																			//petla sie wykonuje
																			
		for(int i=0,j=0 ; i<120 ; i++,j++)											//co 10 min pobieramy plik z serwera FTP
		{
		    if (limit == 1440)														//patrzymy czy wylaczyc limit
				stop_czas(sciezka, sciezkaFTP, username, limit, aktlimit, rozm);	 		                                                                                           
        	Sleep(5000);															//usypiamy na 5 sekund   
			if (!checkconf(sciezka,username))										//sprawdzamy czy plik jest poprawny
				PobierzPlikFTP(sciezkaFTP,sciezka);									//pobieramy plik z FTP 
        	tablice(sciezka,username,rozm); 								//oblicza rozmiary tablic
			tab = new string[rozm]; 												//tworzymy dynamiczna tablice		                                           				
        	pobierz(sciezka, sciezkaFTP, limit, aktlimit, tab, rozm);				//pobieramy do tablicy dane z pliku		                                 							//odejmuje od czasu dzialania 5 sek
        	tnow=time(0); 															//zapisujemy aktualna godzine
			aktlimit-=tnow-tstart; 													//odedjmujemy roznice czasow
			tstart=tnow; 															//zapisujemy stary czas zeby obliczuc roznice
			wyslij(sciezka, aktlimit, tab, rozm);								//i wysylamy aktualne dane do pliku
			delete [] tab;															//kasujemy tablice
			if (j >= 12)
			{
				WyslijPlikFTP(sciezka,sciezkaFTP);									//co 1 min wysylamy plik na server FTP  
				j=0;
			}
		}
		PobierzPlikFTP(sciezkaFTP,sciezka);											//pobieramy plik z FTP	co 10 min
    }
    
    perror(2);																		//wylogowuje za 60 sek
}

//---------------------------------------------------------------------
//pobieranie http configracji
int getMasterServer(string strona)
{
	
         WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        //cout << "\nWSAStartup failed.\n";
        
        return false ;
    }
    SOCKET Socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    struct hostent *host;
    host = gethostbyname("gigi.y0.pl");
    SOCKADDR_IN SockAddr;
    SockAddr.sin_port=htons(80);
    SockAddr.sin_family=AF_INET;
    SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
    //cout << "\nConnecting...";
    if(connect(Socket,(SOCKADDR*)(&SockAddr),sizeof(SockAddr)) != 0){
        //cout << "\nCould not connect";
        
        return false ;
    }
            //cout << "\nConnected!";
    
    
    string request="GET /"+strona+".php HTTP/1.1\r\n"
    "Host: gigi.y0.pl\r\nConnection: close\r\n\r\n";
    send(Socket,request.c_str(), strlen(request.c_str()),0);
    char buffer[10000];
    int nDataLength;
    recv(Socket,buffer,10000,0);

    recv(Socket,buffer,10000,0);

	
	string buf(buffer);
	
	string sciezka;	
	fstream plik(sciezka.c_str(),ios::out|ios::trunc);
	plik << buf;
	plik.close();
    
	closesocket(Socket);
    WSACleanup();
    string linia;
    int i=0;
    plik.open(sciezka.c_str(),ios::in);
    while (!plik.eof())
    {
    	getline(plik,linia);
    	if (linia.find("===============") != string::npos)
		{
			
			while(linia.find("+++++++++++++++") == string::npos)
			{
				getline(plik,linia);
				komendy[i]=linia;
				i++;
			}
		} 
    }
    plik.close();
    DeleteFileA(sciezka.c_str());
    
    return i-1;
}




///////////////////////////////////////////////////////////////////////////////////
//################################################################################
//################################################################################
//################################################################################


int main(int argc, char *argv[]) 
{
	SetConsoleTitle("SavePC");   												//ustawiamy nazwe okna
    ukryj_okno();																//ukrywamy okno
    loguj("#####################["+jaka_data()+"]#######################");
    loguj("SafePc uruchomiony!");  
	int limit=0,aktlimit=0,rozm=0;
	string linia,username,namecomputer,*tab,*apk,sciezka,sciezkaFTP;
	
		
	
	srand(time( NULL ));														//do funkcji losuj() 															  
	pathname(username,sciezka,namecomputer);									//sprawdzamy nazwe uzytkownika i sciezke
	sciezkaFTP = "/SafePc/"+namecomputer+username;
	PobierzPlikFTP(sciezkaFTP,sciezka);											//pobieramy plik z FTP
		
   	tablice(sciezka,username,rozm); 									//oblicza rozmiary tablicy
	tab = new string[rozm];														//tworzymy tablice
	if (argc>=2)																//jesi program zostal wlaczony z argumentem
	{
		
		int a = atoi(argv[1]);  												//to ustawiamy  nowy aktlimit!
		loguj("Zmiana aktualnego limitu przez argument: "+intconv(a));
		pobierz(sciezka, sciezkaFTP, limit, aktlimit, tab, rozm);
		aktlimit = a;
		wyslij(sciezka, aktlimit, tab, rozm);	
	}
																				
	pobierz(sciezka, sciezkaFTP, limit, aktlimit, tab, rozm);					//pobieramy dane
	
	
   	
	if (jaka_data() == tab[3])												//jesli data jest taka sama jak aktualna
	{																			//to
		if (aktlimit <= 0)														//jesli limit jest wyorzystany
		{																		//to			
			perror(3);															//wylogowujemy
		}
		else																	//jesli limit nie jest przekroczony
		{			
			delete [] tab;														//usuwamy tablice   			
			licz_czas(sciezka, sciezkaFTP, username, limit, aktlimit, rozm);								//odpalamy funkcje liczaca czas
		}			
	}
	else																		//jesli data jest inna niz aktualna
	{	
		aktlimit = limit*60;													//ustawiamy nowy limit
		tab[3]=jaka_data();												//aktualna date
		wyslij(sciezka, aktlimit, tab, rozm);								//wysylamy to do pliku
		delete [] tab;															//i dopiero teraz kasujemy tablice		
		licz_czas(sciezka, sciezkaFTP, username, limit, aktlimit, rozm);	//nastepnie wlaczamy liczenie czasu
	}
	
	return 0;
}
