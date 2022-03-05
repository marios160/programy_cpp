//Program do zamykana procesow


#include <windows.h>
//#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

//---------------------------------------------------
/*koduje string*/
string code(string linia)
{
	string kod="#%#G%#$%T$%YBRT&UGBTW$%SEF@#54h54";								//ustalony kod 33 znaki
	string kod2="!@V#TW#VY1^%VYBE%N&*wI%YaW$YQ#T#F";
	int i=0;									
	do																			//w petli zmieniamy znaki
	{
		linia[i]=linia[i]+96;													//na 96 do przodu w tablicy ascii
		i++;
	}while(i<linia.length());													//dopoki nie dojdziemy do konca stringa
	
	linia=linia+kod+linia+kod2;													//dodajemy kodzik zeby wiecej szyfru bylo
	return linia;
}


//---------------------------------------------------
/* rozkodowuje string*/

string decode(string linia)
{
	int i=0;
	string delinia = linia.substr(0,(linia.length()-66)/2);						//wyciagamy naszego stringa z kodu
	do
	{
		delinia[i]=delinia[i]-96;												//zmieniamy na 96 do tylu z tablicy ascii
		i++;
	}while(i<delinia.length());													//dopoki nie dojdziemy do konca stringa
	return delinia;
}


//----------------------------------------------------
/*listuje i zabija dzialajace procesy*/
/*void kill(string program)
{
	PROCESSENTRY32 proc32;  //deklaracja struktury
    HANDLE hSnapshot;       //uchwyt na CreateToolhelp32Snapshot

    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    //ustawiamy rozmiar struktury
    proc32.dwSize = sizeof(PROCESSENTRY32);

    //pierwsze wywolanie Process32First
    if(Process32First(hSnapshot, &proc32))
    {

        //glowna petla wyœwietlaj¹ca procesy przez Process32Next
        while(Process32Next(hSnapshot, &proc32))
        {
			TCHAR* bob =proc32.szExeFile;
    		string s = bob; 
    		if (s==program)
    		{
				string cmd="taskkill /FI \"IMAGENAME eq "+program+"\"";
				system(cmd.c_str());
				cout<<program<<endl;
            	
			}
			//cout<<"nie ma"<<endl;
        }
    }

    CloseHandle(hSnapshot);
	
}
*/

int main()
{
	SetConsoleTitle("ProcesControl");
	HWND okno = FindWindow("ConsoleWindowClass", 0);            				//uchwyt bierze to okno
    ShowWindow(okno, SW_HIDE);                                  				//i wlacza w tle
	char *user = getenv( "USERNAME" );											//wczytujemy nazwe u¿ytkownika
    string linia;
    string nazwa(user);
	for(int j=0; j < nazwa.length(); j++)
	{
		if (nazwa[j] == ' ')													//jesli nazwa uzytkownika ma spacje
			nazwa[j] = '_';														//to zmieniamy je na pod³oge
	}
	
    string sciezka="C:\\Program Files\\Common Files\\cr\\"+nazwa+"smss.dll";	//zapisujemy sciezke			
	
	while (1)																	//nieskonczona petla
	{
		Sleep(5000);															//usypiamy program na 5 sek
		ifstream cfg(sciezka.c_str());											//otwieramy plik konfiguracyjny
		if (!cfg)
		{
			system("shutdown /l ");
			return 0;															//jesli go nie ma to wylogowujemy
		}
		while (!cfg.eof())														//petla idzie do konca pliku
		{
			getline(cfg,linia);													//pobiera linie
			string cmd="taskkill /FI \"IMAGENAME eq "+decode(linia)+"\"";		//zapisuje polecenie dodajac zdekodowana linie
			system(cmd.c_str());												//wysylamy killa do cmd 
			//cout<<tab[j]<<endl;	
			//kill(decode(tab[j]));
		}
		cfg.close();															//zamykamy plik i od nowa
	}
    
    system ("pause");
    return 0;
}
