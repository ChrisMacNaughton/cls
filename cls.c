#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <string.h>

int read_dir(char*);
int main(int argc, char *argv[])
{
    char *path;
    if(argc>1){
        path = argv[1];
        // dir = opendir(argv[1]); /*your directory*/
    } else {
        path = ".";
        // dir = opendir("."); /* Default: current path */
    }
    return read_dir(path);
}

int read_dir(char* path) {
    DIR *dir;
    struct dirent *de;
    struct stat file_stat;
    struct passwd  *pwd;
    struct group   *grp;
    struct tm      *tm;
    char            datestring[256];
    char * filename = NULL;
    dir = opendir(path);
    if (dir == NULL) {
        printf("Cannot open directory '%s'\n", path);
        return 1;
    }

    while(dir)
    {
        de = readdir(dir);
        if (!de) break;

            asprintf(&filename, "%s/%s", path, de->d_name);

            if(stat(filename, &file_stat) == -1)
                continue;

            // if we're a file
            if(!S_ISDIR(file_stat.st_mode) || *de->d_name == '.'){
                // print out permissions
                printf( (S_ISDIR(file_stat.st_mode)) ? "d" : "-");
                printf( (file_stat.st_mode & S_IRUSR) ? "r" : "-");
                printf( (file_stat.st_mode & S_IWUSR) ? "w" : "-");
                printf( (file_stat.st_mode & S_IXUSR) ? "x" : "-");
                printf( (file_stat.st_mode & S_IRGRP) ? "r" : "-");
                printf( (file_stat.st_mode & S_IWGRP) ? "w" : "-");
                printf( (file_stat.st_mode & S_IXGRP) ? "x" : "-");
                printf( (file_stat.st_mode & S_IROTH) ? "r" : "-");
                printf( (file_stat.st_mode & S_IWOTH) ? "w" : "-");
                printf( (file_stat.st_mode & S_IXOTH) ? "x" : "-");
                printf(" %4d", file_stat.st_nlink);

                // print out user and group names if found
                if ((pwd = getpwuid(file_stat.st_uid)) != NULL)
                    printf(" %-8.9s", pwd->pw_name);
                else
                    printf(" %-8d", file_stat.st_uid);

                if ((grp = getgrgid(file_stat.st_gid)) != NULL)
                    printf(" %-8.8s", grp->gr_name);
                else
                    printf(" %-8d", file_stat.st_gid);

                // print size of file
                printf(" %9jd", (intmax_t)file_stat.st_size);


                // Get localized date string.
                tm = localtime(&file_stat.st_mtime);
                strftime(datestring, sizeof(datestring), "%b %d %H:%M", tm);

                printf(" %s %s\n", datestring, de->d_name);
            } else {
                printf("%s:\n", filename);
                asprintf(&filename, "%s", filename);
                read_dir(filename);
            }
    }
    puts("\n");
    closedir(dir);
    return(0);
}