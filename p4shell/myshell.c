#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/wait.h> 

void myPrint(char *msg)
{
    write(STDOUT_FILENO, msg, strlen(msg));
}

void parsecommandline(char * pinput, char cmd_buff[], FILE * file_getline){
    //fgets returns null pointer if end of file is reached
    if (!pinput) { //if there is no input
        exit(0);
    }

    // if input is more than 512 characters
    if ((strlen(pinput)>= 513)&&(pinput[strlen(pinput)-1] != '\n')){
        while(1){ // keep looping until next line or end of file is reached
            pinput = fgets(cmd_buff, 514, file_getline); //batch file
            if (pinput == NULL){ //end of file reached
                break; // IS IT CORRECT TO EXIT HERE?
            }
            myPrint(pinput);
            if (pinput[(strlen(pinput)-1)]=='\n'){ //end of long line
                break;
            }
        }
        char error_message[30] = "An error has occurred\n";
        write(STDOUT_FILENO, error_message, strlen(error_message));
        return;
        //UPDATING LIKE THIS GETS SEGFUALT
        //pinput = fgets(cmd_buff, 514, file_getline); //update to next line
        //if (pinput == NULL){ //end of file reached
        //        exit(0);
        //}
        //QUESTION: HOW DO I MOVE ONTO THE NEXT LINE AND THROW AWAY THE COMMAND?
   }

    char* token;
    char* output; // saves output for redirection
    char* rest; //pointer for ; splitting
    char* rest2; //pointer for space and tab splitting
    char* rest3; //pointer for > splitting
    char* filename; //pointer for redirection
    char* single_cmd = strtok_r(pinput, ";\n", &rest);
    char* arr[514];
    char* output_arr[514]; //used to check for output errors
    int i = 0; //counter for number of arguments
    int pos = 0; //counter for redirection checker
    int redirect_count = 0;
    int advanced_redirect = 0; //1 if advanced redirect detected

    //check for > character
    while(pos < strlen(single_cmd)){
        if (single_cmd[pos]=='>'){
            single_cmd[pos] = '\0'; //replace with /0 to split
            *filename = single_cmd[pos + 1];
            redirect_count++;
            if (single_cmd[pos+1]=='+'){ //advanced redirect
                single_cmd[pos+1] = '\0'; //replace with /0 to split string
                *filename = single_cmd[pos + 2];
                advanced_redirect++;
            }
        }
        pos++;
    }

    //error case: multiple redirects
    if ((advanced_redirect > 1) || (redirect_count > 1)){
        char error_message[30] = "An error has occurred\n";
        write(STDOUT_FILENO, error_message, strlen(error_message));
        exit(0);
    }

    //process token into token array
    token = strtok_r(single_cmd, " \t", &rest2);
    while (token != NULL) {
        arr[i] = token;
        token = strtok_r(NULL, " \t", &rest2);
        i++;
    }
    //append null pointer to end of array
    arr[i] = NULL;

    //make array of output, if redirect detected
    if (redirect_count == 1){
        i = 0;
        output = strtok_r(filename, " \t", &rest3);
        //error case: no file specified
        if (output == NULL){
            char error_message[30] = "An error has occurred\n";
            write(STDOUT_FILENO, error_message, strlen(error_message));
            exit(0);
        }
        else{
            while (output != NULL) {
                if (i > 1){ //error case: more than one file specified
                    char error_message[30] = "An error has occurred\n";
                    write(STDOUT_FILENO, error_message, strlen(error_message));
                    exit(0);
                }
                output_arr[i] = output;
                output = strtok_r(NULL, " \t", &filename);
                i++;
            }
        }
        //different output modes depending on redirection mode
        int output_fd;
        if (advanced_redirect == 0){
            output_fd = open(output_arr[0], O_CREAT | O_EXCL);
        }
        else{
            //advanced redirect
            //can make new file, copy new data over, then append original file
            //then rename?
            //may also have to fork
            output_fd = open(output_arr[1], O_CREAT | O_EXCL);    
        }
        //error checker
        if (output_fd == -1){ //error checker
            char error_message[30] = "An error has occurred\n";
            write(STDOUT_FILENO, error_message, strlen(error_message));
            exit(0);
        }

        dup2(output_fd, STDOUT_FILENO);
    }

    //EXIT - built in commands
    if (!strcmp(arr[0], "exit")){
        if (redirect_count == 1){ //error case - built in command with redirect
            char error_message[30] = "An error has occurred\n";
            write(STDOUT_FILENO, error_message, strlen(error_message));
            exit(0);
        }
        exit(0);
    }

    //CD - built in commands
    else if (!strcmp(arr[0], "cd")){
        if (redirect_count == 1){ //error case - built in command with redirect
            char error_message[30] = "An error has occurred\n";
            write(STDOUT_FILENO, error_message, strlen(error_message));
            exit(0);
        }
        else if (arr[2]){ // error case - more than 1 flag
            char error_message[30] = "An error has occurred\n";
            write(STDOUT_FILENO, error_message, strlen(error_message));
        }
        else if (arr[1]){
            chdir(arr[1]);
        }
        else{ //go home if blank
            chdir(getenv("HOME"));
        }
    }
    //PWD - built in commands
    else if (!strcmp(arr[0], "pwd")){
        if (redirect_count == 1){ //error case - built in command with redirect
            char error_message[30] = "An error has occurred\n";
            write(STDOUT_FILENO, error_message, strlen(error_message));
            exit(0);
        }
        char cwd[256];
        getcwd(cwd, sizeof(cwd));
        myPrint(cwd);
        myPrint("\n");
    }

    //NON BUILT IN COMMANDS
    else{
        int pid;
        int errno; // PROPER ERROR HANDLING?
        pid = fork();
            if (!pid){ //child case
                errno = execvp(arr[0], arr);
                if (errno == -1){
                    //second error comes from this
                    char error_message[30] = "An error has occurred\n";
                    write(STDOUT_FILENO, error_message, strlen(error_message));
                    exit(pid);
                }
            }
        wait(NULL);
    }

}


int main(int argc, char *argv[]) 
{
    if (argc == 2){ //one file batch mode
        //parse file input - same as cache
        char *input = argv[1];
        char * pinput;
        char cmd_buff[514];
        FILE * file_getline = fopen(input, "r"); //r indicates read operation
        //while pinput is not null
        while ((pinput = fgets(cmd_buff, 514, file_getline)) !=NULL)
        {
            myPrint(pinput);
            parsecommandline(pinput, cmd_buff, file_getline);
        }
    }

    else if (argc > 2){ //error case: more than one file specified for batch mode
        char error_message[30] = "An error has occurred\n";
        write(STDOUT_FILENO, error_message, strlen(error_message));
        exit(0);
    }

    while (argc == 1){ //interactive mode since argc==1
        char cmd_buff[514];
        char *pinput;
        myPrint("myshell> ");
        pinput = fgets(cmd_buff, 514, stdin);
        parsecommandline(pinput, cmd_buff, stdin);
    }
}

    // need to fork with non-built in commands so shell doesn't exit after execvp returns
    // only child process

    //if only a single command
    //while(single_cmd = strtok_r()){
        // prepare a char * array
        //while(strtok_r(single_cmd, " \t")){

        //}
        
        //fork;
        //execvp();
    //}