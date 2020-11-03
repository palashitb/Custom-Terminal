#include "basiccoms.h"
#include "headers.h"

void my_pwd(char home[]){
    char address[10000];
    getcwd(address, 10000);
    if( address == NULL ){
        perror("error");
        printf("some error while using getcwd\n");
    }
    char first[3] = " ~";
    if( strcmp(address, home) == 0 ){
        write(STDOUT_FILENO, " ~\n", 4);
        fflush(stdout);
    }
    else{
        int l = 0;
        while( l < strlen(home) )
            l++;
        printf(" ~");
        for( ; l < strlen(address) ; l++)
            printf("%c", address[l]);
        printf("\n");
    }
}

void my_cd(char address[], char my_home[], char last_dir[]){
    char new[10000], home[10000];
    getcwd(home, 10000);
     if( home == NULL ){
        perror("error");
        printf("some error while using getcwd\n");
    }
    if( strcmp(address, "-") == 0 ){
        if( strlen(last_dir) == 0 ){
            getcwd(last_dir, 1000);
            printf("~\n");
        }
        else{
            char temp[1000];
            strcpy(temp, last_dir);
            if( chdir(last_dir) < 0 )
                perror("'cd -' error");
            my_pwd(my_home);
            strcpy(last_dir, temp);
        }
        return;
    }
    else
        getcwd(last_dir, 1000);
    char *temp = "~";
    if( strcmp(address, "~") == 0 ){
        while( strcmp(home, my_home) != 0 ){
            if( chdir("..") < 0 ){
                printf("Cannot go back any further\n");
                continue;
            }
            getcwd(home, 10000);
        }
    }
    else if( chdir(address) != 0 ){
        perror("invalid path or directory entered\n");
        return;
    }
    home[0] = '\0';
    getcwd(home, 10000);
    if( home == NULL ){
        perror("error");
        printf("some error while using getcwd\n");
    }
}

void my_echo(char **inp, int size){
    for(int i = 1 ; i < size - 1 ; i++)
        printf("%s ", inp[i]);
    printf("\n");
}

void pinfo(char **inp, int size){
    if( size == 1 ){
        int pid = getpid();
        printf("PID: %d", pid);
        char address[100], *p, line[100];
        sprintf(address, "/proc/%d/status", pid);
        FILE *file;
        file = fopen(address, "r");
        if( file == NULL ){
            perror("error");
            printf("Entered pid does not exist\n");
        }
        while( fgets(line, 100, file)) {
            if( strncmp(line, "State:", 6) == 0 )
                printf("%s", line);
            if( strncmp(line, "VmSize:", 7) == 0 ){
                int ctr = 0;
                while( line[ctr] < '0' || line[ctr] > '9' )
                    ctr++;
                printf("Memory: %s", line + ctr);
            }
        }
        sprintf(address, "/proc/%d/exe", pid);
        char buf[1000];
        if( readlink(address, buf, 1000) < 0 )
            perror("error");
        printf("Executable Path: %s\n", buf);
        fclose(file);
    }
    else{
        int temp = 1, si = strlen(inp[1]), pid = 0;
        for(int i = si - 1 ; i >= 0 ; i--)
            pid += temp * (inp[1][i] - '0');
        char address[100], *p, line[100];
        printf("PID: %d", pid);
        sprintf(address, "/proc/%d/status", pid);
        FILE *file;
        file = fopen(address, "r");
        if( file == NULL ){
            perror("error");
            printf("error opening file for pinfo");
        }
        while( fgets(line, 100, file)) {
            if( strncmp(line, "State:", 6) == 0 )
                printf("%s", line);
            if( strncmp(line, "VmSize:", 7) == 0 ){
                int ctr = 0;
                while( line[ctr] < '0' || line[ctr] > '9' )
                    ctr++;
                printf("Memory: %s", line + ctr);
            }
        }
        char buf[1000];
        if(readlink(address, buf, 1000) < 0 ){
            perror("error");        
            printf("error reading path\n");
        }
        printf("Executable Path: %s\n", buf);
        if( fclose(file) < 0 ){
            perror("error");
            printf("error closing file for pinfo\n");
        }
    }
}

void my_history(char **inp,int size, char hist[20][10000], int cur, int size_yet){
    if( size == 1 ){
        // printf("atleast\n%d %d\n", cur, size_yet);
        int ctr = 0;
        for(int i = cur ; i >= 0 ; i = (i + 19) % 20 ){
            if( i > size_yet - 1 )
                break;
            ctr++;
            // printf("%d\n", i);
            printf("%s\n", hist[i]);
            if( ctr == 10 )
                break;
        }
    }
    else{
        int ctr = 0, lim = 0;
        if( strlen(inp[1]) == 1 )
            lim = inp[1][0] - '0';
        else
            lim = (inp[1][0] - '0') * 10 + inp[1][1] - '0';
        for(int i = cur ; i >= 0 ; i = (i + 19) % 20 ){
            if( i >= size_yet )
                break;
            if( ctr == lim )
                break;
            printf("%s\n", hist[i]);
            ctr++;
        }
    }
}