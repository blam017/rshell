#ifndef execution_h
#define execution_h

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <boost/tokenizer.hpp>
#include <vector>
#include <queue>

using namespace std;

bool separator(string userinput)
{
    queue< vector<string> > command_queue;    
    vector<string> commands;

    typedef boost::tokenizer<boost::char_separator<char> > Tok;
        boost::char_separator<char> sep(" ", ";#|&", boost::drop_empty_tokens);
        Tok tok(userinput, sep);  //Container of tokens


    int count = 0; //Number of tokens
    for (Tok::iterator tok_iter = tok.begin(); tok_iter != tok.end();
        ++tok_iter)
    {   
        if (*tok_iter == "#")
        {
            break;
        }
        else if (*tok_iter == ";")
        {
            Tok::iterator check = tok_iter;
            ++check;
            if (*check == "&" || *check == "|")
            {
                cout << "-bash: Syntax error: No command before connector.";
                return true;
            }

            command_queue.push(commands);
            commands.clear();
            ++tok_iter;
        }
        else if (*tok_iter == "&")
        {
            ++tok_iter;
            if (*tok_iter != "&")
            {
                cout << "-bash: Syntax error: Unexpected token near &\n";
                return true;
            }

            Tok::iterator check = tok_iter;
            ++check;
            if (*check == "|" || *check == "&" || *check == ";")
            {
                cout << "-bash: Syntax error: Unexpected token near &\n";
                return true;
            }
            else if (check == tok.end())
            {
                cout << "Error: No command after connector.";
                return true;
            }
            commands.push_back("&&");
        }
        else if (*tok_iter == "|")
        {
            ++tok_iter;
            if (*tok_iter != "|")
            {
                cout << "-bash: Syntax error: Unexpected token near |\n";
                return true;
            }

            Tok::iterator check = tok_iter;
            ++check;
            if (*check == "|" || *check == "&" || *check == ";")
            {
                cout << "-bash: Syntax error: Unexpected token near |\n";
                return true;
            }
            else if (check == tok.end())
            {
                cout << "Error: No command after connector.";
                return true;
            }
            commands.push_back("||");
        }
        else
        {
            commands.push_back(*tok_iter);
        }
    }
    command_queue.push(commands);
/*    
    for (int i = 0; i < command_queue.front().size(); ++i)
    {
        cout << command_queue.back().at(i) << endl;
    }

    cout << endl;

    while(!command_queue.empty())
    {
        for (int i = 0; i < command_queue.front().size(); ++i)
        {
            cout << command_queue.front().at(i) << endl;
        }
        command_queue.pop();
    }
*/
    while(!command_queue.empty())
    {
        if(command_queue.front().at(0) == "exit")
        {
            return false;
        }
        int count = command_queue.front().size();
        char *args[count + 1];
        for (int i = 0; i < count; ++i)
        {
            const char *mystr = command_queue.front().at(i).c_str();
            args[i] = const_cast<char *> (&mystr[0]);
        }
        args[count] = 0;
        
        int status;
        pid_t c_pid, pid; // Where c_pid is child
        c_pid = fork();
        if (c_pid < 0)
        {
            perror("fork failed");
            exit(1);
        }
        else if(c_pid == 0)
        {
            execvp(args[0], args);
            int err = errno;
            perror("-bash");
            exit(errno);
        }
        else if (c_pid > 0)
        {
            if( (pid = wait(&status)) < 0)
            {
                cout << "check" << endl;
                perror("wait");
                exit(1);
            }
            if(wait(&status))
            {
                cout << "done" << endl;
            }
        }
        command_queue.front().clear();
        command_queue.pop();
    }
    return true;
}
#endif
