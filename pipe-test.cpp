//from http://burweisnote.blogspot.com/2017/10/pipe.html?m=1

//pipe_test.cpp
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <signal.h>
#include <vector>
#include <algorithm>
#include <sys/wait.h>

#define MAX_WORDS_IN_LINE 1000
#define BUFF_SIZE 100000

int main(){

        int p1[2];
        int p2[2];
        int num = 0;
        int this_vbar = 0;
        int last_vbar = 0;
        int stdin_copy = dup(STDIN_FILENO);
        int stdout_copy = dup(STDOUT_FILENO);
        char pipe_buff[BUFF_SIZE] = {0};
        std::string str_in = "";
        std::string str_temp = "";
        std::vector<std::string> cmd;
        std::vector<int> vbar_pos;
        std::stringstream ss;

        /*get input from std::cin, and store it*/
        std::getline(std::cin,str_in,'\n');
        ss.str(str_in);
        vbar_pos.push_back(-1);
        for(int i=0; ss>>str_temp; i++){
                cmd.push_back(str_temp);          /*store splited strings in vector cmd*/
                if(cmd.at(i) == "|"){
                        vbar_pos.push_back(i);   /*store the pos of "|" in vector vbar_pos*/
                }
        }

        /*start doing the pipe task*/
        for(int i=0; i<cmd.size(); i++){
                if(cmd.at(i) == "|" || i==cmd.size()-1){
                        if(cmd.at(i) == "|"){
                                this_vbar = std::find(vbar_pos.begin(), vbar_pos.end(), i) - vbar_pos.begin();
                                last_vbar = this_vbar-1;  /*this_vbar, last_vbar are indexes in vbar_pos*/
                        }
                        if(pipe(p1)<0)
                                std::cout << "create pipe1 error" << std::endl;
                        if(pipe(p2)<0)
                                std::cout << "create pipe2 error" << std::endl;
                        signal(SIGCHLD,SIG_IGN);
                        if(fork()==0){ /*child process: call execvp if cmd.at(i) is "|" or is the last word*/
                                if(i==cmd.size()-1){
                                  //cmd is some string array
                                        close(p1[1]);
                                        dup2(p1[0],STDIN_FILENO); /*p1 will close after STDIN receive EOF*/
                                        close(p1[0]);
                                        close(p2[0]);
                                        close(p2[1]);
                                        dup2(stdout_copy,STDOUT_FILENO);
                                        char* arg[MAX_WORDS_IN_LINE] = {0};
                                        for(int j=0 ; j<(i-vbar_pos.at(this_vbar)); j++){
                                                arg[j] = strdup(cmd.at(vbar_pos.at(this_vbar)+j+1).c_str());
                                        }
                                        arg[i-vbar_pos.at(this_vbar)] = NULL;
                                        execvp(arg[0],arg);
                                        exit(0);
                                }else{
                                        close(p1[1]);
                                        dup2(p1[0],STDIN_FILENO); /*p1 will close after STDIN receive EOF*/
                                        close(p1[0]);
                                        close(p2[0]);
                                        dup2(p2[1],STDOUT_FILENO);
                                        close(p2[1]);
                                        char* arg[MAX_WORDS_IN_LINE] = {0};
                                        for(int j=0 ; j<(i-vbar_pos.at(last_vbar)-1); j++){
                                                arg[j] = strdup(cmd.at(vbar_pos.at(last_vbar)+j+1).c_str());
                                        }
                                        arg[i-vbar_pos.at(last_vbar)-1] = NULL;
                                        execvp(arg[0],arg);
                                        exit(0);
                                }
                        }else{ /*parent process: pass old data to the new child and store new data*/
                                close(p1[0]);
                                dup2(p1[1],STDOUT_FILENO);
                                close(p1[1]);
                                std::cout << pipe_buff << std::flush;
				//close(STDOUT_FILENO);
				//dup(stdout_copy);
                                dup2(stdout_copy,STDOUT_FILENO); /*p1 write end isn't used anymore, send EOF*/
                                close(p2[1]);
                                if(i<cmd.size()-1){
                                        memset(pipe_buff, 0, sizeof(pipe_buff));
                                        read(p2[0],pipe_buff, sizeof(pipe_buff));
                                }
                                close(p2[0]);  /*p2 close here*/
                        }
                }
        }
}
