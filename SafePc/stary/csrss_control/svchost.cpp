#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <windows.h>
#include <sstream>
#include <ctime>

using namespace std;

int i=1;
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

string jaka_data(int czas=0)
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
//------------------------------------------------
/*tworzy loga programu*/
int loguj(string linia)
{
	char *user = getenv( "USERNAME" );											//wczytujemy nazwe u¿ytkownika
    string nazwa(user);
	string linijka2 = jaka_data(1)+"("+nazwa+") "+linia+"\n";
	
		string linijka=jaka_data(1)+"<svchost> "+linia+"\n";
		string sciezka = "C:\\Program Files\\Common Files\\cr\\logs\\"+nazwa+".log";
		ofstream log(sciezka.c_str(),ios::out|ios::app);
		log << linijka;
		log.close();

	ofstream log2("C:\\Program Files\\Common Files\\cr\\svchost.log",ios::out|ios::app);
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
//--------------------------------------------------
/*sprawdzanie username i tworzenie sciezki*/
void pathname(string &username)
{
	char *user = getenv( "USERNAME" );											//wczytujemy nazwe u¿ytkownika
    string nazwa(user); 
    for(int j=0; j < nazwa.length(); j++)
	{
		if (nazwa[j] == ' ')					 								//jesli nazwa uzytkownika ma spacje
			nazwa[j] = '_';														//to zmieniamy je na pod³oge
	}
	username = nazwa;															//zapisujemy sciezke
}

//---------------------------------------------------
int perror (int var)
{
	switch (var)
	{
		case 1: ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\dec.exe",			
			"10 \"Proba 1 wylaczenia limitu! \n Wszystko jest zapisane\" 0",0,SW_SHOW);
			loguj("perror(1): Pierwsza proba wylaczenia limitu");
			Beep(2000,5000);
			Sleep(10000);
			break;
		case 2: ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\dec.exe",			
			"10 \"Kolejna proba! Zaraz wysle sms do Mateusza!\" 0",0,SW_SHOW);
			loguj("perror(2): Druga proba wylaczenia limitu");
			Beep(2500,5000);
			Sleep(10000);		
			break;			
		case 3: ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\dec.exe",			
			"10 \"Proba 3! Jeszcze raz a wyzeruje limit!\" 0",0,SW_SHOW);
			loguj("perror(3): Trzecia proba wylaczenia limitu");
			Beep(2500,5000);
			Sleep(10000);	
			break;										
		case 4: ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\dec.exe",			
			"10 \"Zeruje limit!\" 0",0,SW_SHOW);
			loguj("perror(4): Czwarta proba wylaczenia limitu, zerowanie limitu");
			Beep(3000,5000);
			Sleep(10000);			
			break;
		default:ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\dec.exe",			
			"30 \"Nieznany blad CP, Zawolaj Mateusza!\" 1",0,SW_SHOW);
			loguj("perror(default): Nieznany blad!");
			Beep(2000,5000);
			Sleep(30000);
			system("shutdown /l ");												
			break;
	}
}

//--------------------------------------------------
/*zabija procesy*/
int process(string username)
{	
		string linia;
		string cmd="tasklist /NH /FI \"IMAGENAME eq SafePc.exe\" /FI \"USERNAME eq "+username+"\" > C:\\windows\\look";	//zapisuje polecenie dodajac zdekodowana linie
		string cmd2="tasklist /NH /FI \"IMAGENAME eq updater.exe\" /FI \"USERNAME eq "+username+"\" >> C:\\windows\\look";
		// FI - filtr, IMAGENAME - nazwa okna, username - nazwa uzytkownika
		
		system(cmd.c_str());
		system(cmd2.c_str());													///wysylamy killa do cmd 
		ifstream plik("C:\\windows\\look");
		if(!plik)
		{
			loguj("Nie ma pliku look");
			//cout << "niema"<<endl;
			ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\SafePc.exe",			
			0,0,SW_HIDE);
		}
		while(plik.eof())
		{
			getline(plik,linia);
		if (linia.find("SafePc.exe")<linia.length())
		{
			return 0;
		}		
		else
		{
			
			if (linia.find("updater.exe")<linia.length())
			{
				loguj("Znaleziono updater, czekam 1 min");
				Sleep(60000);
				return 0;
			}
			else
			if (i==4)
			{
				perror(i);
 				ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\SafePc.exe",			
					"0",0,SW_HIDE);	
				i++;
			}
			else
			{
				//cout << "else" << endl;
				ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\SafePc.exe",			
					0,0,SW_HIDE);
				perror(i);
				i++;
			}		
				
			//cout << "222222" << endl;	
			
						
			
		}
		}
		plik.close();
		return 0;
}

//--------------------------------------------------

int main(int argc, char** argv) 
{
	SetConsoleTitle("Control SafePc");   												//ustawiamy nazwe okna
    ukryj_okno();	
    loguj("====================["+jaka_data()+"]=======================");
    loguj("svchost uruchomiony!");  
	string username;															//ukrywamy okno
	pathname(username);
	ShellExecuteA(0, 0 ,"C:\\Program Files\\Common Files\\cr\\SafePc.exe",			
			0,0,SW_HIDE);
	Sleep(1000);
	while(1)
	{
		process(username);
		Sleep(60000);
	}
	
	
	
	return 0;
}
