/**
	simple_shell.c
	purpose: simple linux shell.
	
	@author Milinds Sandaruwan
	@version 1.0 03/08/2017

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


#define ARG_SIZE 10 
#define SPLT " \t\r\n\a"


//function declarations
char* read_line(void);
char** split_args(char *line);
int run_command(char** arguments);
int is_exit(char** arguments);


int main(void){

	char *line;
	char **arguments;
	int execution_ok = 1, exit = 1;

	do{
		printf("> ");
		line = read_line();	
		arguments = split_args(line);
		exit = is_exit(arguments);

		if(exit){
			return 0;
		}

		execution_ok = run_command(arguments);

		free(line);
		free(arguments);

	}while(execution_ok);

}



/**
	@brief scan the user input
	@return pointer of char array which stored user input 
*/
char* read_line(void){

	char *line = NULL;
	size_t size = 0;

	getline(&line, &size, stdin);
	return line;
}



/**
	@brief split the line in to arguments
	@param line pointer of the char array
	@return arguments string array with splited arguments
*/
char** split_args(char *line){

	char **arguments = malloc(ARG_SIZE*sizeof(char*));
	char *current_arg = NULL;
	int index = 0, arg_size = ARG_SIZE;

	//splitting arguments
	current_arg = strtok(line, SPLT);

	while(current_arg != NULL){
		arguments[index] = current_arg;
		current_arg = strtok(NULL, SPLT);

		//reallocate arguments array if it needs more blocks
		if (arg_size == (index+1)){
			arg_size += ARG_SIZE;
			arguments = realloc(arguments, arg_size*sizeof(char*));	
		}

		index++;
	}

	arguments[index] = NULL;

	return arguments;
}



/**
	@brief check the exit command of the shell
	@param arguments splited arguments
	@return status code
*/
int is_exit(char** arguments){
	
	if(arguments[0] == NULL){
		return 1;
	}else if(!strcmp(arguments[0], "exit")){
		return 1;
	}

	return 0;
}


/**
	@brief create a new process and run the user command
	@param arguments splited arguments
	@return status code
*/
int run_command(char** arguments){
	int pid;
	pid = fork();

	if(pid < 0){
		perror("fork");
		return 0;
	}
	if(pid == 0){
		execvp(arguments[0], arguments);
	}
	else{
		wait(NULL);
		return 1;
	}

	return 1;
}
