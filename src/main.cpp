#include <iostream>
#include "execution.h"

using namespace std;

int main()
{
    char hostname[32];
    gethostname(hostname, sizeof hostname);
    bool notExit = true; 
    //Gets user input until "exit" is typed
    while(notExit)
    {
        cout << getlogin() << "@" << hostname  << "$ ";
        string userinput;
        getline(cin, userinput);
        
        notExit = separator(userinput);
    }
    return 0;
}

