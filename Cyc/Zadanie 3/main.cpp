#include <iostream>
#include <cstring>

using namespace std;


float add(float x, float y)
{
    return x+y;
};
float subtract(float x, float y)
{
    return x-y;

};
float multiply(float x, float y)
{
    return x*y;

};
float divide(float x, float y)
{
    return x/y;

};

float calculate(float x, float y, char * operations[], unsigned int size)
{
    float result=0;
    for(int i=1; i<size; i++)
    {
        if (strncmp(operations[i], "add", strlen(operations[i])) == 0)
        {
            result+=add(x,y);
        }
        else if (strncmp(operations[i], "sub", strlen(operations[i])) == 0)
        {
            result+=subtract(x,y);
        }
        else if (strncmp(operations[i], "mul", strlen(operations[i])) == 0)
        {
            result+=multiply(x,y);
        }
        else if (strncmp(operations[i], "div", strlen(operations[i])) == 0)
        {
            result+=divide(x,y);
        }

    }
    return result;
};

int main(int argc, char * argv [])
{
    float x,y;
    cout<<"Podaj dwie liczby: "<<endl;
    cin>>x>>y;

    cout<<calculate(x,y,argv,argc)<<endl;

    return 0;
}
