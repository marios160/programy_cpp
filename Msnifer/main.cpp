#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <string.h>
#include <time.h>
#include <sstream>
#include <winsock2.h>
#include <ws2tcpip.h>
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
#include <windows.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdio.h>
#include <winsock2.h> //niekoniecznie
#include <pcap.h>  //pliki nag³ówkowe winpcap
#include <remote-ext.h>




using namespace std;


struct Dane
{
	string ip;
	long czas;
	int port;
};

WSADATA WSD;
SOCKET sock;
struct sockaddr_in Addr;
int port;	
char *nameInterface;
string localip, remoteip;


Dane player;
string linia,arcon="",jrcon="";
time_t rawtime;
int bronie=92;

char info[] =
            "\xde\xad\xbe\xef\xab\xab\xab\xab\xab\xab\xab\xab\x0\x0\x0\x0\x05\xab\xab\xab\xba\xde\xab\xee",    
        pck1[] =
            "\xde\xad\xbe\xef\xab\xab\xab\xab\xab\xab\xab\xab\x00\x01\x00\x14\x00\xab\xab\xab\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xba\xde\xab\xee",
		pck2[] =
            "\xde\xad\xbe\xef\xab\xab\xab\xab\xab\xab\xab\xab\x00\x03\x00\x14\x00\xab\xab\xab\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xba\xde\xab\xee",
        pck3[] =
			"\x00\x14\x00\xab\xab\xab";


void packet_handler(u_char* param,const struct pcap_pkthdr* header,const u_char* pkt);
string send(char *cmd);

		
void* snif()//void* unused)
{
	pcap_if_t *alldevs,*d; //lista urz¹dzeñ

    u_int inum,i=0;
    char errbuf[PCAP_ERRBUF_SIZE]; //buffor na kody b³êdów
    int res;
    u_int netmask;  //maska sieci potrzebna przy filtrowaniu pakietów
    struct bpf_program fcode;
	
	char *dev="eth4";//nameInterface; //nazwa urzadzenia
	pcap_t* fp;
	u_int ip_addr;
	char packet_filter[]="udp";


	
	if(pcap_lookupnet(dev,&ip_addr,&netmask,errbuf)==-1)
	{	
		printf("Nie mozna uzyskac maski: %s\n",errbuf);
		ip_addr=0;
		netmask=0;
	}
//otwieramy urzÃ‚Â¹denie


if(pcap_findalldevs_ex(PCAP_SRC_IF_STRING,NULL,&alldevs,errbuf)==-1)
    {
        fprintf(stderr,"Blad w pcap_findalldevs_ex: %s\n",errbuf);
        exit(1);
    }

	printf("Dostepne urzadenia:\n");
	
	//Pokazywanie listy urz¹dzeñ
    for(d=alldevs;d;d=d->next)
    {
    	
        printf("%d. %s\n    ",++i,d->name);
        if(d->description)
            printf(" (%s)\n",d->description);
        else
            printf(" (Brak opisu)\n");
   }
    /*if(i==0)
    {
        printf("\nBrak urzadzen\n");
        exit(1);
    }
	*/
	/*printf("Podaj numer urzadzenia (1-%d): ",i);
	scanf("%d",&inum);
	
	if((inum<1)||(inum>i))
	{
	    printf("Numer z poza przedzialu.\n");
		pcap_freealldevs(alldevs);
		exit(1);
	}*/
	
	//wybórk konkretnego sprzetu
    //for(d=alldevs,i=0;i<inum-1;d=d->next,i++);
	
	//otwarcie urz¹denia
    
#ifdef WIN32        
    if((fp=pcap_open("any",//nazwa urzadenia
                            65536, //wielkosc pakietu, 65536 gearantuje ze zostanie przeczytany ca³y pakiet
                            PCAP_OPENFLAG_PROMISCUOUS, //promiscuous mode //jakby komuœ nie dzia³a³o daje 0
                            1000, //czas oczekiwania
                            NULL,errbuf))==NULL)
	{
	    fprintf(stderr,"\nNie mo¿na otworzyæ urzadenia. %s nie obslugiwane przez WinPcap\n");
        pcap_freealldevs(alldevs);
        exit(1);
    }
#else
	if((fp=pcap_open_live(dev,65536,0,1000,errbuf))==NULL)
	{
		printf("Nie moÃ‚Â¿na otworzyÃƒÂ¦ urzÃ‚Â¹dzenia: %s\n",errbuf);
		//exit(1);
		system("pause");
	}
#endif*/
//kompilacja filtra
	if(pcap_compile(fp,&fcode,packet_filter,0,netmask)<0)
	{
		printf("SkÃ‚Â³adnia filtru jest niepoprawna.\n");
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
	//pcap_freealldevs(alldevs);
	pcap_loop(fp,0,packet_handler,NULL); //i to jest funkcja przechwytujÃ‚Â¹ca pakiety
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
	cout<<pkt<<endl;
	time (&rawtime);
	ipheader* ih;
	udpheader *ud;
	sockaddr_in source_addr,dest_addr;
	ih=(struct ipheader*)(pkt+16); //pomijamy 16 bajtÃƒÂ³w 
	ud=(struct udpheader*)(pkt+36);
	
	source_addr.sin_addr.s_addr=ih->ip_src;
	source_addr.sin_port = ud->udph_srcport;
	dest_addr.sin_addr.s_addr=ih->ip_dst;
	dest_addr.sin_port = ud->udph_destport;

	string sourceIP = inet_ntoa(source_addr.sin_addr);
	string destIP = inet_ntoa(dest_addr.sin_addr);
	int port = htons(source_addr.sin_port);

	char rcon[]="NOPASSNO", id[]="11";



	if (sourceIP == remoteip) 			//bronie
	{
		if(pkt[72]==80 && pkt[73]== 79 && pkt[74] == 82 && pkt[75] == 68) // DROP - PORD 
			bronie--;
		else
		if(pkt[72]==75 && pkt[73]== 67 && pkt[74] == 73 && pkt[75] == 80) // PICK - KCIP
			bronie++;
	}
	else  
	if (destIP == remoteip)
	{
		
//--------------------------ANTYALUIGI------------------------------------------------------------------
		
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
					if(player.port != port)
					{
						#ifdef WIN32
							string cmd = "netsh advfirewall firewall add rule name=\"CRASHER\" dir=in protocol=udp interface=any action=block remoteip="+sourceIP+"/32";
						#else
							string cmd = "/sbin/iptables -I INPUT -s "+sourceIP+" -j DROP";
						#endif
						system(cmd.c_str());	
						string cmd1 = "Mautorun crasher: "+sourceIP;
						char *msg = (char*)cmd1.c_str();
						send(msg);
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
	
//------------------BRONIE--------------------------------------------------------------------------------------

		
		
//-------------------------RCONY---------------------------------------------------------------------
	
		if(pkt[72]==84 && pkt[73]== 83 && pkt[74] == 73 && pkt[75] == 76) 		//jesli TSIL - LIST
	     {
			if(pkt[128]==114 && pkt[129]== 99 && pkt[130] == 111 && pkt[131] == 110) 	//jesli rcon
			{
				strncpy(rcon,(char*)pkt+135,8);
				strncpy(id,(char*)pkt+88,1);
				string sid(id);
				string srcon(rcon);
				if(srcon.rfind("\"") < 8)
				{
					srcon=srcon.substr(0,srcon.rfind("\""));
				}
				if(srcon == arcon)
				{
					string cmd = "Mautorun aadmin "+sid;
					char *msg = (char*)cmd.c_str();
					send(msg);
				}
				else
				if(srcon == jrcon)
				{	
					string cmd = "Mautorun ajadmin "+sid;
					char *msg = (char*)cmd.c_str();
					send(msg);
				}
			}
	     }
		else
		
//----------------------CZAT-------------------------------------------------------------------------------------

		if(pkt[72]==84 && pkt[73]== 65 && pkt[74] == 72 && pkt[75] == 67) //TAHC - CHAT
		{
			char *wiadomosc;
			strncpy(id,(char*)pkt+64,1);
			string sid(id);
			int k;
			for(int i=79; i<310; i++)
				if(pkt[i] == 10)
					k=i;
			strncpy(wiadomosc,(char*)pkt+79,k-79);
			
			string cmd = "Mautorun chat ["+sid+"] "+string(wiadomosc);
					char *msg = (char*)cmd.c_str();
					send(msg);
			
		}
	}




}



//g++ main.cpp -o 3in1 -lpcap -lpthread
//./snif venet0 udp
//apt-get install libpcap0.8 libpcap-dev			



///////////////////////////////////////////////////////////////////////////////////////////////////////////////


string send(char *cmd)
{	
	char odp[3048]={0};
	sock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	if (WSAGetLastError() != 0 ) 
		cout <<"Creating UDP socket"<<endl; 
	int addrlen;
  	sendto( sock, cmd, strlen(cmd), 0, (struct sockaddr*)&Addr, sizeof( struct sockaddr ) );
 	if (recvfrom(sock, odp, sizeof(odp), 0,NULL, NULL) > 0);
	closesocket(sock);
	return string(odp);
}

int main(int argc, char** argv)
{
		
	/*localip = (argv[1]);
	port = atoi(argv[2]);
		
	WSAStartup(MAKEWORD( 2, 0 ) , &WSD );
	if (WSAGetLastError() != 0 ) 
		cout <<"WSA-UDP"<<endl;                    
                                             
	Addr.sin_family = AF_INET; 
	Addr.sin_port = htons(port);
	Addr.sin_addr.s_addr=inet_addr(localip.c_str());
	nameInterface = "eth4";//(char*)send("Mautorun interface").c_str();
	arcon = send("Mautorun rcon");
	jrcon = send("Mautorun juniorrcon");
	remoteip = send("Mautorun remoteip");*/
	
	snif();
	system("pause");
	return 0;
}



