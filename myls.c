#include "myls.h"
#include "headers.h"

int max(int a, int b){
    if( a > b )
        return a;
    return b;
}

int digits_ctr(long long int a){
    int ctr = 0;
    while( a ){
        ctr++;
        a /= 10;
    }
    return ctr;
}

void ggid(uid_t uid, gid_t gid){
    struct group *a;
    a = getgrgid(gid);
    struct passwd *b;
    b = getpwuid(uid);
    printf("%s %s ", b->pw_name, a->gr_name);
}

void print_time(long int *a){
    struct tm *time;
    time = localtime(a);
    if( time -> tm_mon == 0 )
        printf("Jan");
    else if( time -> tm_mon == 1 )
        printf("Feb");
    else if( time -> tm_mon == 2 )
        printf("Mar");
    else if( time -> tm_mon == 3 )
        printf("Apr");
    else if( time -> tm_mon == 4 )
        printf("May");        
    else if( time -> tm_mon == 5 )
        printf("Jun");
    else if( time -> tm_mon == 6 )
        printf("Jul");
    else if( time -> tm_mon == 7 )
        printf("Aug");
    else if( time -> tm_mon == 8 )
        printf("Sep");
    else if( time -> tm_mon == 9 )
        printf("Oct");
    else if( time -> tm_mon == 10 )
        printf("Nov");
    else if( time -> tm_mon == 11 )
        printf("Dec");
    printf(" %2.0d ", time -> tm_mday);
    if( time -> tm_hour < 10 )
        printf("0");
    printf("%d:", time -> tm_hour);
    if( time -> tm_min < 10 )
        printf("0");     
    printf("%d ", time -> tm_min);
}

void print_attr(long long int a, int b){
    int dig = digits_ctr(a);
    for(int i = dig ; i < b ; i++)
        printf(" ");
    printf("%lld ", a); 
}

void ls_dot(char **inp, int size){
    DIR *dir;
    struct dirent *filesname;
    if( size == 1 ){
        dir = opendir(".");
        if( dir )
            while( (filesname = readdir(dir)) )
                if( filesname -> d_name[0] != '.' )
                    printf("%s\n", filesname -> d_name);
    }
    else{
        for(int i = 1 ; i < size ; i++){
            printf("%s:\n", inp[i]);
            dir = opendir(inp[i]);
            if( dir )
                while( (filesname = readdir(dir)) )
                    if( filesname -> d_name[0] != '.' )
                        printf("%s\n", filesname -> d_name);
            if( dir == NULL )
                printf("Invalid Directory name");
            printf("\n");           
        }
    }
}

void ls_a(char **inp, int size){
    DIR *dir;
    struct dirent *filesname;
    if( size == 2 ){
        dir = opendir(".");
        if( dir )
            while( (filesname = readdir(dir)) )
                printf("%s\n", filesname -> d_name);
    }
    else{
        for(int i = 1 ; i < size ; i++){
            if( strcmp(inp[i], "-a") == 0 )
                continue;
            printf("%s:\n", inp[i]);
            dir = opendir(inp[i]);
            if( dir )
                while( (filesname = readdir(dir)) )
                    printf("%s\n", filesname -> d_name);
            if( dir == NULL ){
                printf("Invalid Directory name");
            }
            printf("\n");           
        }
    }
}

void ls_l(char **inp, int size){
    printf("entered\n");
    DIR *dir;
    struct dirent *filesname;
    if( size == 2 ){
        dir = opendir(".");
        int max_link = 0, max_len_owner = 0, max_len_group = 0, max_size = 0;
        if( dir )
            while( (filesname = readdir(dir)) ){
                if( filesname -> d_name[0] == '.' )
                    continue;       
                struct stat file;
                stat(filesname -> d_name, &file);
                max_link = max(max_link, file.st_nlink);
                max_len_owner = max(max_len_owner, file.st_uid);
                max_len_group = max(max_len_group, file.st_gid);
                max_size = max(max_size, digits_ctr(file.st_size));
            }
        closedir(dir);
        dir = opendir(".");
        if( dir ){
            while( (filesname = readdir(dir)) ){
                if( filesname -> d_name[0] == '.' )
                    continue;
                struct stat file;
                stat(filesname -> d_name, &file);
                if( (file.st_mode & S_IFMT) == S_IFREG )
                    printf("-");
                else if( (file.st_mode & S_IFMT) == S_IFDIR )
                    printf("d");
                if( file.st_mode & S_IRUSR)
                    printf("r");
                else
                    printf("-");
                if( file.st_mode & S_IWUSR)
                    printf("w");
                else
                    printf("-");
                if( file.st_mode & S_IXUSR)
                    printf("x");
                else
                    printf("-");
                if( file.st_mode & S_IRGRP)
                    printf("r");
                else
                    printf("-");
                if( file.st_mode & S_IWGRP)
                    printf("w");
                else
                    printf("-");
                if( file.st_mode & S_IXGRP)
                    printf("x");
                else
                    printf("-");
                if( file.st_mode & S_IROTH)
                    printf("r");
                else
                    printf("-");
                if( file.st_mode & S_IWOTH)
                    printf("w");
                else
                    printf("-");
                if( file.st_mode & S_IXOTH)
                    printf("x ");
                else
                    printf("- ");
                print_attr(file.st_nlink, max_link);
                ggid(file.st_uid, file.st_gid);
                print_attr(file.st_size, max_size);
                print_time(&file.st_mtime);
                printf("%s\n", filesname -> d_name);    
            }
        }
    }
    else{
        for(int i = 1 ; i < size ; i++){
            if( strcmp(inp[i], "-l") == 0 )
                continue;
            printf("%s:\n", inp[i]);
            dir = opendir(inp[i]);
            int max_link = 0, max_len_owner = 0, max_len_group = 0, max_size = 0;
            if( dir )
                while( (filesname = readdir(dir)) ){
                    if( filesname -> d_name[0] == '.' )
                        continue;       
                    struct stat file;
                    stat(filesname -> d_name, &file);
                    max_link = max(max_link, file.st_nlink);
                    max_len_owner = max(max_len_owner, file.st_uid);
                    max_len_group = max(max_len_group, file.st_gid);
                    max_size = max(max_size, digits_ctr(file.st_size));
                }
            if( dir == NULL ){
                printf("Invalid directory name\n");
                continue;
            }
            closedir(dir);
            dir = opendir(inp[i]);
            if( dir ){
                while( (filesname = readdir(dir)) ){
                    if( filesname -> d_name[0] == '.' )
                        continue;
                    struct stat file;
                    stat(filesname -> d_name, &file);
                    if( (file.st_mode & S_IFMT) == S_IFREG )
                        printf("-");
                    else if( (file.st_mode & S_IFMT) == S_IFDIR )
                        printf("d");
                    if( file.st_mode & S_IRUSR)
                        printf("r");
                    else
                        printf("-");
                    if( file.st_mode & S_IWUSR)
                        printf("w");
                    else
                        printf("-");
                    if( file.st_mode & S_IXUSR)
                        printf("x");
                    else
                        printf("-");
                    if( file.st_mode & S_IRGRP)
                        printf("r");
                    else
                        printf("-");
                    if( file.st_mode & S_IWGRP)
                        printf("w");
                    else
                        printf("-");
                    if( file.st_mode & S_IXGRP)
                        printf("x");
                    else
                        printf("-");
                    if( file.st_mode & S_IROTH)
                        printf("r");
                    else
                        printf("-");
                    if( file.st_mode & S_IWOTH)
                        printf("w");
                    else
                        printf("-");
                    if( file.st_mode & S_IXOTH)
                        printf("x ");
                    else
                        printf("- ");
                    print_attr(file.st_nlink, max_link);
                    printf("%d %d ", file.st_uid, file.st_gid);
                    print_attr(file.st_size, max_size);
                    print_time(&file.st_mtime);
                    printf("%s\n", filesname -> d_name);    
                }
            }
        }
    }
}

void ls_la(char **inp, int size){
    DIR *dir;
    struct dirent *filesname;
    if( size == 2 ){
        dir = opendir(".");
        int max_link = 0, max_len_owner = 0, max_len_group = 0, max_size = 0;
        if( dir )
            while( (filesname = readdir(dir)) ){
                if( filesname -> d_name[0] == '.' )
                    continue;       
                struct stat file;
                stat(filesname -> d_name, &file);
                max_link = max(max_link, file.st_nlink);
                max_len_owner = max(max_len_owner, file.st_uid);
                max_len_group = max(max_len_group, file.st_gid);
                max_size = max(max_size, digits_ctr(file.st_size));
            }
        closedir(dir);
        dir = opendir(".");
        if( dir ){
            while( (filesname = readdir(dir)) ){
                if( filesname -> d_name[0] == '.' )
                    continue;
                struct stat file;
                stat(filesname -> d_name, &file);
                if( (file.st_mode & S_IFMT) == S_IFREG )
                    printf("-");
                else if( (file.st_mode & S_IFMT) == S_IFDIR )
                    printf("d");
                if( file.st_mode & S_IRUSR)
                    printf("r");
                else
                    printf("-");
                if( file.st_mode & S_IWUSR)
                    printf("w");
                else
                    printf("-");
                if( file.st_mode & S_IXUSR)
                    printf("x");
                else
                    printf("-");
                if( file.st_mode & S_IRGRP)
                    printf("r");
                else
                    printf("-");
                if( file.st_mode & S_IWGRP)
                    printf("w");
                else
                    printf("-");
                if( file.st_mode & S_IXGRP)
                    printf("x");
                else
                    printf("-");
                if( file.st_mode & S_IROTH)
                    printf("r");
                else
                    printf("-");
                if( file.st_mode & S_IWOTH)
                    printf("w");
                else
                    printf("-");
                if( file.st_mode & S_IXOTH)
                    printf("x ");
                else
                    printf("- ");
                print_attr(file.st_nlink, max_link);
                printf("%d %d ", file.st_uid, file.st_gid);
                print_attr(file.st_size, max_size);
                print_time(&file.st_mtime);
                printf("%s\n", filesname -> d_name);    
            }
        }
        printf("\n");
    }
    else{
        for(int i = 1 ; i < size ; i++){
            if( strcmp(inp[i], "-l") == 0 || strcmp(inp[i], "-a") == 0 || 
                strcmp(inp[i], "-la") == 0 || strcmp(inp[i], "-al") == 0 )
                continue;
            printf("%s:\n", inp[i]);
            dir = opendir(inp[i]);
            int max_link = 0, max_len_owner = 0, max_len_group = 0, max_size = 0;
            if( dir )
                while( (filesname = readdir(dir)) ){     
                    struct stat file;
                    stat(filesname -> d_name, &file);
                    max_link = max(max_link, file.st_nlink);
                    max_len_owner = max(max_len_owner, file.st_uid);
                    max_len_group = max(max_len_group, file.st_gid);
                    max_size = max(max_size, digits_ctr(file.st_size));
                }
            if( dir == NULL ){
                printf("Invalid directory name\n");
                continue;
            }
            closedir(dir);
            dir = opendir(inp[i]);
            if( dir ){
                while( (filesname = readdir(dir)) ){
                    struct stat file;
                    stat(filesname -> d_name, &file);
                    if( (file.st_mode & S_IFMT) == S_IFREG )
                        printf("-");
                    else if( (file.st_mode & S_IFMT) == S_IFDIR )
                        printf("d");
                    if( file.st_mode & S_IRUSR)
                        printf("r");
                    else
                        printf("-");
                    if( file.st_mode & S_IWUSR)
                        printf("w");
                    else
                        printf("-");
                    if( file.st_mode & S_IXUSR)
                        printf("x");
                    else
                        printf("-");
                    if( file.st_mode & S_IRGRP)
                        printf("r");
                    else
                        printf("-");
                    if( file.st_mode & S_IWGRP)
                        printf("w");
                    else
                        printf("-");
                    if( file.st_mode & S_IXGRP)
                        printf("x");
                    else
                        printf("-");
                    if( file.st_mode & S_IROTH)
                        printf("r");
                    else
                        printf("-");
                    if( file.st_mode & S_IWOTH)
                        printf("w");
                    else
                        printf("-");
                    if( file.st_mode & S_IXOTH)
                        printf("x ");
                    else
                        printf("- ");
                    print_attr(file.st_nlink, max_link);
                    printf("%d %d ", file.st_uid, file.st_gid);
                    print_attr(file.st_size, max_size);
                    print_time(&file.st_mtime);
                    printf("%s\n", filesname -> d_name);    
                }
            }
            printf("\n");
        }
    }
}