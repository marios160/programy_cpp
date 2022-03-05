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
string maintime;
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


string intconv(int value)									//zmiana int na string
{
	ostringstream ss ;
	ss << value;
	return ss.str();
}

//-----------------------------------------------------
/* otwiera okienko z danym bledem */
int perror (int var)
{
	string path = sciezka+"dec.exe";								//przygotowujemy sciezke do programu
	string cmd="shutdown /l ";										// komenda wylogowujaca uzytkownika
	switch (var)
	{											
		case 1: ShellExecuteA(0, 0 ,path.c_str(),					//jesli limit wykorzystany			
			"60 \"Limit wykorzystany!\" 1",0,SW_SHOW);
			loguj("perror(1): Koniec limitu");
			Beep(2000,1000);										//pipczymy
			Sleep(60000);											//usypiamy na 60 sek
			loguj("Wylogowuje!");
			system(cmd.c_str());	
			break;													//wylogowujemy
		case 2: ShellExecuteA(0, 0 ,path.c_str(),			
			"5 \"Limit wykorzystany! Sprobuj jutro.\" 1",0,SW_SHOW);		//jesli zalogowalismy sie po wykorzystaniu limitu
			loguj("perror(2): Proba logowania po wykorzystanym limicie");
			Beep(2000,1000);
			Sleep(20000);
			loguj("Wylogowuje!");
			system(cmd.c_str());		
			break;													//wylogowujemy																					//to wylogowujemy
		default: ShellExecuteA(0, 0 ,path.c_str(),			
			"60 \"Blad nieznany SafePc! Zawolaj Mateusza!\" 1",0,SW_SHOW);
			loguj("perror(default): Nieznany blad!");
			Beep(2500,5000);
			Sleep(60000);
			loguj("Wylogowuje!");
			system(cmd.c_str());	
			break;											//wylogowujemy
			
	}
}
//----------------------------------------------------------
/* konwertuje date do wygodnego typu string
jesli w argumencie damy 1 funkcja zwroci date i czas*/

string jaka_data(int czas)
{
	string data;
    time_t now = time(0);
    tm *ltm = localtime(&now);
    
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
    	data = data_czas2[0]+data_czas2[1]+data_czas2[2];
    return data;
}

//------------------------------------------------
/*tworzy loga programu*/
int loguj(string linia)
{
	string linijka=jaka_data(1)+"<IncognitoN>"+linia+"\n";	
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
	plik2 << koduj("+_)(*&^%$#@!~") << "\n";				//z domyslnymi ustawieniami
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
		return 0;										//blad, brak pliku!
	}
	var = 1;		
	getline(plik,linia);
	linia=dekoduj(linia);
	if (linia.find("+_)(*&^%$#@!~") == 0 )				//jesli znajdziemy pierwsza linie						
	{
		var = 2;
		getline(plik,linia);
		linia=dekoduj(linia);
		if (linia.find("@"+user) == 0 )					//nazwa uzytkownika 
		{
			var = 3;
			getline(plik,linia);
			linia=dekoduj(linia);
			if (linia.find("*") == 0 )					//limit
			{
				var = 4;
				getline(plik,linia);
				linia=dekoduj(linia);
				if (linia.find("*") == 0 )				//aktualny limit					
				{
					var = 5;
					getline(plik,linia);
					linia=dekoduj(linia);
					if (linia.find("*") == 0 )			//data			
					{
						return 1;					//jesli wszystko pasuje to plik poprawny
					}
				}
			}
		}
	}					//jesli cos nie pasuje to plik niepoprawny
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
	char *username = getenv( "USERNAME" );								//wczytujemy nazwe u¿ytkownika
    string nazwa(username);
    for(int j=0; j < nazwa.length(); j++)
	{
		if (nazwa[j] == ' ')					 						//jesli nazwa uzytkownika ma spacje
			nazwa[j] = '_';												//to zmieniamy je na pod³oge
	}
	user = nazwa;
	char *user2 = getenv( "COMPUTERNAME" );								//wczytujemy nazwe komputera
    string nazwa2(user2);
	computer= nazwa2;
}

//---------------------------------------------------------------------
/* pobiera trasc strony podanej w parametrze*/

bool pobierzWWW(string &adr, string strona)
{	
	DWORD dwError;
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        loguj("Blad WSADATA ("+strona+")");
        return false ;
    }
    SOCKET Socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    struct hostent *host;
    host = gethostbyname("igi2.xaa.pl");
    if (host != NULL) 
	{
		loguj("Nawiazano polaczenie z internetem");
        return 1;
    }
	closesocket(Socket);
    WSACleanup();
    return false;
}

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
		createuser();
	}
	ifstream plik(path.c_str());
	for(int i=0; i<5; i++)											//zapisujemy caly plik do tablicy
	{
		getline(plik,linia);
		tab[i] = dekoduj(linia);
	}
	plik.close();
	
	limit = atoi(tab[2].substr(1).c_str());							//ustawiamy limit - w minutach
	if (limit > 1440)												//nie mozliwy jest limit ponad 24h
	{
		limit=0;													//to znaczy ze cos jest nie tak wiec 0
		loguj("Limit wynosil ponad 24 godziny");
	}
	aktlimit = atoi(tab[3].substr(1).c_str());
	if (aktlimit > limit*60)										//jak aktlimit jest wiekszy niz limit
	{
		aktlimit = limit*60;										//to znaczy ze cos jest nie tak
		loguj("Aktualny limit wynosil wiecej niz limit");
	}																//wiec ustawiamy taki jak jest limit	
	if (tab[4].substr(1) != jaka_data(0))
	{
		tab[4] = "*" + jaka_data(0);										//zapisujemy aktualna date
		loguj("Inna data, resetowanie limitu");					//to mozna resetowac limit
		aktlimit = limit*60;
															//jesli jest error to nie resetujemy
	}
}
//---------------------------------------------------
/*wyslij dane do pliku*/
void zapisz()
{	
	tab[3] = "*" + intconv(aktlimit);							//do tablicy dajemy aktualny limit
	ofstream plik(path.c_str(),ios::out|ios::trunc);           	//otwiera plik konfiuguracyjny
	for(int i=0;i<5;i++)
	{
			plik << koduj(tab[i])<<"\n";						//kodujemy i do pliku		
	}		   
    plik.close();  
    
}

//------------------------------------------------------
/*liczenie czasu*/

void licz()
{
	loguj("Liczenie limitu bez internetu jest uruchomione");
	time_t tnow, tstart;
	tstart=time(0);
	string change,path = sciezka+"Incognito.exe";
	int i=0;
    while( aktlimit > 0)	                                              //dopoki aktualny limit jest powyzej zera 
    {   																//petla sie wykonuje										                                                                                           
        	Sleep(5000);												//usypiamy na 5 sekund   
        	if(pobierzWWW(change,"incognito/h.php"))
        	{
        		ShellExecuteA(0, 0 ,path.c_str(),0,0,SW_SHOW);
    			exit(0);
			}
        	wczytaj();		                                 							//odejmuje od czasu dzialania 5 sek
        	tnow=time(0); 								//zapisujemy aktualna godzine
			aktlimit-=tnow-tstart; 						//odedjmujemy roznice czasow
			tstart=tnow; 				
			zapisz();
    }
    perror(1);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//============================================================================================================
//============================================================================================================
//============================================================================================================
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
	
	SetConsoleTitle("IncognitoN");   												//ustawiamy nazwe okna
    //ukryj_okno();	
	if (argc>=2)												//jesi program zostal wlaczony z argumentem
		sciezka = argv[1];				  						//to ustawiamy  nowy aktlimit!

	cout<<sciezka<<endl;															//ukrywamy okno
	pathname();					
	path = sciezka+user;
    loguj("#####################["+jaka_data(0).substr(0,2)+"-"+jaka_data(0).substr(2,2)+"-"+jaka_data(0).substr(4,2)+"]#######################");
    loguj("SafePc bez internetu uruchomiony!");  																										  
	limit=0;
	aktlimit=0;			
	srand(time( NULL ));
	tab = new string[5];												//tworzymy tablice																			
			
				
										//pobieramy plik z FTP
	wczytaj();											//pobieramy dane
	zapisz(); 	
	if (aktlimit <= 0)					//jesli limit jest wyorzystany
	{									//to			
		perror(2);						//wylogowujemy
	}
	else								//jesli limit nie jest przekroczony
	{			 			
		licz();							//odpalamy funkcje liczaca czas
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

