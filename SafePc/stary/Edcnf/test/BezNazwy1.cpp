#include <iostream>
#include <cstdlib>
#include <vector>
#include <sstream>
using namespace std;
string *tab;
/*
void a()
{
	delete [] tab; 
	tab = new string [10];
	tab[0]="0";
}
*/
//---------------------------------------------------
/* konwertuje INT na STRING*/
string intconv(double value)
{
	ostringstream ss ;
	ss << value;
	return ss.str();
}

//---------------------------------------------------
/* konwertuje STRING na INT*/
double strconv(string value)
{
	double i;
	istringstream iss(value);
	iss >> i;
	return i; 
}

string minuty(string linia)
{
	double liczba;
	liczba = strconv(linia);
	liczba=liczba/60.00;
	
	return intconv(liczba);
}

int main()
{
	//system("notepad.exe tmp");
	/*int w;
	while (1)
	{
	cin >> w;
	switch(w)
	{
		case 1:
			cout << "asdasD" << endl;
			break;
		case 2:
			break;
		default:
			cout <<"koniec" << endl;
			break;
	}
	if (w==2)
		break;
	}
	cout << "koniec " << endl;
	system ("pause");*/
	
	/* 
	tab = new string [100];
	
	tab[0]="1";
	tab[1]="2";
	tab[50]="51";
	cout << tab[0] << endl;
	a();
	cout << tab[0] << endl;
	//cout << tab[51] << endl;
	delete [] tab;
	*/
	string linia="2343";
	linia=minuty(linia);
	cout << linia << endl;
	
	
}
