#include <iostream>
#include "connectors.h"

using namespace std;

int main()
{
    char hostname[32];
    gethostname(hostname, sizeof hostname);

    //Gets user input until "exit" is typed
    while(1)
    {
        cout << getlogin() << "@" << hostname  << "$ ";
        string userinput;
        getline(cin, userinput);
        
        separator(userinput);
    }
    return 0;
}

