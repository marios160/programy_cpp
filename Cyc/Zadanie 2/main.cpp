#include <iostream>
#include <string.h>


using namespace std;

void convertCase(char text[]){
    for(int i=0; i<strlen(text);i++){
        int letter = (int)text[i];
        if(letter>=65 && letter <=90){
            letter+=32;
            text[i]=(char)letter;
        }
        else if(letter>=97 && letter <=122){
            letter-=32;
            text[i]=(char)letter;
        }
    }
};

int main()
{
    cout <<"Wpisz znaki: "<<endl;
    char text[20];
    cin>>text;
    convertCase(text);
    for(int i=0; i<strlen(text); i++){
        cout<<text[i];
    }
    return 0;
}
