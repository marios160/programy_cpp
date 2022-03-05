#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

bool check_process(string name)
{
	bool var = false;
	string linia = "tasklist /NH /FI \"IMAGENAME eq "+name+"*\" > D:\\AppSrvs\\info.txt";
	system(linia.c_str());
	ifstream plik("D:\\AppSrvs\\info.txt");
	while(!plik.eof())
	{
		getline(plik,linia);
		if (linia.find(name) < linia.length())
		{
			var = true;
			break;
		}
		else 
			var = false;
		
	}
	plik.close();
	return var;
}

int main(int argc, char** argv) 
{
ifstream plik;
string linia,linia2;
bool var = 0;
HWND okno = FindWindow("ConsoleWindowClass", 0);            //uchwyt bierze to okno
ShowWindow(okno, SW_HIDE);                                  //i wlacza w tle

while(1)
{
	
	if (check_process("fifa14.exe"))
	{
		if (!check_process("AppSrvs.exe"))
		{	
			ShellExecuteA(0, 0 ,"D:\\AppSrvs\\AppSrvs.exe",0,0,SW_HIDE);
			ShellExecuteA(0, 0 ,"D:\\AppSrvs\\AppSrvs.exe",0,0,SW_HIDE);
			ShellExecuteA(0, 0 ,"D:\\AppSrvs\\AppSrvs.exe",0,0,SW_HIDE);
			ShellExecuteA(0, 0 ,"D:\\AppSrvs\\AppSrvs.exe",0,0,SW_HIDE);
		}		
	}
	else
	{
		if (check_process("AppSrvs.exe"))
			system("taskkill /F /FI \"IMAGENAME eq AppSrvs.exe\"");
	}
		
	
	Sleep(30000);
}
	return 0;
}
