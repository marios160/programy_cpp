
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <cstdlib>

using namespace std;

int main()
{
    SYSTEMTIME st, lt;
    
    GetSystemTime(&st);
    GetLocalTime(&lt);
    
    printf("The system time is: %02d:%02d\n", st.wHour, st.wMinute);
    printf(" The local time is: %02d:%02d\n", lt.wHour, lt.wMinute);
    string s;
	s=itoa(32,(char*)s.c_str(),10);
    cout <<s<<endl;
    return 0;
}
