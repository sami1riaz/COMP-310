#include <stdio.h>      // This is standard input/output header file in which Input/Output functions are declared
#include <unistd.h>     // Provides access to the POSIX operating system API.
#include <string.h>     // All string related functions are defined in this header fil
#include <stdlib.h>     // Contains general functions used in C programs
#include <math.h>
#include <dirent.h>
//
// This code is given for illustration purposes. You need not include or follow this
// strictly. Feel free to write better or bug free code. This example code block does not
// worry about deallocating memory. You need to ensure memory is allocated and deallocated
// properly so that your shell works without leaking memory.
//

// The getcmd() function reads in the user’s next command, and then parses it into separate
// tokens that are used to fill the argument vector for the command to be executed. 



int commandNumber = 0;
struct node *head_job = NULL;
struct node *current_job = NULL;



struct node {
	int number; // the job number
	int pid; // the process id of the a specific process
	struct node *next; // when another process is called you add to the end of the linked list
};

/* Add a job to the list of jobs
 */
void addToJobList(pid_t process_pid) {

	struct node *job = malloc(sizeof(struct node));

	//If the job list is empty, create a new head
	if (head_job == NULL) {
		job->number = 1;
		job->pid = process_pid;

		//the new head is also the current node
		job->next = NULL;
		head_job = job;
		current_job = head_job;
	}

	//Otherwise create a new job node and link the current node to it
	else {

		job->number = current_job->number + 1;
		job->pid = process_pid;

		current_job->next = job;
		current_job = job;
		job->next = NULL;
	}
}

int getcmd(char *prompt, char *args[], int *background){
    int length, i = 0;
    char *token, *loc;
    char *line = NULL;
    size_t linecap = 0; //size_t is an unsigned data type. This type is used to represent the size of an object. Library functions that take or return sizes expect them to be of type or have the return type of size_t.
    printf("%s", prompt);
    length = getline(&line, &linecap, stdin); //return the number of characters written, excluding the terminating NUL character. The value -1 is returned if an error occurs, or if end-of-file is reached.
    if (length <= 0) {
        exit(-1);   
    }
// Check if background is specified..
    if ((loc = index(line, '&')) != NULL) {
        *background = 1;
        *loc = ' ';
    } 
    else
        *background = 0;
    while ((token = strsep(&line, " \t\n")) != NULL) {
        for (int j = 0; j < strlen(token); j++)
            if (token[j] <= 32)
                token[j] = '\0';
            if (strlen(token) > 0)
                args[i++] = token;
    }
    return i;
}



int main(void) {
    
	
	int i,j;
	
	char *args[20];
 	int bg;
	char username[50];
    
    char *path;     // for cd
    
    while(1) {
        bg = 0;
        int cnt = getcmd("\n>> ", args, &bg);


        // cd COMMAND
        if (!strcmp("cd", args[0])){
            int result = 0;
			if (args[1] == NULL) { // this will fetch home directory if you just input "cd" with no arguments
				char *home = getenv("HOME");
				if (home != NULL) {
					result = chdir(home);
				}
				else {
					printf("cd: No $HOME variable declared in the environment");
				}
			}
			//Otherwise go to specified directory
			else {
				result = chdir(args[1]);
			}
			if (result == -1) fprintf(stderr, "cd: %s: No such file or directory", args[1]);

		}

        // "PWD" COMMAND
		if (!strcmp("pwd", args[0])){
			char cwd[100];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s", cwd);
            }
            
        }
        
		// "EXIT" COMMAND
		if (!strcmp("exit", args[0])){

            exit(-1);   // just exit
        }

        // "CAT" COMMAND
        if (!strcmp("cat", args[0])){
            FILE *fp;
            char file_name[256], ch;
            for(i = 1; i <= sizeof(args); i++){
                strncpy(file_name, args[i], 256);
                fp = fopen(file_name, "r");
                if(fp == NULL){         // thought this would do the trick
                    printf("No such file or directory\n"); // prints no matter what
                    return 0;
                    
                }
                while((ch = fgetc(fp)) != EOF){
                    putchar(ch);
                }
            fclose(fp);
            }
        return 0;
        }

        if (!strcmp("ls", args[0])) { 
            char dir[100];
            DIR*p;
            struct dirent *d;

            if(args[1] != NULL){
                p = opendir(dir);
                if(p == NULL){
                    perror("Cannot find directory");
                }
                while(d == readdir(p)){
                    printf("%s\n", d -> d_name);
                }
              
        
            }
        }
        
    }
}