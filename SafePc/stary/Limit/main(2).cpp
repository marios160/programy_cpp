#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <sstream>
#include <math.h>
#include <iomanip>
#include <ctime>


using namespace std;
string jaka_data(int czas=0);
//------------------------------------------------
/*tworzy loga programu*/
int loguj(string linia)
{
	char *user = getenv( "USERNAME" );											//wczytujemy nazwe u¿ytkownika
    string nazwa(user);
	string linijka = jaka_data(1)+"<Sprawdz_limit> "+linia+"\n";
	string sciezka = "D:\\fesf4353g542fs\\cr\\"+nazwa+".log";
	ofstream log(sciezka.c_str(),ios::out|ios::app);
	log << linijka;
	log.close();

	
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
//--------------------------------------------------
/*sprawdzanie username i tworzenie sciezki*/
void pathname(string &username, string &sciezka)
{
	char *user = getenv( "USERNAME" );											//wczytujemy nazwe u¿ytkownika
    string nazwa(user);
    for(int j=0; j < nazwa.length(); j++)
	{
		if (nazwa[j] == ' ')					 								//jesli nazwa uzytkownika ma spacje
			nazwa[j] = '_';														//to zmieniamy je na pod³oge
	}
	username = nazwa;
    sciezka = "D:\\fesf4353g542fs\\cr\\"+username;			//zapisujemy sciezke
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

//---------------------------------------------------
/* konwertuje STRING na INT*/
int strconv(string value)
{
	int i;
	istringstream iss(value);
	iss >> i;
	return i; 
}
//--------------------------------------------------
/*pobiera dane z pliku*/
int pobierz(string sciezka, int &limit, int &aktlimit)
{
	
	string linia;
	int l=0;
		/*
			tab[pozp] - username
			tab[pozp+1] - limit w minutach
			tab[pozp+2] - aktlimit	w sekundach
			tab[pozp+3] - data
			tab[pozp+4] - reg
			tab[pozp+5] - smss
			tab[pozp+6] - startapk
			tab[pozp+7] - aplikacje.....
			tab[pozp+....] -  aplikacje
			tab[pozk] - endapk
		*/
	ifstream plik(sciezka.c_str());
	if (!plik)																	//jesli go nie ma
    {	
    		cout << "Nie ma pliku!" <<endl;
    		system("pause");
			return 0;															//blad, brak pliku!
	}
	getline(plik,linia);
	getline(plik,linia);
	limit = strconv(decode(linia));
	getline(plik,linia);
	aktlimit = strconv(decode(linia));
	plik.close();
		
}
//---------------------------------------------------



int main(int argc, char** argv) {
	
		int minut,godzin,limit=0,aktlimit=0;
	string linia,username,sciezka;
	
	SetConsoleTitle("Look Limit");
	pathname(username,sciezka);
	while(1)
	{
		pobierz(sciezka, limit, aktlimit);					//pobieramy dane
		loguj("Sprawdzono limit: "+intconv(limit)+", aktlimit: "+intconv(aktlimit));
		system("cls");
		cout << "\n\n\n\n\n\n\n\n" << endl;
		cout << setw(43) << "Uzytkownik: " << username << "\n" <<endl;
		cout << setw(39) << "Limit: " << limit/60 << " godz.\n" << endl;
		godzin = aktlimit / 3600;
		minut = (aktlimit % 3600)/60;
		cout << setw(36) << "Zostalo: " << godzin <<" godz. i " << minut << " min." << endl; 
		Sleep(60000);
		aktlimit-=60;
		
		
	}
		
	return 0;
	
}
