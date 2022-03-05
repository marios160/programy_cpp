#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>

using namespace std;
void logo()
{
 cout<<"###########################################\n"
       "##      WebBlock by Mateusz Blaszczak    ##\n"
       "##       Program do blokowania stron     ##\n"
       "##                  2014                 ##\n"
       "###########################################\n\n\n";  
}

int main()
{
    logo();
    string linia;
    ifstream hosts("C:\\Windows\\System32\\drivers\\etc\\hosts");
    ofstream tmp("C:\\Windows\\System32\\drivers\\etc\\tmp.txt",ios::out|ios::trunc); 
    while(!hosts.eof())
    {
        getline(hosts,linia);
        if (linia != "127.0.0.1	chomikuj.pl")
        {
            tmp << linia+"\n";   
        }   
    }    
    tmp.close();
    hosts.close();
    ofstream hosts2("C:\\Windows\\System32\\drivers\\etc\\hosts",ios::out|ios::trunc);
    ifstream tmp2("C:\\Windows\\System32\\drivers\\etc\\tmp.txt"); 
    while(!tmp2.eof())
    {
        getline(tmp2,linia);
        hosts2 << linia+"\n";      
    }    
    tmp2.close();
    hosts2.close();
    remove("C:\\Windows\\System32\\drivers\\etc\\tmp.txt");
    cout<<"Skonczone! Wcisnij ENTER"<<endl;
    getch();
    return 0;
}
