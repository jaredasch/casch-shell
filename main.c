#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include "parsing.h"
#include "piping.h"

/*======== void parent_cmds() ==========
	Inputs:  char ** args
    Returns: 1 if a custom command is found and run
             0 if no custom command found
    Inner Workings:
        Checks if args[0] is "exit", exits shell if it is
        Checks if args[0] is "cd", changes directory if it is
        Otherwise returns 0
	====================*/
int parent_cmds(char ** args){
    if(!strcmp(args[0], "exit")){ //exit
        exit(0);
        return 1;
    }
    else if(!strcmp(args[0], "cd")){ //CD
        if (chdir(args[1])){
          printf("-casch: %s: --directry not found--\n",args[1]);
        }
        return 1;
    }
    else if(!strcmp(args[0], "joke")){
        printf("Why didn't the chicken cross the road?\n");
        sleep(2 );
        printf("Because he couldn't redirect the traffic :)\n");
        return 1;
    }
    return 0;
}

/*======== void parse_redir() ==========
	Inputs:  char ** args
    Returns: A modified version of args with any redirection symbols replaced with NULL
    Inner Workings:
        Checks if each arg is a redirection symbol
        If any indicates redirection, then the appropriate file is duped
	====================*/
char** parse_redir(char ** args){
  int i = 0;
  int ray[5]; //contains all indicies of <,> etc in args
  int n = -1; //counter for size of ray

  while(args[i]){   // Iterate through args
      if(!strcmp(args[i],">")){
          n++;
          ray[n] = i;
          int f = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
          dup2(f, STDOUT_FILENO);
      }
      if(!strcmp(args[i],">>")){
          n++;
          ray[n] = i;
          int f = open(args[i+1], O_WRONLY | O_CREAT | O_APPEND, 0777);
          dup2(f, STDOUT_FILENO);
      }
      if(!strcmp(args[i],"<")){
          n++;
          ray[n] = i;
          int f = open(args[i+1], O_RDONLY);
          if (f == -1){
              printf("-casch: %s: --file not found--\n",args[i+1]);
              exit(0);
          }
          dup2(f, STDIN_FILENO);
      }
      if(!strcmp(args[i],"2>")){
          n++;
          ray[n] = i;
          int f = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
          dup2(f, STDERR_FILENO);
      }
      if(!strcmp(args[i],"2>>")){
          n++;
          ray[n] = i;
          int f = open(args[i+1], O_WRONLY | O_CREAT | O_APPEND, 0777);
          dup2(f, STDERR_FILENO);
      }
      i++;
    }
    while(n>-1){ //sets args[x] to NULL for every x in ray, so execvp doesnt execute <,> ect as an argument
      args[ray[n]] = NULL;
      n--;
    }
    return args;
}


/*======== void fork_exec() ==========
	Inputs:  char ** args
	Returns: ---
    Inner Workings:
        First checks if args is not NULL
        Checks if parsing needs to care about pipes
        Checks if the enetered command is one of our custom commands
        Otherwise it forks off another process, parent waits for the child process to exit
	====================*/
void fork_exec(char ** args){
    if (args[0] == NULL){
      return;
    }
    if(is_piped(args)){
      pipe_exec(args);
      return;
    }
    if (parent_cmds(args)){
      return;
    }

    int f = fork();
    if (f){ //PARENT
        int status, childpid;
        childpid = wait(&status);
    }
    else{ //CHILD
        args = parse_redir(args);
        if (execvp(args[0], args) == -1){
          printf("-casch: %s: --command not found--\n",args[0]);
          exit(0);
        }
    }
    return;
}

/*======== void main() ==========
	Inputs: ---
	Returns: 0 if nothing broke
    Inner Workings:
        Prints out current working directory
        Reads stdin into cmd_buffer until a newline is encountered
        Split the cmd_buffer on instances of ";", creating an array of commands cmds
        Executes each command in cmds after splitting it on the whitespace to create an array of arguments
        Repeats
	====================*/
int main(){
    //char ** history = calloc(50, 100); //stores up to 100 commands of 50 chars max
    //int hist_size = 0;
    char * cmd_buffer = calloc(1, 100);
    while(1){
        char * cwd_buffer = calloc(1, 100);
        getcwd(cwd_buffer, 100);
        printf("%s casch$ ", cwd_buffer);

        fgets(cmd_buffer, 100, stdin);
        //history[hist_size] = cmd_buffer;
        //hist_size++;

        char ** cmds = parse_line(cmd_buffer);
        for(int i = 0; cmds[i]; i++){   // Iterate through commands
            char ** args = parse_args(cmds[i]);
            fork_exec(args);
        }
        free(cwd_buffer);
    }
    //free(history);
    free(cmd_buffer);
    return 0;
}
