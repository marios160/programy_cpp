#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <ctime>
#include <cstdlib>
#include <conio.h>

using namespace std;
//---------------------------------------------------
/* losuje string do zakodowania*/

string losuj()
{	
	string kod="";																
	for (int i=0;i<20;i++)
	{
		kod+=char((rand()%94)+33);												//petla dodaje do stringa losowy znak
	}
	return kod;
}

//--------------------------------------------------------

void code(int n, string tab[])
{
	int j;
	for(int i=0;i<n+1;i++)
	{
		j = 0;
		while(j < tab[i].length())
		{
			tab[i][j]=tab[i][j]+97;
		
			j++;		
		}
		tab[i]=losuj()+tab[i]+losuj()+tab[i]+losuj();
		
	}
}

void decode(int n, string tab[])
{
	int j;
	for(int i=0;i<n;i++)
	{
		j = 0;
		tab[i] = tab[i].substr(20,(tab[i].length()-60)/2);
		while(j < tab[i].length())
		{
			tab[i][j]=tab[i][j]-97;
			j++;		
		}		
	}
}

int main()
{
	string pass;
	char znak;
	while(true) { 
                znak = getch(); 
                if(znak == '\n' || znak == '\r' || znak == -1) 
                        break; 

                pass += znak; 
                putch('*'); 
        } 	
	if (pass != "tastee")
		return 0;
	system ("cls");
	srand(time( NULL ));
	int n=0;
	string nazwa,linia;
	cout<<"Podaj nazwe pliku"<<endl;
	cin >> nazwa;
	
	ifstream plik(nazwa.c_str());
	if (!plik)
	{
		cout << "Nie ma takiego pliku" << endl;
		return 0;
	}
		
	while(!plik.eof())
	{
		getline(plik,linia);
		n++;	
	}
	plik.close();
	string tab[n+1];
	plik.open(nazwa.c_str());
	for (int i=0;i<n;i++)
	{
		getline(plik,tab[i]);			
	}
	plik.close();
	int wybor;
	cout<<"Zdekodowac 1, zakodowac 2 :"<<endl;
	cin >> wybor;
	if (wybor == 1)
	{
		decode(n,tab);	
	}
	else if (wybor == 2)
	{
		code(n,tab);
	}
	
	ofstream plik2(nazwa.c_str(),ios::out|ios::trunc);
	for(int i=0;i<n;i++)
	{
		if (i == 0)
			plik2 << tab[i];
		else
			plik2 << "\n" << tab[i];
			
	}
	plik2.close();
	cout << "Gotowe!" << endl;
	return 0;
}


