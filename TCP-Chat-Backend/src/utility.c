#include "utility.h"

pthread_mutex_t stdoutMutex = PTHREAD_MUTEX_INITIALIZER;

void error(const char* szMsg){
    perror(szMsg);
    exit(1);
}

void printToConsole(const char* szMsg, int showPrompt){
    LOCK(&stdoutMutex);
    printf("\r\033[K"); //For clearing the current line

    if(szMsg) printf("%s", szMsg);
    if(showPrompt) printf("%s",PROMPT);

    fflush(stdout);
    UNLOCK(&stdoutMutex);
}