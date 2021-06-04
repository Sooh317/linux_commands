#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#define SIZE 512

int main(int argc, char* argv[]){
    int i, word_flag = -1; //flag = 1 if reading a word, otherwize 0
    char buf[SIZE];
    int line = 0, word = 0, byte = 0;

    int fd = open(argv[1], O_RDONLY);

    if(fd == -1){
        perror("cannot open");
        exit(1);
    }
    while(1){
        int cnt = read(fd, buf, sizeof(buf));
        if(cnt == -1){
            perror("read error");
            close(fd);
            exit(1);
        }
        if(cnt == 0) break;
        if(word_flag == -1){
            if(buf[0] == ' ') word_flag = 0;
            else word_flag = 1;
        }
        for(i = 0; i < cnt; i++){
            ++byte;
            if(buf[i] == '\n') ++line;
            if(word_flag == 1){
                if(buf[i] == ' ' || buf[i] == '\n'){
                    ++word;
                    word_flag = 0;
                }
                else continue;
            }
            else{
                if(buf[i] == ' ' || buf[i] == '\n') continue;
                else word_flag = 1;
            }
        }
    }
    if(word_flag == 1) ++word;
    int n = close(fd);
    if(n == -1){
        perror("cannot close");
        exit(1);
    }
    printf("%d %d %d %s\n", line, word, byte, argv[1]);
}
