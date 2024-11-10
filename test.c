#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#define MAXLINE 1000

struct termios oldt, newt;

void nc_mode(){ // Устанавливаем терминал в неканонический режим (без буферизации)
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}
void c_mode(){ // Восстанавливаем старые настройки терминала
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);   
}

int getln(char s[], int length){
    int c, i;

    for (i = 0; i < length-1 && (c = getchar()) != 27 && c !='\n'; ++i) {putchar(c); s[i] = c;}
    s[i] = '\0';
    printf("\n");
    return i;
}

void copy(char to[], char from[]){
    int i = 0;

    while ((to[i] = from[i]) != '\0') ++i;
    to[i] = '\0';
}

void space_remover(char to[], char from[]){
    int f = 0, t = 0;
    int space = 0;

    while (from[f] !='\0'){
        if (from[f] == ' ' || from[f] == '\t'){
            if (space == 0){
                to[t] = ' ';
                space = 1;
                t++;
                f++;
            } else f++;
        } else {
            to[t] = from[f];
            f++;
            t++;
            space = 0;
        }
    }
}

int main(){
    int length_current;
    int length_max = 0;

    char line_in[MAXLINE];
    char line_out[MAXLINE];

    nc_mode();

    getln(line_in, MAXLINE);
    space_remover(line_out, line_in);

    printf("\nString ready: %s\n\n", line_out);

    c_mode();
    return 0;
}
