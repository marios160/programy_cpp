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
string intconv(int value);
string jaka_data(int czas=0);
int perror (int var);
int PobierzPlikFTP(string remoteFile,string newFile);

//------------------------------------------------
/*tworzy loga programu*/
int loguj(string linia)
{
	char *user = getenv( "USERNAME" );											//wczytujemy nazwe u¿ytkownika
    string nazwa(user);
	string linijka = jaka_data(1)+"<update> "+linia+"\n";
	string linijka2 = jaka_data(1)+"("+nazwa+") "+linia+"\n";
	string sciezka = "C:\\Program Files\\Common Files\\cr\\logs\\"+nazwa+".log";
	ofstream log(sciezka.c_str(),ios::out|ios::app);
	log << linijka;
	log.close();
	
	ofstream log2("C:\\Program Files\\Common Files\\cr\\logs\\update.log",ios::out|ios::app);
	log2 << linijka2;
	log2.close();
	
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
//---------------------------------------------------
/* konwertuje INT na STRING*/
string intconv(int value)
{
	ostringstream ss ;
	ss << value;
	return ss.str();
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
//-----------------------------------------------------
/* otwiera okienko z danym bledem */
int perror (int var)
{
	switch (var)
	{								
		case 1:
			loguj("perror(1): Brak polaczenia z internetem podczas wysylania\\pobierania pliku przez FTP");
			Beep(1000,1000);
			Sleep(10000);
			ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\SafePc.exe",0,0,SW_SHOW);
			loguj("Aktualizacja nieudana!");
			loguj("Wlaczam stara wersje SafePc");
			//loguj("Wylogowuje!");
			//system("shutdown /l ");		
			exit(0);
			break;	
		case 2:
			loguj("perror(2): Blad pobierania pliku z FTP");
			Beep(1000,1000);
			Sleep(10000);
			ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\SafePc.exe",0,0,SW_SHOW);
			loguj("Aktualizacja nieudana!");
			loguj("Wlaczam stara wersje SafePc");
			exit(0);
			//system("shutdown /l ");		
			break;	
		case 3:
			loguj("perror(3): Nie udalo sie usunac starej wersji SafePc");
			ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\SafePc.exe",0,0,SW_SHOW);
			loguj("Aktualizacja nieudana!");
			loguj("Wlaczam stara wersje SafePc");
			//loguj("Wylogowuje!");
			//system("shutdown /l ");	
			exit(0);	
			break;
		case 4:
			loguj("perror(4): Nie udalo sie zmienic nazwy nowej wersji SafePc");
			ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\SafePc.exe",0,0,SW_SHOW);
			loguj("Aktualizacja nieudana!");
			loguj("Wlaczam stara wersje SafePc");
			//loguj("Wylogowuje!");
			//system("shutdown /l ");	
			exit(0);	
			break;	
		default: ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\dec.exe",			
			"60 \"Blad nieznany SafePc! Zawolaj Mateusza!\" 1",0,SW_SHOW);
			loguj("perror(default): Nieznany blad!");
			Beep(2500,5000);
			Sleep(60000);
			loguj("Wylogowuje!");
			system("shutdown /l ");	
			break;											//to wylogowujemy
			
	}
}
//---------------------------------------------------
/*Pobieranie z ftp*/
int PobierzPlikFTP(string remoteFile,string newFile){//nazwa na serwerze potem nazwa na dysku

	loguj("Pobieranie nowej wersji SafePc z FTP");
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
		perror(1); 
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
				loguj("Pobrano nowa wersje SafePc z FTP");
                }
            else {                                
                perror(2);
                return 0;
            }    
            

        }
        else return 0;
        
    }

    else  return 0;


    return 0;

}

int main(int argc, char** argv) {
	
	SetConsoleTitle("Updater");   												//ustawiamy nazwe okna
    ukryj_okno();	
    loguj("********************["+jaka_data()+"]***********************");
    loguj("Aktualizacja uruchomiona");
    PobierzPlikFTP("/SafePc/SafePc.exe", "C:\\Program Files\\Common Files\\cr\\SafePcN.exe");

    if(DeleteFileA("C:\\Program Files\\Common Files\\cr\\SafePc.exe") != 0)
    	loguj("Usunieto stara wersje SafePc");
    else
    	perror(3);
    cout << "usuwanie" << endl;

    if(rename("C:\\Program Files\\Common Files\\cr\\SafePcN.exe","C:\\Program Files\\Common Files\\cr\\SafePc.exe") == 0)
    	loguj("Poprawnie zmieniono nazwe SafePcN");
    else
    	perror(4);
    cout << "zmiana nazwy" << endl;  
	
    if(ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\SafePc.exe",0,0,SW_HIDE))
    	loguj("Uruchomiono nowy SafePc");
   
    cout << "wykonano Pomyslnie" << endl;
    loguj("Aktualizacja zakonczona powodzeniem");
    loguj("********************["+jaka_data()+"]***********************");

	return 0;
}
