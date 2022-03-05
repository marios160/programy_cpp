#include <iostream>
#include <windows.h>
#include <iomanip>
#include <string>


using namespace std;

int main(int argc, char *argv[])
{
    SetConsoleTitle("Kontrola czasu!");
   
    int a = atoi(argv[1]);  	
	string s =argv[2];
	int b = atoi(argv[3]);  
	
    for(int i = a; i >= 0; i--)
    {
    	
    	
        system("cls");
        cout<<"\n\n\n\n\n\n\n" <<setw(s.length()+10)<<s<<
                
                "\n\n\n";
        if (b)
			cout<<"                         Za "<<i<<" sek wyloguje uzytkownika!"<<endl;
		else
			cout<<i<<endl;
        Sleep(1000);
    }
    return EXIT_SUCCESS;
    
}
