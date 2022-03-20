#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define SUCCESS 0
#define PUTENV_ENOMEM 1
#define TIME_ERROR (time_t)-1
#define CTIME_ERROR 2

int main() {
    time_t now;

    // int putenv(char* string) - изменяет или добавляет значение переменной среды. string должна имет вид NAME=VALUE. Также, если просто
    // вызвать putenv(NAME), то произойдет удаление NAME из среды.
    // В случае успеха возвращет 0, иначе не 0.

    // Попытка изменить переменную среды TZ.
    int putenv_result = putenv("TZ=America/Los_Angeles");
    if(putenv_result != SUCCESS) {
        perror("Couldn't change TZ");
        return PUTENV_ENOMEM;
    }
    
    
    // time возвращает количество секунд, прошедших с 00:00:00 1 января 1970 года времени UTC
    // -1 в случае неудачи

    // Пытаемся получить время в секундах
    time_t time_result = time(&now);
    if(time_result == TIME_ERROR) {
        perror("Couldn't get time");
        return TIME_ERROR;
    }
    

    // char* ctime(time_t) в качестве агрумента используют тип данных time_t , представляющий собой календарное время.
    // Функция интерпетирует количество секунд, прошедших с 00:00:00 1 января 1970 года времени UTC, как строку и возвращет ее.

    // Перевод секунд в строку, содержащую текущую дату
    char* string_date = ctime(&now);
    if (string_date == NULL) {
        perror("Couldn't get time as string");
        return CTIME_ERROR;
    }

    // Печать времени
    printf("Time in California: ");
    printf("%s \n", string_date);
    return SUCCESS;
} 
