#include "prompt.h"
#include "headers.h"

void prompt(char home[]) {
    fflush(stdout);
    struct utsname sysName;
    uname(&sysName);
    int len = 10000;
    char name[10000];
    gethostname(name, len);
    printf("<%s@%s:", name, sysName.sysname);
    char address[10000];
    getcwd(address, 10000);
    if( strcmp(home, address) == 0 )
        printf("~");
    else if( strlen(home) < strlen(address) ){
        int l = strlen(home);
        printf("~");
        for(; l < strlen(address) ; l++)
            printf("%c", address[l]);
    }
    else{
        int l = strlen(address) - 1;
        for(; l >= 0 ; l--)
            if( address[l] == '/' )
                break;
        printf("%s", address + l);
    }
    printf(">");
    fflush(stdout);  
}
