#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<string.h>

#define MAXLOCKS 20

int main() {

    int maxLines = 200; // Maximum number of lines you want to read
    char lines[maxLines][32]; // Assuming each line can have up to 31 characters (plus the null-terminator)

    //Initialization
    int command[maxLines];
    int commandValue[maxLines];
    char* to_file[maxLines];
    char* from_file[maxLines];

    for(int i = 0; i < maxLines; i++){
        command[i] = -1; 
        commandValue[i] = -1;
    }

    /*  command
        -1 -> Empty after that
        1 -> read
        2 -> write

        command value
        1 -> write of type 1
        2 -> write of type 2

    */

    int lineCount = 0;
    while (lineCount < maxLines && fgets(lines[lineCount], sizeof(lines[0]), stdin)) {
        // Remove the newline character at the end of the line
        lines[lineCount][strcspn(lines[lineCount], "\n")] = '\0';
        if(!strcmp(lines[lineCount],"exit")) break;
        lineCount++;
    }


        char input[256];
        fgets(input, sizeof(input), stdin);
        input[strlen(input) - 1] = '\0';

        if (strcmp(input, "exit") == 0) {
            break;
        } else if (strncmp(input, "read", 4) == 0) {
            const char *file_name = input + 5;
            pthread_t reader_thread;
            pthread_create(&reader_thread, NULL, reader_thread, (void *)file_name);
            pthread_join(reader_thread, NULL);
        } else if (strncmp(input, "write", 5) == 0) {
            const char *args = input + 6;
            pthread_t writer_thread;
            pthread_create(&writer_thread, NULL, writer_thread, (void *)args);
            pthread_join(writer_thread, NULL);
        }


    for (int i = 0; i < lineCount; i++) {
        int value;
        if (sscanf(lines[i], "read %d", &value) == 1) {
            commandArray[i] = 1;
            commandValue[i] = value;
        } else if(sscanf(lines[i], "delete %d", &value) == 1){
            commandArray[i] = 2;
            commandValue[i] = value;
        } else if(sscanf(lines[i], "contains %d", &value) == 1){
            commandArray[i] = 3;
            commandValue[i] = value;
        } 
        else{
            printf("Invalid Command\n");
        }
    }

    //Input taken  

    //Now make threads for all above commands!
    //We need to make function pointers for each of insert, 
    pthread_t thread_id[MAXTHREADS]; // Thread identifier
    
    for(int i =0; i<lineCount; i++){
        // Create a new thread and pass the argument
        //Chnage the arguments and function pointers after making them
        if(commandArray[i] == 1 ){
            pthread_create(&thread_id[i], NULL, insert, &commandValue[i]);
        }
        else if(commandArray[i] == 2 ){
            pthread_create(&thread_id[i], NULL, delete, &commandValue[i]);
        }
        else if(commandArray[i] == 3 ){
            pthread_create(&thread_id[i], NULL, contains, &commandValue[i]);
        }
        else if(commandArray[i] == 4 ){
            pthread_create(&thread_id[i], NULL, inOrder, NULL);
        }
    
    }
        //Need to Wait for all the threads to finish
        //Use pthread_join to do this!
        
        //Maintain an output array to print output to console at the very end.

        //
        
    
    return 0;
}



