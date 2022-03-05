#include <iostream>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <process.h>
#include <Winuser.h>
#include <string>
#include <fstream>
using namespace std;

int random;
bool endrand = false;







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
	plik.open(filename.c_str(), ios::in);
	if(plik)
	{
		//losuj(1);
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
}

void dekoduj_plik(string filename)
{
	ofstream out("decode.txt", ios::out | ios::trunc);

	fstream plik;
	plik.open(filename.c_str(), ios::in);
	if(plik)
	{
		//losuj(1);
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
}

int main()
{
	//while(1)
	
		//losuj();
		//string ret = koduj("oto przyklad");
		//cout <<  ret << endl;
		//cout << dekoduj(ret) << endl << endl;
		int x;
		cin >> x;
		if (x == 1)
		koduj_plik("Mateusz");
		else
		dekoduj_plik("Mateusz");
	


	
}
