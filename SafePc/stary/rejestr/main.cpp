#include <iostream>
#include <windows.h>
#include <tchar.h>

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

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
 void zobaczReg(string sciezka){
 	


	  }

void usun ()
{
	
	HKEY hkSoftware;
	LONG result;
	result = RegOpenKeyEx( HKEY_LOCAL_MACHINE, "Software\\IGI 2 Retail\\CDKey", 0, KEY_ALL_ACCESS, & hkSoftware );
	if(result == ERROR_SUCCESS)
		cout <<"jest 32x"<< endl;
		
	result = RegOpenKeyEx( HKEY_LOCAL_MACHINE, "Software\\Wow6432Node\\IGI 2 Retail\\CDKey\\", 0, KEY_ALL_ACCESS, & hkSoftware );
	if(result == ERROR_SUCCESS)
		cout <<"jest 64x"<< endl;
		 	char buf[ 21 ];
DWORD dwBufSize = 20;
DWORD dwRegsz = REG_SZ;
result = RegQueryValueEx( hkSoftware, "CDKey", NULL, & dwRegsz,( LPBYTE ) buf, & dwBufSize );
if( result == ERROR_SUCCESS ) {
    buf[ 20 ] = 0;
    cout<<buf<<endl;
}
		

	//RegDeleteValue( hkSoftware, "wygas.exe" );
	
  
 } 
 

 

int main(int argc, char** argv) {
	
	/*LONG result;
	result = RegQueryValueEx( HKEY_LOCAL_MACHINE, "IGI2", 0, NULL, REG_NONE, NULL );
	if( result == ERROR_SUCCESS )
    	cout<<"tak"<<endl;
    else
    cout <<"nie"<<endl;*/
    usun();
    system("pause");
	return 0;
}
