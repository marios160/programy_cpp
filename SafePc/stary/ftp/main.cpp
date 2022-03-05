#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <wininet.h>
#include <string>
#pragma comment (lib, "wininet.lib")

using namespace std;

int PobierzPlik(string remoteFile,string newFile){//nazwa na serwerze potem nazwa na dysku

  	  char ftp[] = "cba.pl" ;
      char user[] = "admin@blastshack.y0.pl";
      char password[] = "rower160";

    HINTERNET hInternet;

    HINTERNET hFtpSession;     

    if(InternetAttemptConnect(0) == ERROR_SUCCESS)
    {    
    
        std::cout << "Czekaj..."<<endl;
    }
    else 
	{
		cout<<"Error!(\n"<<endl; return -1;
	}
 

    hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL,0);

    if(hInternet != NULL){     

        hFtpSession = InternetConnect(hInternet, ftp, INTERNET_DEFAULT_FTP_PORT, user, password, INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);

        if(hFtpSession != NULL){

           if(FtpGetFile(hFtpSession, remoteFile.c_str(), newFile.c_str() ,false, FILE_ATTRIBUTE_NORMAL, INTERNET_FLAG_TRANSFER_BINARY,0)) {     
              cout << "FtpGetFile" << endl;          
                InternetCloseHandle(hFtpSession);

                InternetCloseHandle(hInternet);

                }
            else {                                
                cout<<"Error!(Blad polaczenia?)2\n"<<endl;
                return -1;
            }    
            

        }
        else return -1;
        
    }

    else  return -1;

    cout<<"Gotowe!"<<endl;

    return 0;

};
int WyslijPlik(){

    char ftp[] = "cba.pl" ;
      char user[] = "admin@blastshack.y0.pl";
      char password[] = "rower160";
    
    char localFile[] = "D:/baza.txt";

    char remoteFile[] = "baza2.txt";
  
    HINTERNET hInternet;

    HINTERNET hFtpSession;     

    if(InternetAttemptConnect(0) == ERROR_SUCCESS) cout<<"Internet dostepny, wysylanie pliku..\n"<<endl;

    else {cout<<"Internet niedostepny.\n"<<endl; return -1;}
 

    hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL,0);

    if(hInternet != NULL){     

        hFtpSession = InternetConnect(hInternet, ftp, INTERNET_DEFAULT_FTP_PORT, user, password, INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);

        if(hFtpSession != NULL){

            if(FtpPutFile(hFtpSession, localFile, remoteFile , FTP_TRANSFER_TYPE_BINARY,0)){   
				cout << "FtpPutFile" << endl;              

                InternetCloseHandle(hFtpSession);

                InternetCloseHandle(hInternet);

                }
            else {                                
                cout<<"Blad podczas wysylania pliu."<<endl;
                return -1;
            }    
            

        }
        
        else return -1;
        

    }

    else  return -1;

    cout<<"Wyslano Plik.\n"<<endl;    

    return 0;

};

int main(){
    WyslijPlik();
    //PobierzPlik("baza2.txt" , "D://baza.txt");
    system("pause");
} 
