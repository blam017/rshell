#ifndef EXECUTION_H
#define EXECUTION_H

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

//  This function runs based on the && connector.  It takes in a vector of 
//commands to run along with 2 bools.  ran_first is the important one as it
//detects whether or not the first command ran or not.
void run_and (vector<string> &commands, bool &is_first, bool ran_first)
{
    if (ran_first)
    {
        int count = commands.size() + 1;
        char **args = new char*[count];
        for (int i = 0; i < count - 1; ++i)
        {
            const char *mystr = commands.at(i).c_str();
            args[i] = const_cast<char *> (&mystr[0]);
        }
        args[count - 1] = 0;
        
        int status;
        pid_t c_pid, pid; // Where c_pid is child
        c_pid = fork();
        if (c_pid < 0)
        {
            perror("fork failed");
            exit(1);
        }
        else if (c_pid == 0)
        {
            execvp(args[0], args);
            perror("-bash");
            exit(errno);
        }
        else if (c_pid > 0)
        {
            if ( (pid = wait(&status)) < 0)
            {;
                perror("wait");
                exit(1);
            }
            delete [] args;
        }
    }
    is_first = false;
    return;
}

//  Executes based on || connector.
void run_or (vector<string> &commands, bool &is_first, bool ran_first)
{
    if (!ran_first)
    {
        int count = commands.size() + 1;
        char **args = new char*[count];
        for (int i = 0; i < count - 1; ++i)
        {
            const char *mystr = commands.at(i).c_str();
            args[i] = const_cast<char *> (&mystr[0]);
        }
        args[count - 1] = 0;
        
        int status;
        pid_t c_pid, pid; // Where c_pid is child
        c_pid = fork();
        if (c_pid < 0)
        {
            perror("fork failed");
            exit(1);
        }
        else if (c_pid == 0)
        {
            execvp(args[0], args);
            perror("-bash");
            exit(errno);
        }
        else if (c_pid > 0)
        {
            if ( (pid = wait(&status)) < 0)
            {
                perror("wait");
                exit(1);
            }
            delete [] args;
        }
    }
    is_first = false;
    return;
}

//  First command or no connectors.
void run_first(vector <string> &commands, bool &is_first, bool &ran_first)
{
    int count = commands.size() + 1;
    char **args = new char*[count];
    for (int i = 0; i < count - 1; ++i)
    {
        const char *mystr = commands.at(i).c_str();
        args[i] = const_cast<char *> (&mystr[0]);
    }
    args[count - 1] = 0;
    
    int status;
    pid_t c_pid, pid; // Where c_pid is child
    c_pid = fork();
    if (c_pid < 0)
    {
        perror("fork failed");
        exit(1);
    }
    else if (c_pid == 0) // Child process
    {
        execvp(args[0], args);
        perror("-bash");
        exit(errno);
    }
    else if (c_pid > 0)
    {
        if ( (pid = wait(&status)) < 0)
        {
            perror("wait");
            exit(1);
        }
        if (WEXITSTATUS(status) != 0)
        {
            ran_first = false; // True if execvp executes, else false.
        }
        delete [] args;
    }
    is_first = false;
}

bool execution (string userinput)
{
    queue< vector<string> > command_queue; // Queue of commands
    vector<string> commands; // Holds single commands to run.

    // Tokenizer which searches for connectors
    typedef boost::tokenizer<boost::char_separator<char> > Tok;
        boost::char_separator<char> sep(" ", ";#|&", boost::drop_empty_tokens);
        Tok tok(userinput, sep);  //Container of tokens

    for (Tok::iterator tok_iter = tok.begin(); tok_iter != tok.end();
        ++tok_iter)
    {   
        if (*tok_iter == "#")
        {
            break; // If # ignore rest.
        }
        else if (*tok_iter == ";") // Check for ";" connector.
        {

            Tok::iterator check = tok_iter;
            ++check;
            if (*check == "&" || *check == "|")
            {
                cout << "-bash: Syntax error: No command before connector.\n";
                return true;
            }

            command_queue.push(commands);
            commands.clear();
        }
        else if (*tok_iter == "&") // Check for "&&" connector.
        {
            ++tok_iter;
            if (tok_iter == tok.end() || *tok_iter != "&")
            {
                cout << "-bash: Syntax error: Unexpected token near &\n";
                return true;
            }

            Tok::iterator check = tok_iter;
            ++check;
            if (check == tok.end())
            {
                cout << "Error: No command after connector.\n";
                return true;
            }

            else if (*check == "|" || *check == "&" || *check == ";")
            {
                cout << "-bash: Syntax error: Unexpected token near &\n";
                return true;
            }
            commands.push_back("&&");
        }
        else if (*tok_iter == "|") // Checks for "||" connector.
        {
            ++tok_iter;
            if (*tok_iter != "|")
            {
                cout << "-bash: Syntax error: Unexpected token near |\n";
                return true;
            }

            Tok::iterator check = tok_iter;
            ++check;
            if (check == tok.end())
            {
                cout << "Error: No command after connector.";
                return true;
            }
            else if (*check == "|" || *check == "&" || *check == ";")
            {
                cout << "-bash: Syntax error: Unexpected token near |\n";
                return true;
            }
            commands.push_back("||");
        }
        else
        {
            commands.push_back(*tok_iter); // Pushes input into vector
        }
    }
    if (!commands.empty())
    {
        command_queue.push(commands);
    }

    while (!command_queue.empty()) // Execution loop after parsing commands.
    {
        bool first_command_ran = true;
        bool is_first_command = true;
        if (command_queue.front().at(0) == "exit")
        {
            return false;
        }
        if (command_queue.front().at(0) == "&&" || 
            command_queue.front().at(0) == "||")
        {
            cout << "-bash: Syntax error: Connector at front of argument.\n";
            return true;
        }

        vector<string> commands_to_run;
        queue< vector<string> > run_queue;
        for (unsigned i = 0; i < command_queue.front().size(); ++i)
        {
            if (command_queue.front().at(i) != "&&" && 
               command_queue.front().at(i) != "||")
            {
                commands_to_run.push_back(command_queue.front().at(i));
            }
            else
            {
                run_queue.push(commands_to_run);
                commands_to_run.clear();
                commands_to_run.push_back(command_queue.front().at(i));
                run_queue.push(commands_to_run);
                commands_to_run.clear();
            }
        }
        if (!commands_to_run.empty())
        {
            run_queue.push(commands_to_run);
        }

        if (run_queue.back().at(0) == "&&" || run_queue.back().at(0) == "||")
        {
            cout << "-bash: Syntax error: Connector at end of argument.\n";
            return true;
        }

        while (!run_queue.empty())
        {
            if (run_queue.front().at(0) == "&&")
            {
                run_queue.pop();
                if (run_queue.front().at(0) == "exit" && first_command_ran)
                {
                    return false;
                }
                run_and (run_queue.front(), is_first_command, first_command_ran);
                run_queue.pop();
            }
            else if (run_queue.front().at(0) == "||")
            {
                run_queue.pop();
                if (run_queue.front().at(0) == "exit" && !first_command_ran)
                {
                    return false;
                }
                run_or (run_queue.front(), is_first_command, first_command_ran);
                run_queue.pop();
            }
            else
            {
                run_first(run_queue.front(), is_first_command, first_command_ran);
                run_queue.pop();
            }
        }
        
        command_queue.front().clear();
        command_queue.pop();
    }
    return true;
}

#endif
