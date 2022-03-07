#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define SETUID_ERROR -1
#define SUCCESS 0
#define ERROR_OPENING_FILE 1 
#define ERROR_CLOSING_FILE 2
#define CAN_NOT_CHANGE_EFF_ID 3

void print_real_effective_id(){
    printf("Real User ID = %d\n", getuid());
    printf("Effective User ID = %d\n\n", geteuid());
}

int open_file() {
    FILE* file = fopen("test.txt", "r");
    
    if(file == NULL){
        perror("Couldn't open file");
        return ERROR_OPENING_FILE;
    }

    int result_fclose = fclose(file);
    if(flag == EOF) {
        perror("Couldn't close file");
        return ERROR_CLOSING_FILE;
    }
    return SUCCESS;
}

int main() {
    int err = SUCCESS;
    print_real_effective_id();
    err = open_file();

    if(err != SUCCES) return err;

    int result_setuid = setuid(getuid()); 

    if(result_setuid == SETUID_ERROR) {
        perror("Couldn't set same id");
        return CAN_NOT_CHANGE_EFF_ID; 
    }

    print_real_effective_id();
    err = open_file();

    if(err != SUCCESS) return err;

    return 0;
};





