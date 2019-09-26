#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>



#define MAX_LINE_SIZE 1024   //the maximum bytes of an inputted command line
#define MAX_ARG_NUM  10    //the maximum number of arguments in a command line

int shell_read_line(char *);
int get_line_args(char *, char **);

int shell_read_line(char * cmd_buf)
{
  int position = 0;
  char c;

  while (1) {
    // Read one character each time
    c = getchar();

    // For newline, put a null character and return.
    if ( c == '\n') {
      cmd_buf[position] = '\0';
      return position;
    } else {
      cmd_buf[position] = c;
      position++;

      //if too big, warning and return -1
      if (position >= MAX_LINE_SIZE) {
        printf("The command size is too big\n");
        return -1;
      }
    }
  }
}


int get_line_args(char * line, char ** args)
{
  int start_position = 0;
  int end_position = 0;
  char c;
  int argc = 0;

  while (argc < MAX_ARG_NUM ){
    //Jump to the first non-space/tab char
    while(1){
      c= line[start_position];
      if ( c == ' ' || c == '\t'){
        start_position ++;
      }else{
        break;
      }
    }

    //Check if the end of string - if yes, return the argument as NULL; otherwise, find the argument
    if ( c == '\0'){
      args[argc] = NULL;
      argc++;
      return argc;
    }else{
      end_position = start_position;

      //Move end_position to the end of the argument
      while (1){
        end_position++;
        c= line[end_position];
        if ( c == ' ' || c == '\t' || c == '\0')
          break;
      }


      if( c != '\0'){
        line[end_position] = '\0';
        end_position++;
      }

      args[argc] = & line[start_position];
      argc ++;

      start_position = end_position;

    }

  }

  //Should never go here; Return -1 for error
  return -1;

}
