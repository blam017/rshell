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

void run_and (vector<string> &commands, bool &is_first, bool ran_first)
{
    if (ran_first)
    {
        int count = commands.size();
        char *args[count + 1];
        for (int i = 0; i < count; ++i)
        {
            const char *mystr = commands.at(i).c_str();
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
        else if (c_pid == 0)
        {
            execvp(args[0], args);
            int err = errno;
            perror("-bash");
            exit(errno);
        }
        else if (c_pid > 0)
        {
            if ( (pid = wait(&status)) < 0)
            {
                cout << "check" << endl;
                perror("wait");
                exit(1);
            }
        }
    }
    is_first = false;
    return;
}

void run_or (vector<string> &commands, bool &is_first, bool ran_first)
{
    if (!ran_first)
    {
        int count = commands.size();
        char *args[count + 1];
        for (int i = 0; i < count; ++i)
        {
            const char *mystr = commands.at(i).c_str();
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
        else if (c_pid == 0)
        {
            execvp(args[0], args);
            int err = errno;
            perror("-bash");
            exit(errno);
        }
        else if (c_pid > 0)
        {
            if ( (pid = wait(&status)) < 0)
            {
                cout << "check" << endl;
                perror("wait");
                exit(1);
            }
        }
    }
    is_first = false;
    return;
}

void run_first(vector <string> &commands, bool &is_first, bool &ran_first)
{
    int count = commands.size();
    char *args[count + 1];
    for (int i = 0; i < count; ++i)
    {
        const char *mystr = commands.at(i).c_str();
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
    else if (c_pid == 0)
    {
        execvp(args[0], args);
        int err = errno;
        perror("-bash");
        exit(errno);
    }
    else if (c_pid > 0)
    {
        if ( (pid = wait(&status)) < 0)
        {
            cout << "check" << endl;
            perror("wait");
            exit(1);
        }
        if (WEXITSTATUS(status) != 0)
        {
            ran_first = false;
        }
    }
    is_first = false;
}

bool execution (string userinput)
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
        }
        else if (*tok_iter == "&")
        {
            ++tok_iter;
            if (tok_iter == tok.end() || *tok_iter != "&")
            {
                cout << "-bash: Syntax error: Unexpected token near &\n";
                return true;
            }
            /*
            if(tok_iter == tok.end())
            {
                cout << "-bash: Syntax error:";
            }*/

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
    while (!command_queue.empty())
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
            cout << "-bash: Syntax error: Connector at front of argument";
            return true;
        }

        vector<string> commands_to_run;
        queue< vector<string> > run_queue;
        for (int i = 0; i < command_queue.front().size(); ++i)
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
        run_queue.push(commands_to_run);
        /*
        while (!run_queue.empty())
        {
            for(int i = 0; i < run_queue.front().size(); ++i)
            {
                cout << run_queue.front().at(i);
            }
            run_queue.pop();
        }*/

        if (run_queue.back().at(0) == "&&" || run_queue.back().at(0) == "||")
        {
            cout << "-bash: Syntax error: Connector at end of argument.";
            return true;
        }


        while (!run_queue.empty())
        {
            if (run_queue.front().at(0) == "&&")
            {
                run_queue.pop();
                run_and (run_queue.front(), is_first_command, first_command_ran);
                run_queue.pop();
            }
            else if (run_queue.front().at(0) == "||")
            {
                run_queue.pop();
                run_or (run_queue.front(), is_first_command, first_command_ran);
                run_queue.pop();
            }
            else
            {
                run_first(run_queue.front(), is_first_command, first_command_ran);
                run_queue.pop();
            }
        }
        /*
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
        else if (c_pid == 0)
        {
            cout << args[0] << endl;
            execvp(args[0], args);
            int err = errno;
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
            if(WIFEXITED(status))
            {
                if(WEXITSTATUS(status) == 0)
                {
                cout << "SUCCESS" << endl;
                }
                else
                    cout << "failure" << endl;
            }
            else if (!WIFEXITED(status))
            {
                cout << "FAILED" << endl;
            }
        }*/
        command_queue.front().clear();
        command_queue.pop();
    }
    return true;
}

#endif
