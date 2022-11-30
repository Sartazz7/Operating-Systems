#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
#define MAX_PATH_SIZE 1024
#define MAX_PROCESSES 1024

// TODO: YOUR CODE HERE
// INITIALIZE DATA STRUCTURE TO STORE PIDS OF PROCESSES TO KILL LATER

/* Splits the string by space and returns the array of tokens
*
*/
char **tokenize(char *line) {

    // tokenizer variables
	char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
	char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
	int i, tokenIndex = 0, tokenNo = 0;

    // loop on length of line
	for(i=0; i < strlen(line); i++){

		char readChar = line[i];

        // tokenize on any kind of space
		if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
			token[tokenIndex] = '\0';
			if (tokenIndex != 0) {
				tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
				strcpy(tokens[tokenNo++], token);
				tokenIndex = 0; 
			}
		}
		else {
			token[tokenIndex++] = readChar;
		}
	}
	
	free(token);
	tokens[tokenNo] = NULL ;
	return tokens;
}

// TODO
// MAKE FUNCTIONS AS REQUIRED TO AVOID REPETITIVE WORK

char ***distribute(char **tokens, char *bridge){
	char ***tokensArr = (char ***)malloc(MAX_NUM_TOKENS * sizeof(char **));
	int currTokens=0, tokensIdx=0, tokensArrIdx=0;
    tokensArr[currTokens] = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
	for(tokensIdx=0; tokens[tokensIdx]!=NULL; tokensIdx++){
		if(strcmp(tokens[tokensIdx],bridge)==0){
			tokensArr[currTokens][tokensArrIdx] = NULL;
			tokensArrIdx=0;
			currTokens++;
            tokensArr[currTokens] = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
		}
		else {
            tokensArr[currTokens][tokensArrIdx] = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
			strcpy(tokensArr[currTokens][tokensArrIdx],tokens[tokensIdx]);
			tokensArrIdx++;
		}
	}
	tokensArr[currTokens+1] = NULL;
	return tokensArr;
}

int *processPID;
int num_pid;

void exitProcess(){
    for(int i=num_pid; i>=0; i--){
        kill(processPID[i],SIGKILL);
        exit(0);
    }
}

void parallelProcessing(char **tokens){
    char cd[] = "cd";
    char exit[] = "exit";
    char parallel[] = "&&&";
    char ***tokensArr = distribute(tokens,parallel);
    int childPID[MAX_NUM_TOKENS];
    for(int i=0; tokensArr[i]!=NULL; i++){
        int pid=fork();
        if(pid==0){
            if(strcmp(tokensArr[i][0],cd)==0){
                chdir(tokensArr[i][1]);
            }
            execvp(tokensArr[i][0],tokensArr[i]);
        }
        else {
            childPID[i] = pid;
        }
    }
    for(int i=0; tokensArr[i]!=NULL; i++){
        int status;
        waitpid(childPID[i],&status,0);
        processPID[num_pid] = childPID[i];
        num_pid++;
    }
}

void seriesProcessing(char **tokens){
    char series[] = "&&";
    char ext[] = "exit";
    if(strcmp(tokens[0],ext)==0){
        exitProcess();
        return;
    }
	char ***tokensArr = distribute(tokens,series);
	for(int i=0; tokensArr[i]!=NULL; i++){
		int pid = fork();
		if(pid==0){
			parallelProcessing(tokensArr[i]);
		}
		else {
			int status;
			waitpid(pid,&status,0);
            processPID[num_pid]=pid;
            num_pid++;
		}
	}
}

int main(int argc, char* argv[]) {

	char  line[MAX_INPUT_SIZE];
	char  **tokens;

	// TODO: YOUR CODE HERE
	// INITIALIZE GLOBAL DATA STRUCTURE OF PIDS TO SOME DEFAULT VALUE  
	// INITIALIZE OTHER VARIABLES AS NEEDED

    num_pid = 0;
    processPID = (int *)malloc(MAX_PROCESSES * sizeof(int));
    processPID[num_pid]=getpid();
    num_pid++;

	while(1) {	

		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		printf("$ ");
		scanf("%[^\n]", line);
		getchar();
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);

		// You can maintain boolean variables to check which type of command is given by user and then
        // conditionally execute depending on them

        // TODO: YOUR CODE HERE
        //
        // TODO: YOUR CODE HERE

		seriesProcessing(tokens);
    
        // freeing the memory
		for(int i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}

		free(tokens);

	}
	return 0;
}


