#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define SUCCESS 0
#define ERROR_OPENING_FILE 1 
#define ERROR_CLOSING_FILE 2
#define ERROR_CHANGING_EFF_ID -1

// У процесса есть реальный и эффективный идентификаторы пользователя
// getuid - возвращает реальный идентификатор пользователя, те того, который запустил процесс.
// geteuid - возвращает эффективный идентификатор пользователя, те того, кто владеет программой.(используется для определения прав доступа к файлам)

// Исполняемые файлы могут иметь биты установки идентификатора пользователя (SUID бит), которые мы можем установить при помощи команды chmod
// или системного вызова chmod(). И если до запуска такого исполняемого файла этот бит установлен, то эффективный идентификатор пользователя процесса
// становится таким же, как и у владельца.

// Зачем это надо: Используя SUID-бит можно получить доступ к недостижимому файлу, если владелец этого файла напишет программу и 
// установит этот бит. Например, пользователи могут менять свой пароль, хотя доступ к файлу с паролями есть только у его владельца(root).

// В нашей программе в первом варианте будет два "Couldn't open file", тк SUID бит не установлен, а то, что мы поменяли эффективный адрес на реальный
// ничем не поможет.
// Но во втором, вначале мы сможем открыть файл из-за установленного бита, но потом мы сменим этот бит с эффективного на реальный и прав опять не будет.

// Печать реального и эффективного индентификатора
void print_real_effective_id(){
    printf("Real User ID = %d\n", getuid());
    printf("Effective User ID = %d\n", geteuid());
}

int open_file() {
    FILE* file = fopen("test.txt", "r");

    if(file == NULL){
        perror("Couldn't open file");
        return ERROR_OPENING_FILE;
    }

    int fclose_result = fclose(file);
    // EOF - end of file - код ошибки, если файл не удалось закрыть
    if(fclose_result == EOF) {
        perror("Couldn't close file");
        return ERROR_CLOSING_FILE;
    }

    printf("File opened successfully\n\n");
    return SUCCESS;
}

int main() {
    int error = SUCCESS;
    // Печать эффективного и реального идентфикатора пользователя
    print_real_effective_id();

    // После первой попытки открытия в случае неудачи надо все равно попробовать открыть второй раз
    error = open_file();

    // Устанавливаем эффективный идентификатор равным реальному
    int setuid_result = setuid(getuid());

    if(setuid_result == ERROR_CHANGING_EFF_ID) {
        perror("Couldn't set effective id");
        // Результат второго открытия не поменяется, так что возвращаем код ошибки
        return ERROR_CHANGING_EFF_ID;
    }

    print_real_effective_id();
    
    // Второй раз пытаемся открыть файл, уже с измененным битом
    error = open_file();

    // Возвращаю код ошибки/успеха
    return error; 
};