#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#define BUF_SIZE 1024
struct linux_dirent{
    long d_ino;
    off_t d_off;
    unsigned short d_reclen;
    char d_name[];
};

int main(int argc, char *argv[]){
    int fd, nread, i, j;
    char* c;
    char buf[BUF_SIZE];
    struct linux_dirent *d;
    struct passwd *pw; 
    struct group *grp;
    struct stat stt;

    fd = open(".", O_RDONLY | O_DIRECTORY);
    if(fd == -1){
        perror("OPEN FAILED");
        exit(1);
    }

    stat(".", &stt);

    while(1){
        nread = syscall(SYS_getdents, fd, buf, BUF_SIZE);
        if(nread == -1){
            perror("GETDENTS FAILED");
            exit(1);
        }
        if(nread == 0) break;
        for(i = 0; i < nread;){
//            printf("nread %d i %d\n", nread, i);
            d = (struct linux_dirent *)(buf + i);

            //ファイルタイプ及び権限
            stat(d->d_name, &stt);

            // ls -al には含まれるが ls -lにはない
//printf("%s\n", d->d_name);
            if(d->d_name[0] == '.'){
		i += d->d_reclen;
	        continue; 
	    }

            printf((S_ISDIR(stt.st_mode)) ? "d" : "-");
            printf((stt.st_mode & S_IRUSR) ? "r" : "-");
            printf((stt.st_mode & S_IWUSR) ? "w" : "-");
            printf((stt.st_mode & S_IXUSR) ? "x" : "-");
            printf((stt.st_mode & S_IRGRP) ? "r" : "-");
            printf((stt.st_mode & S_IWGRP) ? "w" : "-");
            printf((stt.st_mode & S_IXGRP) ? "x" : "-");
            printf((stt.st_mode & S_IROTH) ? "r" : "-");
            printf((stt.st_mode & S_IWOTH) ? "w" : "-");
            printf((stt.st_mode & S_IXOTH) ? "x " : "- "); 

            //リンク数
            printf("%ld ",stt.st_nlink);

            //所有者
            pw = getpwuid(stt.st_uid);
            printf("%s ",pw->pw_name);

            //グループ名
            grp = getgrgid(stt.st_gid);
            printf("%s ",grp->gr_name);

            //大きさ
            printf("%4ld ",stt.st_size);

            //時刻
            c = ctime(&stt.st_mtime);
            for(j = 4; j <= 15; j++) printf("%c", c[j]);
            printf(" ");

            //ファイル名/ディレクトリ名
            printf("%s\n", d->d_name);
            i += d->d_reclen;
        }
        
    }

    close(fd);
    if(fd == -1){
        perror("CLOSE FAILED");
        exit(1);
    }
    return 0;
}  
