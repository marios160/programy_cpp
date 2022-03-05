#include <iostream>
#include <windows.h>

using namespace std;


int main() {
	
	char *user = getenv( "USERNAME" );											//wczytujemy nazwe u¿ytkownika
    string nazwa(user);
	string cmd="taskkill /FI \"USERNAME ne SYSTEM\" /FI \"IMAGENAME eq csrss*\"";		//zapisuje polecenie dodajac zdekodowana linie
		// FI - filtr, IMAGENAME - nazwa okna, T - wy³¹czanie podprocesów, F - wymuszenie zamkniêcia
	system(cmd.c_str());
	//cmd="taskkill /U"+nazwa+" /FI \"IMAGENAME eq csrss.exe\" /T /F";		//zapisuje polecenie dodajac zdekodowana linie
	//system(cmd.c_str());
	system("pause");
	return 0;
}
