#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <boost/tokenizer.hpp>

using namespace std;

int main()
{
    char hostname[32];
    gethostname(hostname, sizeof hostname);

    //Gets user input until "exit" is typed
    while(1)
    {
        cout << getlogin() << hostname  << "$ ";
        string userinput;
        getline(cin, userinput);
        

        //Next 3 lines creates a Tokenizer which parses for characters.
        typedef boost::tokenizer<boost::char_separator<char> > Tok;
        boost::char_separator<char> sep;
        Tok tok(userinput, sep);  //Container of tokens

        for (Tok::iterator tok_iter = tok.begin(); tok_iter != tok.end();
            ++tok_iter)
        {
            if (*tok_iter == "exit")
            {
                exit(0);
            }
        }
    }
    return 0;
}
