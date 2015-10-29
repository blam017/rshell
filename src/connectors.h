#ifndef connectors_h
#define connectors_h

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <boost/tokenizer.hpp>
#include <vector>
#include <queue>

using namespace std;

void separator(string userinput)
{
    queue< vector<string> > command_queue;    
    vector<string> commands;

    typedef boost::tokenizer<boost::char_separator<char> > Tok;
        boost::char_separator<char> sep;
        Tok tok(userinput, sep);  //Container of tokens


    int count = 0; //Number of tokens
    for (Tok::iterator tok_iter = tok.begin(); tok_iter != tok.end();
        ++tok_iter)
    {   
        if (*tok_iter == ";")
        {
            command_queue.push(commands);
            commands.clear();
            ++tok_iter;
            if (tok_iter == tok.end())
            {
                break;
            }
        }
        commands.push_back(*tok_iter);
    }

    while(!command_queue.empty())
    {
        int count = command_queue.front().size();
        char *args[count + 1];
        for (int i = 0; i < count; ++i)
        {
            string s = command_queue.front().at(i);
            char *cstring = new char[s.size() + 1];
            cstring[s.size()] = 0;
            memcpy(cstring, s.c_str(), s.size());
            args[i] = cstring;
        }
        args[count] = NULL;
        
        int status;
        pid_t c_pid, pid; // Where c_pid is child
        c_pid = fork();
        if (c_pid == 0)
        {
            execvp(args[0], args);
            perror("Error: Invalid command");
        }
        else if (c_pid > 0)
        {
            if( (pid = wait(&status)) < 0)
            {
                perror("wait");
                exit(1);
            }
        }
        command_queue.pop();
    }
}

#endif
