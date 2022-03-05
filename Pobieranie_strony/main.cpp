#include <iostream>
#include <winsock.h>
#include <wininet.h>

using namespace std;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

bool getMasterServer(string &adr, string strona)
{
      WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        cout << "\nWSAStartup failed.\n";
        
        return false ;
    }
    SOCKET Socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    struct hostent *host;
    host = gethostbyname("igi2.xaa.pl");
    SOCKADDR_IN SockAddr;
    SockAddr.sin_port=htons(80);
    SockAddr.sin_family=AF_INET;
    SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
    cout << "\nConnecting...";
    if(connect(Socket,(SOCKADDR*)(&SockAddr),sizeof(SockAddr)) != 0){
        cout << "\nCould not connect";
        
        return false ;
    }
        cout << "\nConnected!" << endl;
    
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


int main(int argc, char** argv) {
	
	string buff;
	getMasterServer(buff,"incognito/h.php");
	cout <<buff<<endl;

	return 0;
}
