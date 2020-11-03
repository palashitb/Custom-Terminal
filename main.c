int last_pid = 0;
#include "prompt.h"
#include "basiccoms.h"
#include "myls.h"
#include "jobs.h"
#include "headers.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int len = 1e4;
char home[10000];
char job_names[1000][100][50];

long long int min(long long int a, long long int b){
    if( a < b )
        return a;
    return b;
}

void inthandler(int sig_num){
    signal(SIGINT, inthandler);
    if( kill(last_pid, 9) < 0 )
        perror("no running fg process to terminate");
}

void shifter(int i, int size, char **inp){
    int temp = i;
    size++;
    for(; i < size - 1 ; i++){
        inp[i - 2][0] = '\0';
        strcpy(inp[i - 2], inp[i]);
    }
    //inp[size - 2][0] = '\0';
    inp[size - 3] = NULL;  
}
int bkgrnd_jobs[1000][2];

// void bghandler(int sig_num){
//     signal(SIGTSTP, &bghandler);
//     kill(last_pid, SIGTSTP);
//     // if( kill(last_pid, 19) < 0 )
//     //     printf("No foreground process running currently\n");
//     bkgrnd_jobs[bkgrnd_jobs[0][0]][0] = last_pid + 1;
//     bkgrnd_jobs[0][0]++;
// }

int main()
{
    bkgrnd_jobs[0][0] = 1;
    int L = 1e5, cur = -1, size_yet = 0;
    char hist[20][10000];
    signal(SIGINT, inthandler);
    getcwd(home, len);
    char last_dir[1000];
    int grandpid = getpid();
    while (1)
    {
        prompt(home);
        
        char sf[20][20][20][100], stemp[800000], st2[20][40000], st3[20][20][2000];
        stemp[0] = '\0';
        if( NULL == read(0, stemp, 800000) )
            exit(1);
        if( strcmp(stemp, "\n") == 0 ){
            printf("no command entered. Please enter a command\n");
            continue;
        }
        for(int i = 0 ; i < strlen(stemp) ; i++)
            if( stemp[i] == '\n' ){
                stemp[i] = '\0';
                break;
            }
        // printf("%s * \n", stemp);
        char *a = strtok(stemp, ";");
        int sz1 = 0;
        while( a != NULL ){
            sprintf(st2[sz1], "%s", a);
            a = strtok(NULL, ";");
            if( st2[sz1][strlen(st2[sz1]) - 1] == '\n' ){
                st2[sz1][strlen(st2[sz1]) - 1] = '\0';
            }
            sz1++;
        }   
        int sizear[20];
        for(int i = 0 ; i < sz1 ; i++){
            char *a = strtok(st2[i], "|");
            int sz2 = 0;
            while( a != NULL ){
                sprintf(st3[i][sz2], "%s", a);
                a = strtok(NULL, "|");
                sz2++;
            }
            sizear[i] = sz2;
        }
        for(int i = 0 ; i < sz1 ; i++){
            for(int j = 0 ; j < sizear[i] ; j++){
                char *b = strtok(st3[i][j], " \t");
                int sz3 = 0;
                while( b != NULL ){
                    sprintf(sf[i][j][sz3], "%s", b);
                    b = strtok(NULL, " \t");
                    // printf("%s %%\n", sf[i][j][sz3]);
                    sz3++;
                }
                for(; sz3 < 20 ; sz3++)
                    sf[i][j][sz3][0] = '\0';
                // sprintf(sf[i][j][sz3], "%s", b);
            }
        }
        for(int ijk = 0 ; ijk < sz1 ; ijk++){
            int lol = 0;
            int ctr = 0;
            int noOfPipies = sizear[ijk] + 1, status, index = 0;
            pid_t pipi;
            int pipefds[2 * noOfPipies];
            for(int ist = 0 ; ist < noOfPipies ; ist++)
                if( pipe(pipefds + ist * 2) < 0 )
                    perror("piping error");
                    
            for(int lol = 0 ; lol < 1 ; lol++){    
                    
                if( strcmp(sf[ijk][lol][0], "jobs") == 0 ){
                    jobs(bkgrnd_jobs);
                    continue;
                }

                else if( strcmp(sf[ijk][lol][0], "cd") == 0 ){
                    my_cd(sf[ijk][lol][1], home, last_dir);
                    continue;
                }

                else if( strcmp(sf[ijk][lol][0], "pwd") == 0 ){
                    my_pwd(home);
                    continue;
                }

                int sizer = 1;
                cur = (cur + 1) % 20;
                strcpy(hist[cur], sf[ijk][lol][0]);
                size_yet = min(20, size_yet + 1);
                for(int ij = 0 ; strlen(sf[ijk][lol][ij]) > 0 ; ij++)
                    sizer = ij + 1;
                auto void redirect(int k, char *templ[20]);
                if( sizear[ijk] == 1 )
                {int forky = 0;
                forky = fork();
                if( forky ){   
                    if( strcmp(sf[ijk][lol][sizer - 1], "&") == 0 ){
                        bkgrnd_jobs[bkgrnd_jobs[0][0]][0] = forky + 1;
                        bkgrnd_jobs[0][0]++;
                    }
                    else{
                        last_pid = forky;
                        int statusw;
                        wait(NULL);
                        if( lol + 1 == sizear[ijk] )
                        for(int iu = 0 ; iu < 2 * noOfPipies ; iu++)
                           close(pipefds[iu]);
                        fflush(stdout);
                    }
                    ctr+= 2;
                    continue;
                }}
                else{
                    int k = 0;
                    while( k < sizear[ijk] ){
                        
                        int f = fork();

                        if( f == 0 ){
                            if( k + 1 < sizear[ijk] ){
                                if( dup2(pipefds[ctr + 1], 1) < 0 )
                                    perror("dup2(piping) error");
                                
                            }
                            if( k > 0 ){
                                if( dup2(pipefds[ctr - 2], 0) < 0 )
                                    perror("dup2 failure");
                            }
                            for(int irt = 0 ; irt < 2 * noOfPipies ; irt++)
                                close(pipefds[irt]);
                            
                            char *inppp[20];
                            redirect(k, inppp);
                            execvp(inppp[0], inppp);
                        }
                        k++;
                        ctr += 2;
                    }
                    for(int ii = 0 ; ii < 2 * noOfPipies ; ii++)
                        close(pipefds[ii]);
                    for(int ii = 0 ; ii <= noOfPipies ; ii++)
                        wait(NULL);
                    continue;
                }
                char *temple[20];

                void redirect(int k, char *templ[20]){
                //redirection part
                sizer = 0;
                for(int ii = 0 ; strlen(sf[ijk][k][ii]) > 0  ; ii++){
                    templ[ii] = sf[ijk][k][ii];
                    sizer++;
                }

                templ[sizer] = NULL;
		        int tempee = sizer;
                for(int ila = 0 ; ila < tempee ; ila++){
                    if( strcmp(sf[ijk][k][ila], "<") == 0 ){
                        int fdin = open(sf[ijk][k][ila + 1], 0 | O_NONBLOCK, 0644);
                        if( fdin < 0 ){
                            perror("redirection");
                            printf("Specified file for sf[ijk][lol]ut could not be opened\n");
                        }
                        dup2(fdin, 0);
                        shifter(ila + 2, sizer, templ);
                        sizer -= 2;
                        close(fdin);
                        ila++;
                        continue;
                    }
                    if( strcmp(sf[ijk][k][ila], ">") == 0 ){
                        int fdout = open(sf[ijk][k][ila + 1], 01100 | O_WRONLY | O_NONBLOCK, 0644);
                        if( fdout < 0 ){
                            perror("redirection");
                            printf("Specified file for output redirection could not be opened\n");
                        }
                        dup2(fdout, 1);
                        shifter(ila + 2, sizer, templ);
                        sizer -= 2;
                        ila++;
                        close(fdout);
                        continue;
                    }
                    if( strcmp(sf[ijk][k][ila], ">>") == 0 ){
                        int fdout = open(sf[ijk][k][ila + 1], O_WRONLY | O_CREAT | O_APPEND | O_NONBLOCK, 0644);
                        if( fdout < 0 ){
                            perror("redirection");
                            printf("Specified file for output redirection could not be opened\n");
                        }
                        dup2(fdout, 1);
                        close(fdout);
                        shifter(ila + 2, sizer, templ);
                        sizer -= 2;
                        ila++;
                        close(fdout);
                        continue;
                    }
                }
                }
                redirect(0, temple);

                if( sizear[ijk] > 1 ){
                    int forklift = fork();
                    if( forklift == 0 )
                    {
                        execvp(temple[0], temple);
                    }
                    if( sizear[ijk] - 1 == lol ){
                        for(int ii = 0 ; ii < 2 * noOfPipies ; ii++)
                            close(pipefds[ii]);
                        for(int ii = 0 ; ii < sizear[ijk] ; ii++)
                            wait(NULL);
                    }
                    
                }
                if( strcmp(temple[sizer - 1], "&") == 0 ){
                    temple[sizer - 1] = NULL;
                    sizer--;
                    int ara = fork();
                    if( ara == 0 ){
                        printf("Background Pid: %d\n", getpid());
                        execvp(temple[0], temple);
                    }
                    wait(NULL);
                    printf("\n%s with PID: %d finished exection\n", st2[ijk], getpid() + 1);
                    continue; 
                }

                

                
                else if( strcmp(temple[0], "echo") == 0 ){
                    my_echo(temple, sizer + 1);
                }

                else if( strcmp(temple[0], "kjob") == 0 ){
                    int a = 0, b = 0, temp = 1;
                    for(int i = strlen(temple[1]) - 1 ; i >= 0 ; i--){
                        a += temp * (temple[1][i] - '0');
                        temp *= 10;
                    }
                    temp = 1;
                    for(int i = strlen(temple[2]) - 1 ; i >= 0 ; i--){
                        b += temp * (temple[2][i] - '0');
                        temp *= 10;
                    }
                    int job_pid;
                    for(int i = 1 ; i < bkgrnd_jobs[0][0] ; i++){
                        if( bkgrnd_jobs[i][1] == a ){
                            job_pid = bkgrnd_jobs[i][0];
                            break;
                        }
                    }
                    kjob(job_pid, b);
                }

                else if( strcmp(temple[0], "fg") == 0 ){
                    int a = 0, temp = 1;
                    for(int i = strlen(temple[1]) - 1 ; i >= 0 ; i--){
                        a += temp * (temple[1][i] - '0');
                        temp *= 10;
                    }
                    int job_pid;
                    for(int i = 1 ; i < bkgrnd_jobs[0][0] ; i++)
                        if( bkgrnd_jobs[i][1] == a ){
                            job_pid = bkgrnd_jobs[i][0];
                            break;
                        }
                    fg_bg(job_pid, 0);
                }

                else if( strcmp(temple[0], "bg") == 0 ){
                    int a = 0, temp = 1;
                    for(int i = strlen(temple[1]) - 1 ; i >= 0 ; i--){
                        a += temp * (temple[1][i] - '0');
                        temp *= 10;
                    }
                    int job_pid;
                    for(int i = 1 ; i < bkgrnd_jobs[0][0] ; i++)
                        if( bkgrnd_jobs[i][1] == a ){
                            job_pid = bkgrnd_jobs[i][0];
                            break;
                        }
                    fg_bg(job_pid, 1);
                }
                
                else if( strcmp(temple[0], "overkill") == 0 ){  //just need to verify once which commands were used to overkill
                    for(int i = 1 ; i < bkgrnd_jobs[0][0] ; i++)
                        if( kill(bkgrnd_jobs[i][0], 0) == 0 )
                            kill(bkgrnd_jobs[i][0], SIGKILL);
                }

                else if( strcmp(temple[0], "history") == 0 )
                    my_history(temple, sizer, hist, cur, size_yet);
                
                else if( strcmp(temple[0], "quit") == 0 )
                    kill(grandpid, SIGTERM);
                // printf("%s haha\n", sf[ijk][lol][0]);
                else if( strcmp(temple[0], "ls") == 0 ){
                    // printf("are we cool?\n");
                    _Bool flagl = 0, flaga = 0;
                    char *temp;
                    for(int i = 1 ; i < sizer ; i++){
                        if( strcmp(temple[i], "-l") == 0 )
                            flagl = 1;
                        else if( strcmp(temple[i], "-a") == 0 )
                            flaga = 1;                        
                        else if( strcmp(temple[i], "-la") == 0 || strcmp(temple[i], "-al") == 0 ){
                            flagl = 1;
                            flaga = 1;
                        }
                    }
                    if( !flagl && !flaga )
                        ls_dot(temple, sizer);
                    else if( !flagl )
                        ls_a(temple, sizer);
                    else if( !flaga )
                        ls_l(temple, sizer);
                    else
                        ls_la(temple, sizer);
                }
                else if( strcmp(temple[0], "setenv") == 0 ){
                    char *nul;
                    nul = NULL;
                    if( sizer == 1 || sizer > 3 )
                        write(1, "Either too many or too less arguments passed with setenv command\n",66);
                    else if( sizer == 2 ){
                        if( setenv(temple[1], nul, 1) < 0 )
                            perror("setenv error");
                    }
                    else{
                        if( setenv(temple[1], temple[2], 1) < 0 )
                            perror("setenv error");
                    }
                }
                else if( strcmp(temple[0], "unsetenv") == 0 ){
                    if( sizer == 1 )
                        write(1, "Too few arguments passed with unsetenv\n", 40);
                    else if( sizer > 2 )
                        write(1, "Too many arguments passed with unsetenv\n", 41);
                    else if( -1 == unsetenv(temple[1]) )
                        perror("unsetenv error");
                }
                
                else if( strcmp(temple[0], "pinfo") == 0 )                
                    pinfo(temple, sizer - 1);
                
                else{
                    execvp(temple[0], temple);
                }
                fflush(stdout);
                return 0;                
            }

        }
    }
}
