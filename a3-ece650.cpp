#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdio>
#include <signal.h>
#include <string.h>
#include <vector>
#include <string>
#include <poll.h>


using namespace std;

int Read_Command_Line(int argc, vector<char*> arg, char command, int limit, int inital){
    int i = 0;
    while(i < argc){
        if (arg[i][1] == command){
            int tmp_int = atoi(arg[i+1]);
            if (tmp_int >= limit){
                return tmp_int;
            }
            else {
                cerr <<"Error: The command: "<< command <<" value should be bigger or equal to " << limit << endl;
                break;
            }
        }
        i++;
    }
    return inital;
}
vector<string> String_Split(string String_Input, string Deliminator){           //A Function split a string and return a vector
    int String_Index = 0;                                                       //Initialized the String Index
    vector<string> Splited_String;                                              //Initialized the Vector
    string Sub_String;                                                          //Initialized the substring
    string String_For_Split = String_Input;                                     //Record the string for split
    while(String_Index != -1){                                                  //Stop the loop if deliminator is not found
        String_Index = String_For_Split.find(Deliminator);
        Sub_String = String_For_Split.substr(0,String_Index);                   //Find the substring
        Splited_String.push_back(Sub_String);                                   //Added the substring to vector
        String_For_Split.erase(0, String_Index+1);                              //Re-size the string
    }
    return Splited_String;
}

int main (int argc, char* argv[]) {
    //Initialize the command line varibales
    int s;   //10 default
    int n;   //5 default
    int l;   //5 default
    int c;   //20 default
    string input_rgen;
    vector<pid_t> children;
    //const char * input_rgen_char;
    pid_t idRgen;

    pid_t a1ece650;
    pid_t a2ece650;
    //Create Pipe_a3 to rgen
    int pipe_ece650a3_rgen[2];
    pipe(pipe_ece650a3_rgen);
    //Create Pipe_rgen to a1
    int pipe_rgen_ece650a1[2];
    pipe(pipe_rgen_ece650a1);

    //Create Pipe_a1 to a2
    int pipe_ece650a1_ece650a2[2];
    pipe(pipe_ece650a1_ece650a2);

    pid_t input_process;

    string user_input;

    vector<char *> arg_list;
    for (int i = 0; i < argc; ++i) {
        arg_list.push_back(argv[i]);
    }
    //update the command line variables
    s = Read_Command_Line(argc, arg_list, 's', 2, 10);
    n = Read_Command_Line(argc, arg_list, 'n', 1, 5);
    l = Read_Command_Line(argc, arg_list, 'l', 5, 5);
    c = Read_Command_Line(argc, arg_list, 'c', 1, 20);


    //Start child process

    idRgen = fork();
    children.push_back(idRgen);
    if (idRgen == 0) {
        dup2(pipe_rgen_ece650a1[1], STDOUT_FILENO);
        close(pipe_rgen_ece650a1[1]);
        close(pipe_rgen_ece650a1[0]);
        execl("./rgen","rgen", "-s", to_string(s).c_str(), "-n", to_string(n).c_str(),"-l", to_string(l).c_str(), "-c",to_string(c).c_str(), (char*) NULL);
    } else {
        a1ece650 = fork();
        children.push_back(a1ece650);
        if (a1ece650 == 0) {
            dup2(pipe_rgen_ece650a1[0], STDIN_FILENO);
            close(pipe_rgen_ece650a1[1]);
            close(pipe_rgen_ece650a1[0]);
            dup2(pipe_ece650a1_ece650a2[1], STDOUT_FILENO);
            //dup2(pipe_ece650a1_ece650a2[1], STDERR_FILENO);
            close(pipe_ece650a1_ece650a2[1]);
            close(pipe_ece650a1_ece650a2[0]);
            execl("/usr/bin/python", "python", "a1-ece650.py", NULL);
        } else {
            //children.push_back(a1ece650);
            a2ece650 = fork();
            children.push_back(a1ece650);
            if (a2ece650 == 0) {
                dup2(pipe_ece650a1_ece650a2[0], STDIN_FILENO);
                close(pipe_ece650a1_ece650a2[1]);
                close(pipe_ece650a1_ece650a2[0]);
                execl("./a2-ece650", "a2-ece650", NULL);
            } else {
                input_process = fork();
                children.push_back(input_process);
                if (input_process == 0) {
                    dup2(pipe_ece650a1_ece650a2[1], STDOUT_FILENO);
                    close(pipe_ece650a1_ece650a2[0]);
                    close(pipe_ece650a1_ece650a2[1]);
                    while (true) {
                        getline(cin, user_input);
                        cout << user_input << endl;
                    }
                } else {
                    int status;
                    while (1) {
                        for (auto it = children.begin(); it != children.end(); ++it) {
                            waitpid(idRgen, &status, WNOHANG);
                            if (status == 0) {
                                //cout << status << endl;
                                if ((*it) == idRgen) {
                                    kill(a1ece650, SIGTERM);
                                    kill(a2ece650, SIGTERM);
                                    kill(input_process, SIGTERM);
                                    return 0;
                                } else {
                                    kill(a1ece650, SIGTERM);
                                    kill(a2ece650, SIGTERM);
                                    kill(idRgen, SIGTERM);
                                    return 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }



    //children.push_back(a2ece650);


    //waitpid(idRgen,&rgenstatus,WNOHANG);

    //if (rgenstatusresult != 0){
    //for(pid_t p: children){
    //kill(p,SIGTERM);
    // }
    //}

    //return 0;
}
