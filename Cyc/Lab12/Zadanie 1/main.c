#include "statemachine.h"
#include "reader.c"
#include "state.c"

int main()
{
    exec();
    return 0;
}

void exec()
{
    char stan = 'e';
    char przejscie = 'e';
    while (przejscie != 'q')
    {
        przejscie = readChar();
        switch(stan)
        {
        case 'e':
            switch(przejscie)
            {
            case 'w':
                stan='w';
                write(przejscie);
                break;
            case 'q':
                return;
                break;
            default:
                echo(przejscie);
                break;
            }
            break;
        case 'w':
            switch(przejscie)
            {
            case 'e':
                stan='e';
                echo(przejscie);
                break;
            case 'q':
                return;
                break;
            default:
                write(przejscie);
                break;

            }
            break;
        }
    }
}



