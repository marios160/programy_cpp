#include <iostream>
#include <windows.h>

using namespace std;


int main() {
	cout << "Wygaszacz ekranu - Mateusz Blaszczak\n"<<endl;
	cout << "Mozesz wylaczyc program"<<endl;
	SendMessage( HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, 2 );
	return 0;
}
