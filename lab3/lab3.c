#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define SUCCESS 0
#define ERROR_OPENING_FILE 1 
#define ERROR_CLOSING_FILE 2
#define CAN_NOT_CHANGE_EFF_ID 3

void print_real_effective_id(){
    printf("Real User ID = %d\n",getuid());
    printf("Effective User ID = %d\n\n",geteuid());
}

int open_file() {
    FILE* file = fopen("test.txt", "r");
    
    if(file == NULL){
        perror("Couldn't open file");
        return ERROR_OPENING_FILE;
    }

    int flag = fclose(file);
    if(flag == 0) {
        perror("Couldn't close file");
        return ERROR_CLOSING_FILE;
    }
    return SUCCESS;
}

int main() {
    int err = 0;
    print_real_effective_id();
    err = open_file();

    if(err) return err;

    int flag = setuid(getuid()); 

    if(flag == -1) {
        perror("Couldn't set same id");
        return CAN_NOT_CHANGE_EFF_ID; 
    }

    print_real_effective_id();
    err = open_file();

    if(err) return err;

    return 0;
};





