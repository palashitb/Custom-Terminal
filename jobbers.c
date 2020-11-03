#include "headers.h"
#include "jobs.h"

void jobs(int bkgrnd_jobs[][2]){
    // printf("hello %d\n", bkgrnd_jobs[0][0]);
    int ctr = 1;
    // printf("Hi\n");
    int job_size = bkgrnd_jobs[0][0];
    // printf("size is %d\n", job_size);
    for(int i = 1 ; i < job_size ; i++){
        if( kill(bkgrnd_jobs[i][0], 0) == 0 ){
            char temp1[10], address[1000], line[100], comnd[1000];              
            printf("[%d]", ctr);
            temp1[4] = '\0';
            sprintf(address, "/proc/%d/status", bkgrnd_jobs[i][0]);
            bkgrnd_jobs[i][1] = ctr;
            FILE *file;
            file = fopen(address, "r");
            if( file == NULL ){
                perror("error");
                printf("Entered pid does not exist\n");
            }
            while( fgets(line, 100, file) )
                if( strncmp(line, "State:", 6) == 0 ){
                    char *token = strtok(line, " \t");
                    token = strtok(NULL, " \t");
                    char *c = token;
                    if( strcmp(c, "T") == 0 )
                        printf("  Stopped ");
                    else
                        printf("  Running ");
                }
            fclose(file);
            sprintf(comnd, "/proc/%d/cmdline", bkgrnd_jobs[i][0]);
            file = fopen(comnd, "r");
            char cmd[1000];
            fgets(cmd, 1000, file);
            printf("%s [%d]\n", cmd, bkgrnd_jobs[i][0]);
            ctr++;
        }
        else
            bkgrnd_jobs[i][1] = 0;
        
    }
    fflush(stdout);
}

void kjob(int job_pid, int sig){
    if( kill(job_pid, 0) == 0 )
        kill(job_pid, sig);
    else
        printf("No job with the given no. exists. It may have completed its execution\n");
}

void fg_bg(int job_pid, int flag){
    int *l;
    if( kill(job_pid, 0) == 0 ){
        kill(job_pid, SIGCONT);
        if( flag == 0 )
            waitpid(job_pid, l, 0);
    }
    else
        printf("No job with the given no. exists. It may have completed its executio\n");
}