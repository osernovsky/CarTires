#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int main()
{
    struct termios oldt, newt;
    int c;
    int chars[255] = {0};

    // Получаем текущие настройки терминала
    // и устанавливаем терминал в неканонический режим (без буферизации)
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Отключаем канонический режим и эхо
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    printf("Нажимайте клавишу ESC для выхода:\n");

    while ((c = getchar()) != 27){ // 4 это EOF - CTRL+D
        putchar(c);
        chars[c] ++;
    }

    for (int i = 0; i < 255; i++){
        if (chars[i] > 0) {
            printf("%c - ", i);
            for (int j = 0; j < chars[i]; j++) printf("#");
            printf("\n");
        }
    }

    // Восстанавливаем старые настройки терминала перед выходом
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return 0;
}
