#include <iostream>

using namespace std;

int main()
{
   int tabSize;
   cout<< "Podaj rozmiar tablicy"<<endl;
   cin>>tabSize;
  int **tab= new int*[tabSize];
  for(int  i=0; i<tabSize;i++ ){
    tab[i]=new int [tabSize];
}
  cout<<"Podaj "<<tabSize*tabSize<<" liczb ca³kowitycz"<<endl;
  for(int i=0; i<tabSize; i++){
    for(int j=0; j<tabSize; j++){
        cin>>tab[i][j];
    }
  }
  for(int i=0; i<tabSize; i++){
    for(int j=0; j<tabSize; j++){
        cout<<tab[i][j]<<" ";
    }
    cout<<endl;
  }

}
