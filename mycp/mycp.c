#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define SIZE 512

int main(int argc, char *argv[]){
    int fd_s, fd_t, res_s, res_t, cnt_r;
    int i;
    char buf[SIZE];

    fd_s = open(argv[1], O_RDONLY);
    fd_t = open(argv[2], O_CREAT | O_TRUNC | O_RDWR, 0644);

    if(fd_s == -1){
        perror("cannot open file");
        return 1;
    }
    
    while(1){
        cnt_r = read(fd_s, buf, sizeof(buf));
        if(cnt_r == -1){
            perror("cannot read file");
            return 1;
        }
        if(cnt_r == 0) break;
        while(1){
            int count = write(fd_t, buf, cnt_r);
            if(count == -1){
                perror("cannot write to file");
                return 1;
            }
            if(count == 0) break;
            cnt_r -= count;
            for(i = count; i < SIZE; i++) buf[i - count] = buf[i];
        }
    }

    res_s = close(fd_s);
    res_t = close(fd_t);
    if(res_s == -1){
        perror("error : cannot close SOURCE");
        return 1;
    }
    if(res_t == -1){
        perror("error : cannot close TARGET");
        return 1;
    }
}