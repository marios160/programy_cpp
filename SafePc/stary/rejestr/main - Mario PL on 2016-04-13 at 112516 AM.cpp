#include <iostream>
#include <windows.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
using namespace std;
void autostart() 
{
    /* DODAWANIE DO REJESTRU */
    HKEY klucz;
    LONG Wynik;
    DWORD Nowy;
    const BYTE * lpData =( const BYTE * ) "C:\\windows\\wygas.exe"; 
    Wynik = RegCreateKeyEx( HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 
				0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, & klucz, & Nowy );
    if( Wynik == ERROR_SUCCESS )
    {
        RegSetValueEx( klucz, "wygas.exe", 0, REG_SZ, lpData, 256 );
        RegCloseKey( klucz );
    }
}  

void usun ()
{
	
	HKEY hkSoftware;
	LONG result;
	result = RegOpenKeyEx( HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, & hkSoftware );
	RegDeleteValue( hkSoftware, "wygas.exe" );
  
 } 
int main(int argc, char** argv) {
	LONG result;
	result = RegQueryValueEx( HKEY_LOCAL_MACHINE, "IGI2", 0, NULL, REG_NONE, NULL );
	if( result == ERROR_SUCCESS )
    	cout<<"tak"<<endl;
    else
    cout <<"nie"<<endl;
	return 0;
}
