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
                cerr <<"The command: "<< command <<" value should be bigger or equal to " << limit << endl;
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
    const char * input_rgen_char;
    pid_t idRgen;
    const char *rgen_cmd = "./rgen";
    char *argv_rgen[1];
    argv_rgen[0] = nullptr;
    pid_t a1ece650;
    const char *ece650a1_cmd_1 = "/usr/bin/python";
    const char *ece650a1_cmd_2 = "python";
    const char *ece650a1_cmd_3 = "../a1-ece650.py";
    pid_t a2ece650;
    const char *ece650a2_cmd = "./a2-ece650";
    char *argv_ece650a2[1];
    argv_ece650a2[0] = nullptr;
    //Create Pipe_a3 to rgen
    int pipe_ece650a3_rgen[2];
    pipe(pipe_ece650a3_rgen);

    //Create Pipe_rgen to a1
    int pipe_rgen_ece650a1[2];
    pipe(pipe_rgen_ece650a1);

    //Create Pipe_a1 to a2
    int pipe_ece650a1_ece650a2[2];
    pipe(pipe_ece650a1_ece650a2);
    int a1status;
    pid_t a1statusresult;
    //int a1outputline1result;
    //char a1outputline1[4096];
    //int a1outputline2result;
    //char a1outputline2[4096];

    const char * user_input_a2_char;

    string user_input;


    vector<char *> arg_list;
    for (int i = 0; i < argc; ++i) {
        arg_list.push_back(argv[i]);
    }
    //update the command line variables
    s = Read_Command_Line(argc, arg_list, 's', 2, 3);
    n = Read_Command_Line(argc, arg_list, 'n', 1, 3);
    l = Read_Command_Line(argc, arg_list, 'l', 1, 4);
    c = Read_Command_Line(argc, arg_list, 'c', 1, 15);

    //Create ouput for rgen
    input_rgen = input_rgen + "s" + to_string(s) + ",";
    input_rgen = input_rgen + "n" + to_string(n) + ",";
    input_rgen = input_rgen + "l" + to_string(l) + ",";
    input_rgen = input_rgen + "c" + to_string(c) + ":";
    input_rgen_char = input_rgen.c_str();

    //Start child process



    idRgen = fork();
    if (idRgen != 0) {
        FILE *stream_input_rgen;
        stream_input_rgen = fdopen(pipe_ece650a3_rgen[1], "w");
        fprintf(stream_input_rgen, "%s", input_rgen_char);
        fflush(stream_input_rgen);
        close(pipe_ece650a3_rgen[0]);
        close(pipe_ece650a3_rgen[1]);
    }else{
        dup2(pipe_ece650a3_rgen[0], STDIN_FILENO);
        close(pipe_ece650a3_rgen[1]);
        close(pipe_ece650a3_rgen[0]);
        dup2(pipe_rgen_ece650a1[1], STDOUT_FILENO);
        close(pipe_rgen_ece650a1[1]);
        close(pipe_rgen_ece650a1[0]);
        execv(rgen_cmd, argv_rgen);
    }
    children.push_back(idRgen);

    a1ece650 = fork();
    if (a1ece650 == 0) {
        dup2(pipe_rgen_ece650a1[0], STDIN_FILENO);
        close(pipe_rgen_ece650a1[1]);
        close(pipe_rgen_ece650a1[0]);
        dup2(pipe_ece650a1_ece650a2[1], STDOUT_FILENO);
        close(pipe_ece650a1_ece650a2[1]);
        close(pipe_ece650a1_ece650a2[0]);
        execl(ece650a1_cmd_1,ece650a1_cmd_2, ece650a1_cmd_3,NULL);
    }
    children.push_back(a1ece650);
    a1statusresult = waitpid(a1ece650,&a1status,WNOHANG);

    while(a1statusresult == 0){
        a2ece650 = fork();
        if (a2ece650==0) {
            //read(pipe_ece650a1_ece650a2[0],a1outputline1,sizeof(a1outputline1));
            //cout << a1outputline1 << endl;
            dup2(pipe_ece650a1_ece650a2[0], STDIN_FILENO);
            close(pipe_ece650a1_ece650a2[1]);
            close(pipe_ece650a1_ece650a2[0]);
            execv(ece650a2_cmd, argv_ece650a2);
        }
        //else{
            //getline(cin, user_input);
            if (poll(pipe_ece650a1_ece650a2, ))
            //user_input_a2_char = user_input.c_str();
            //FILE *stream_input_a1;
            //stream_input_a1 = fdopen(pipe_ece650a1_ece650a2[1], "w");
            //fprintf(stream_input_a1, "%s", user_input_a2_char);
            //fflush(stream_input_a1);
            //close(pipe_ece650a1_ece650a2[0]);
            //close(pipe_ece650a1_ece650a2[1]);
        //}
    }
    children.push_back(a2ece650);

    return 0;
}
