#include <stdio.h>

// Определяю булевый тип, отсутствующий в Си

typedef enum {false, true} bool; 

// Структура хранения данных автомобиля

// Структура хранения данных о колесах

struct Tire {
    char name[100]; // Информация о модели и размерности шины
    bool repair;    // Информация о наличии ремонтов шин
    int odo_count;
    struct Odo *odo;
};

// Структура хранения интервалов эксплуатации шин

struct Odo {
    int start_interval; // Начало и конец пробега
    int end_interval;
};

