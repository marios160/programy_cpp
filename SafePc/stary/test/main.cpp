#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
using namespace std;
int main(int argc, char** argv) {
	
	/*string linia;
	ifstream plik3("tmp");
	ofstream plik4("tmp2",ios::out|ios::trunc);
	getline(plik3,linia);
	plik4 << linia;
	while(!plik3.eof())
	{
		getline(plik3,linia);											
		plik4 <<  linia;			
	} 
	plik3.close();
	plik4.close();*/
	if(ShellExecuteA(0, 0 ,"Limit.exe",0,0,SW_SHOW) >32)
		cout<<"aaaaaaaaaaa"<< endl;
		else
		cout<<"BBBBBBBBBBBBBBB" << endl;
		return 0;
		system("pause");
		system("pause");
}
