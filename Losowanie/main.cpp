#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <conio.h>
using namespace std;

bool wylosowana(int *tab, int liczba, int rozm)
{
	for (int i=0 ; i<rozm; i++)
		if (tab[i] == liczba)
			return true;
	return false;
}

int main()
{
	int tab[200];
	srand (time(NULL));
	string chodza[200];
	string niechodza[200];
	string linia;
	bool var = true;
	int rozm=0;
	int i=0,j=0;
	ifstream plik("lista.txt");
	if (!plik)
	{
		cout <<"Nie ma listy braci!"<<endl;
		getch();
		return 0;
	}
	
	while(!plik.eof())
	{
		getline(plik,linia);
		if (linia.find("AKTYWNI") == string::npos && !linia.empty() && linia.find("NIEAKTYWNI") == string::npos)
		{		
			if (var)
			{
			
				chodza[i]=linia;
				i++;
			}
			else
			{
				niechodza[j]=linia;
				j++;
			}
		}
		else 
			if (linia.find("NIEAKTYWNI") < linia.length())
					var = false;
	}
	plik.close();
	int ile;
	cout <<"Podaj na ile grup podzielic liste: "<<endl;
	cin >> ile;
	int liczba;
	
	
	for(int k=0; k<i; k++)
	{
		do
		{
			liczba = (rand() % i );
		}
		while(wylosowana(tab,liczba,rozm));
		
		tab[rozm] = liczba;
		rozm++;
		cout << chodza[liczba]<<endl;
		
	}
	cout <<endl;
	rozm=0;
	for (int k=0; k<j; k++)
	{
		
		do
		{
			liczba = (rand() % j );
		}while(wylosowana(tab,liczba,rozm));
		tab[rozm] = liczba;
		rozm++;
		cout <<niechodza[liczba]<<endl;
	}
	
	getch();
	return 0;
}
