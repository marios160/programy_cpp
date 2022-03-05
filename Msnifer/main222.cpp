#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <string.h>
#include <time.h>
#include <sstream>
#ifdef WIN32
    #include <winsock.h>
    #define sleep 	Sleep
    #define zamknij   closesocket
    #include <process.h>
#else
    #include <arpa/inet.h>
    #include <pthread.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #define zamknij   close

#endif
#include <pcap.h>



using namespace std;


struct Dane
{
	string ip;
	long czas;
	int port;
};

FILE *plk;
string bancdk[50];
int licz=0, licztab=0;
//Dane tab[100];
Dane player;
string ip="",zakres="";
string linia,arcon="(s*E=15)",jrcon="juniorA";
time_t rawtime,inf;
int bronie=92;
struct tm * timeinfo;
char info[] =
            "\xde\xad\xbe\xef\xab\xab\xab\xab\xab\xab\xab\xab\x0\x0\x0\x0\x05\xab\xab\xab\xba\xde\xab\xee",    
        pck1[] =
            "\xde\xad\xbe\xef\xab\xab\xab\xab\xab\xab\xab\xab\x00\x01\x00\x14\x00\xab\xab\xab\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xba\xde\xab\xee",
		pck2[] =
            "\xde\xad\xbe\xef\xab\xab\xab\xab\xab\xab\xab\xab\x00\x03\x00\x14\x00\xab\xab\xab\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xba\xde\xab\xee",
        pck3[] =
			"\x00\x14\x00\xab\xab\xab";


void packet_handler(u_char* param,const struct pcap_pkthdr* header,const u_char* pkt);

bool pobierzWWW(string &adr, string strona)
{
#ifdef WIN32
      WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        
        return false ;
    }
#endif
    int sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    struct hostent *host;
    host = gethostbyname("www.igi2.xaa.pl");
    struct sockaddr_in SockAddr;
    SockAddr.sin_port=htons(80);
    SockAddr.sin_family=AF_INET;
    SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
    if(connect(sock,(sockaddr*)(&SockAddr),sizeof(struct sockaddr)) != 0){
       	cout <<"blad"<<endl;
        return false ;
    }
    string request="GET /"+strona+" HTTP/1.1\r\n"
    "Host: igi2.xaa.pl\r\nConnection: close\r\n\r\n";
    send(sock,request.c_str(), strlen(request.c_str()),0);
    char buffer[10000];
    int nDataLength;
    recv(sock,buffer,10000,0);

    recv(sock,buffer,10000,0);

	string buf(buffer);		//zawartosc ktora nas interesuje jest miedzy ||||| a :::::
	
	adr=buf.substr(buf.find("|||||")+5,buf.find(":::::")-(buf.find("|||||")+5));
	zamknij(sock);
#ifdef WIN32
    WSACleanup();
#endif
    
    return true;
}
			
			
string czas()
{
	string adr;
	pobierzWWW(adr,"czas.php?var=5");
	return adr;
 	
}			
			
void* snif(void* unused)
{
	
	char dev[]="venet0"; //nazwa urzadzenia
	pcap_t* fp;
	char errbuf[PCAP_ERRBUF_SIZE]; //jeÂ¿eli bÃªdziemy chcieli obsÂ³ugiwaÃ¦ bÂ³Ãªdy
	u_int netmask,ip_addr;
	struct bpf_program fcode;
	char packet_filter[]="udp";

	if(pcap_lookupnet(dev,&ip_addr,&netmask,errbuf)==-1)
	{	
		printf("Nie mozna uzyskac maski: %s\n",errbuf);
		ip_addr=0;
		netmask=0;
	}
//otwieramy urzÂ¹denie
	if((fp=pcap_open_live(dev,65536,1,1000,errbuf))==NULL)
	{
		printf("Nie moÂ¿na otworzyÃ¦ urzÂ¹dzenia: %s\n",errbuf);
		exit(1);
	}
//kompilacja filtra
	if(pcap_compile(fp,&fcode,packet_filter,0,netmask)<0)
	{
		printf("SkÂ³adnia filtru jest niepoprawna.\n");
		pcap_close(fp);
		exit(1);
	}
//ustawienie filtra
	if(pcap_setfilter(fp,&fcode)<0)
	{
		printf("Blad podczas ustawiania filtru.\n");
		pcap_close(fp);
		exit(1);
	}
	
	ifstream plik("blackcdk");
	if(plik)
	while(!plik.eof() && licz < 50)
	{
		getline(plik,bancdk[licz]);
		licz++;	
	}
	plik.close();
	ifstream netw("/root/pc/networkconfig.cfg");
	while(!netw.eof())
	{
		getline(netw,linia);
		if(linia.find("lo rconpass(") != string::npos)
		{
			arcon = linia.substr(13,linia.find("\")")-13);
			getline(netw,linia);
			if(linia.find("lo juniorpass(") != string::npos)
				jrcon = linia.substr(15,linia.find("\")")-15);
			else
				jrcon = "dupa";
			break;
		}
	}
	netw.close();
	
	pcap_loop(fp,0,packet_handler,NULL); //i to jest funkcja przechwytujÂ¹ca pakiety
	pcap_close(fp);

}



struct ipheader
{
	unsigned char ip_hl:4,ip_v:4;
	unsigned char ip_tos;
	unsigned short int ip_len;
	unsigned short int ip_id;
	unsigned short int ip_off;
	unsigned char ip_ttl;
	unsigned char ip_p;
	unsigned short int ip_sum;
	unsigned int ip_src;
	unsigned int ip_dst;
};
struct udpheader {
 unsigned short int udph_srcport;
 unsigned short int udph_destport;
 unsigned short int udph_len;
 unsigned short int udph_chksum;
};


void packet_handler(u_char* param,const struct pcap_pkthdr* header,const u_char* pkt)
{
	time (&rawtime);
	ipheader* ih;
	udpheader *ud;
	sockaddr_in source_addr,dest_addr;
	ih=(struct ipheader*)(pkt+16); //pomijamy 16 bajtÃ³w 
	ud=(struct udpheader*)(pkt+36);
	
	source_addr.sin_addr.s_addr=ih->ip_src;
	source_addr.sin_port = ud->udph_srcport;
	dest_addr.sin_addr.s_addr=ih->ip_dst;
	dest_addr.sin_port = ud->udph_destport;

string sourceIP = inet_ntoa(source_addr.sin_addr);
string destIP = inet_ntoa(dest_addr.sin_addr);
int port = htons(source_addr.sin_port);

char cdkc[31];
string atak;
char rcon[]="NOPASSNO", id[]="11";



  
	if (destIP == "91.230.202.108")
	{
		

		if(!memcmp((char*)pkt+44,(char*)info,24))			//jesli to pakiet info
		{
			
			player.ip = sourceIP;
			player.czas = rawtime;
			player.port = port;
		}
		else
		if(!memcmp((char*)pkt+58,(char*)pck3,6))			//jesli to pakiet 1
		{
				if (sourceIP == player.ip)					//jesli port drugiego pakietu jest inny to bugs
				{
					//if (player.czas+1 >= rawtime)
					if(player.port != port)
					{
						string cmd = "/sbin/iptables -I INPUT -s "+sourceIP+" -j DROP";
						system(cmd.c_str());
						ofstream plik("/root/pc/bany.txt",ios::out|ios::app);
						plik<<cmd<<"		#"<<czas()<<" Bugs Crash"<<endl;
						plik.close();	
						cout <<czas()<<" Znaleziono bugs crash0: "<<sourceIP<<endl;
						player.czas = 0;
						player.ip = "";
						player.port = 0;
						
					}
					else
					{
						player.czas = 0;
						player.ip = "";
						player.port = 0;
					}
					
				}
			
		}
		else
		if(!memcmp((char*)pkt+44,(char*)pck1,44))			//jesli to pakiet 1
		{
				if (sourceIP == player.ip)					//jesli port drugiego pakietu jest inny to bugs
				{
					//if (player.czas+1 >= rawtime)
					if(player.port != port)
					{
						string cmd = "/sbin/iptables -I INPUT -s "+sourceIP+" -j DROP";
						system(cmd.c_str());
						ofstream plik("/root/pc/bany.txt",ios::out|ios::app);
						plik<<cmd<<"		#"<<czas()<<" Bugs Crash"<<endl;
						plik.close();	
						cout <<czas()<<" Znaleziono bugs crash1: "<<sourceIP<<endl;
						player.czas = 0;
						player.ip = "";
						player.port = 0;
						
					}
					else
					{
						player.czas = 0;
						player.ip = "";
						player.port = 0;
					}
					
				}
			
		}
		else if(!memcmp((char*)pkt+44,(char*)pck2,44))			//jesli to pakiet 1
		{
				if (sourceIP == player.ip)					//jesli port drugiego pakietu jest inny to bugs
				{
					//if (player.czas+1 >= rawtime)
					if(player.port != port)
					{
						string cmd = "/sbin/iptables -I INPUT -s "+sourceIP+" -j DROP";
						system(cmd.c_str());
						ofstream plik("/root/pc/bany.txt",ios::out|ios::app);
						plik<<cmd<<"		#"<<czas()<<" Bugs Crash"<<endl;
						plik.close();	
						cout <<czas()<<" Znaleziono bugs crash3: "<<sourceIP<<endl;
						player.czas = 0;
						player.ip = "";
						player.port = 0;
						
					}
					else
					{
						player.czas = 0;
						player.ip = "";
						player.port = 0;
					}
					
				}
			
		}
		else
		if(pkt[72]==72 && pkt[73]== 84 && pkt[74] == 85 && pkt[75] == 65) 		//jesli HTUA  - AUTH
		{
			strncpy(cdkc,(char*)pkt+84,31);
			string cdk(cdkc);
			for(int i=0; i<licz; i++)
				if(cdk == bancdk[i])
				{
					if (ip != sourceIP)
					{
						string cmd = "/sbin/iptables -I INPUT -s "+sourceIP+" -j DROP";
						system(cmd.c_str());
						ip=sourceIP;
						ofstream plik("/root/pc/bany.txt",ios::out|ios::app);
						plik<<cmd<<"		#"<<czas()<<" Black CDK"<<endl;
						plik.close();
						cout <<czas()<<" Znaleziono zablokowany CD-KEY: "<<sourceIP<<endl;
					}
						break;
					
				}
			
			ofstream plik("bazacdk",ios::out|ios::app);
			plik<<cdk<<"	"<<sourceIP<<endl;
			plik.close();
		}
		else
		if(pkt[72]==84 && pkt[73]== 83 && pkt[74] == 73 && pkt[75] == 76) 		//jesli TSIL - LIST
	     {
			if(pkt[128]==114 && pkt[129]== 99 && pkt[130] == 111 && pkt[131] == 110) 	//jesli rcon
			{
				strncpy(id,(char*)pkt+88,1);
				strncpy(rcon,(char*)pkt+135,8);
				int iid;
				string sid(id);
				iid = (int)sid[0];
				string srcon(rcon);
				if(srcon.rfind("\"") < 8)
				{
					srcon=srcon.substr(0,srcon.rfind("\""));
				}
				if(srcon == arcon)
				{
					ofstream plik("/root/pc/DATA/admin.txt",ios::out|ios::app);
					plik<<"a"<<iid<<endl;
					plik.close();
					cout <<czas()<<" Znaleziono rcona: "<<sourceIP<<" "<<iid<<" "<<srcon<<endl;
				}
				else
				if(srcon == jrcon)
				{	
					ofstream plik("/root/pc/DATA/admin.txt",ios::out|ios::app);
					plik<<"j"<<iid<<endl;
					plik.close();
					cout <<czas()<<" Znaleziono junior rcona: "<<sourceIP<<" "<<iid<<" "<<srcon<<endl;
				}

					
				
		
			}
	     }

		
		
	}
	/*else if (sourceIP == "91.230.202.108")
	{
		if(pkt[72]==80 && pkt[73]== 79 && pkt[74] == 82 && pkt[75] == 68) // DROP - PORD 
		{
			bronie--;
		}
		else
		if(pkt[72]==75 && pkt[73]== 67 && pkt[74] == 73 && pkt[75] == 80) // PICK - KCIP
		{
			bronie++;
		}
	}*/


}



//g++ main.cpp -o 3in1 -lpcap -lpthread
//./snif venet0 udp
//apt-get install libpcap0.8 libpcap-dev			



///////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool check(string &ip)
{
    system("wget -q www.igi2.xaa.pl/unbaniki.txt");
    ifstream plik("unbaniki.txt");
    getline(plik,ip);
    plik.close();
    if (ip.empty())
    	return false;
    else
    	return true;
}



void* gtacc(void* unused)
{
	string ip;
	bool var ;
	string ban;
	ofstream plik;
	while (1)
	{
		var = check(ip);
		if (var)
		{
			ban="/sbin/iptables -I INPUT -s "+ip+" -j ACCEPT";
			system(ban.c_str());
			ofstream plik("wyjatki",ios::out|ios::app);
			plik << ban <<endl;
			plik.close();
			cout <<czas()<<" Dodano wyjatek dla: "<<ip<<endl;
			system("wget -q www.igi2.xaa.pl/gt.php?usun=1");
			system("rm gt.php?usun=1");
			ip = "";
		}
		system("rm unbaniki.txt");
		sleep(5);

	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////


void* hacker(void* unused)
{
	int obecnapoz=0, poprzedniapoz=0;
	string linia, ip, ip1, ip2, cmd;
	int i=0;
	time (&rawtime);

	ifstream netw("/var/log/auth.log");
	netw.seekg(0,ios::end);				//ustawia pozycje czytania na koncu pliku
        poprzedniapoz = netw.tellg();
	netw.close();

	while(1)
	{
		ifstream netw("/var/log/auth.log");
		if (!netw) continue;
		
		netw.seekg(0,ios::end);				//ustawia pozycje czytania na koncu pliku
        	obecnapoz = netw.tellg();			// zapisuje do zmiennej pozycje w pliku
		if(obecnapoz > poprzedniapoz)		//jesli pojawily sie nowe linijki
        	{
			netw.seekg(poprzedniapoz);		//to ustawia pozycje na poprzednia
			poprzedniapoz=obecnapoz;		// przepisuje aktualna pozycje do pozycji poprzedniej
		}
		while(!netw.eof())
		{
			getline(netw,linia);
			if(linia.find("PAM 5 more authentication failures") != string::npos)
			{
				ip = linia.substr(linia.find("rhost=")+6,linia.find("  user=")-(linia.find("rhost=")+6));
				cmd="/sbin/iptables -I INPUT -s "+ip+"/24 -j DROP";
				system(cmd.c_str());
				ofstream plik("/root/pc/bany.txt",ios::out|ios::app);
				plik << cmd <<"		#"<<czas()<<" PUTTY Hacker"<<endl;
				plik.close();
				cout <<czas()<<" Zlapano hackera: "<<ip<<endl;
			}
			else if (linia.find("Failed password for root from ") != string::npos)
			{
				ip2 = linia.substr(linia.find("from ")+5,linia.find(" port")-(linia.find("from ")+5));
				 
				if(ip2 == ip1)
				{
					i++;
				}
				else
				{
					ip1 = ip2;
					i=0;
				}
				
				if(i > 4)
				{
					cmd="/sbin/iptables -I INPUT -s "+ip2+"/24 -j DROP";
					system(cmd.c_str());
					ofstream plik("/root/pc/bany.txt",ios::out|ios::app);
					plik << cmd <<"		#"<<czas()<<" PUTTY Hacker"<<endl;
					plik.close();
					cout <<czas()<<" Zlapano hackera: "<<ip2<<endl;
					i=0;
				}
				
			}
		}
		netw.close();
		
		sleep(5);
	}
}
 /*Jun 30 06:17:36 igi2 sshd[16870]: Failed password for root from 178.42.159.241 port 49701 ssh2
Jun 30 06:17:39 igi2 sshd[16870]: Failed password for root from 178.42.159.241 port 49701 ssh2
Jun 30 06:17:43 igi2 sshd[16870]: Failed password for root from 178.42.159.241 port 49701 ssh2
Jun 30 06:17:47 igi2 sshd[16870]: Failed password for root from 178.42.159.241 port 49701 ssh2
Jun 30 06:17:51 igi2 sshd[16870]: Failed password for root from 178.42.159.241 port 49701 ssh2
Jun 30 06:17:56 igi2 sshd[16870]: Failed password for root from 178.42.159.241 port 49701 ssh2
Jun 30 06:17:56 igi2 sshd[16870]: PAM 5 more authentication failures; logname= uid=0 euid=0 tty=ssh ruser= rhost=afqd241.neoplus.adsl.tpnet.pl  user=root
*/

void* log(void* unused)
{
	int obecnapoz, poprzedniapoz;

	while(1)
	{
		ifstream netw("/var/log/auth.log");
		if (!netw) continue;
		
		netw.seekg(0,ios::end);				//ustawia pozycje czytania na koncu pliku
        	obecnapoz = netw.tellg();			// zapisuje do zmiennej pozycje w pliku
		if(obecnapoz > poprzedniapoz)		//jesli pojawily sie nowe linijki
        {
			netw.seekg(poprzedniapoz);		//to ustawia pozycje na poprzednia
			poprzedniapoz=obecnapoz;		// przepisuje aktualna pozycje do pozycji poprzedniej
		}
		while(!netw.eof())
		{
			getline(netw,linia);
			if(linia.find("MapControl_SetLoading(): set toTRUE") != string::npos)
			{
				bronie = 92;
			}
			
		}
		netw.close();
		sleep(5);
	}
}

void* ftp(void* unused)
{

	int obecnapoz=0, poprzedniapoz=0;
	string linia, ip, ip1, ip2, cmd;
	int i=0, l=0;
	Dane players[5] = {{"",0}};

	ifstream netw("/var/log/proftpd/proftpd.log");
	netw.seekg(0,ios::end);				//ustawia pozycje czytania na koncu pliku
        poprzedniapoz = netw.tellg();
	netw.close();
	
	while(1)
	{
		ifstream netw("/var/log/proftpd/proftpd.log");
		if (!netw) continue;
		
		netw.seekg(0,ios::end);				//ustawia pozycje czytania na koncu pliku
        	obecnapoz = netw.tellg();			// zapisuje do zmiennej pozycje w pliku
		if(obecnapoz > poprzedniapoz)		//jesli pojawily sie nowe linijki
        	{
			netw.seekg(poprzedniapoz);		//to ustawia pozycje na poprzednia
			poprzedniapoz=obecnapoz;		// przepisuje aktualna pozycje do pozycji poprzedniej
		}
		while(!netw.eof())
		{
			getline(netw,linia);
			if(linia.find("Incorrect password") != string::npos)
			{
				ip = linia.substr(linia.find("::ffff:")+7,linia.find("[::")-(linia.find("::ffff:")+7));
				bool var = false;
				for(int i=0; i<5; i++)
				{
					if(ip == players[i].ip)
					{
						if(players[i].czas >= 5)
						{
							cmd="/sbin/iptables -I INPUT -s "+ip+"/24 -j DROP";
							system(cmd.c_str());
				
							ofstream plik("/root/pc/bany.txt",ios::out|ios::app);
							plik << cmd << "		#"<<czas()<<" FTP Hacker" <<endl;
							plik.close();
							cout<<czas()<<" Zlapano hackera: "<<ip<<endl;
							players[i].ip="";
							players[i].czas=0;
						}
						else
						{
							players[i].czas++;
						}
						var = true;
						break;
					}
				}
				if(!var)
				{
					int i=0;
					for(i; i<5; i++)
					{
						if(players[i].ip == "")
						{
							players[i].ip = ip;
							players[i].czas = 1;
							break;
						}
					}
					if (i>=4)
					{
						players[0].ip = ip;
						players[0].czas = 1;
					}
						
				}

			}
			else if(linia.find("no such user found from") != string::npos)
			{
				ip = linia.substr(linia.find("::ffff:")+7,linia.find("[::")-(linia.find("::ffff:")+7));
				bool var = false;
				for(int i=0; i<5; i++)
				{
					if(ip == players[i].ip)
					{
						if(players[i].czas >= 5)
						{
							cmd="/sbin/iptables -I INPUT -s "+ip+"/24 -j DROP";
							system(cmd.c_str());
				
							ofstream plik("/root/pc/bany.txt",ios::out|ios::app);
							plik << cmd << "		#"<<czas()<<" FTP Hacker" <<endl;
							plik.close();
							cout<<czas()<<" Zlapano hackera: "<<ip<<endl;
							players[i].ip="";
							players[i].czas=0;
						}
						else
						{
							players[i].czas++;
						}
						var = true;
						break;
					}
				}
				if(!var)
				{
					int i=0;
					for(i; i<5; i++)
					{
						if(players[i].ip == "")
						{
							players[i].ip = ip;
							players[i].czas = 1;
							break;
						}
					}
					if (i>=4)
					{
						players[0].ip = ip;
						players[0].czas = 1;
					}
						
				}

			}
		}
		netw.close();
		sleep(1);
	}

}


int main() {

	pthread_t t1, t2, t3, t4;

	pthread_create (&t1, NULL, &snif, NULL); 
	pthread_create (&t2, NULL, &gtacc, NULL); 
	pthread_create (&t3, NULL, &hacker, NULL); 
	pthread_create (&t4, NULL, &ftp, NULL);
	pthread_join (t1, NULL);
	pthread_join (t2, NULL);
	pthread_join (t3, NULL);
	pthread_join (t4, NULL);
	
	return 0;
}
