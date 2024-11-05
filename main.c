#include <stdio.h>

// Определяю булевый тип, отсутствующий в Си

typedef enum {false, true} bool; 

// Служебная структура

struct Info {
    int number_vehicles; // Количество автомобилей в базе
};

// Структура хранения данных автомобиля

struct Car {
    char name[100];     // Информация о модели авто
    int current_odo;    // Текущее значение одометра
    int tire_sets;      // Количество комплектов шин у этого автомобиля
    struct Tire *tire;  // Структура пробегов комплектов шин 
};

// Структура хранения данных о комплектах шин

struct Tire {
    char name[100];     // Информация о модели и размерности шины
    bool repair;        // Информация о наличии ремонтов
    int odo_count;      // Количество записей о пробеге комплекта шин
    struct Odo *odo;    // Структура интевалов эксплуатации
};

// Структура хранения интервалов эксплуатации шин

struct Odo {
    int start_interval; // Начало и конец пробега
    int end_interval;
};

// Функция добавления первого автомобиля

int first_car (struct Info *info, struct Car **car){
    *car = malloc (sizeof (struct Car)); // Выделяем память на первый авто
    if (*car == NULL) return 1;          // Ошибка выделения памяти
    (*car)->current_odo = 0;             // Обнуляем одометр
    info->number_vehicles = 1;           // Инициализируем количество авто, инфо для записи в файл
    (*car)->tire_sets = 0;               // В начале колес нет
    (*car)->tire = NULL;                 // Обнуляем стукруру шин
    return 0;
}

// Функция добавления следующего автомобиля

int add_car (struct Info *info, struct Car **car){
    *car = realloc (*car, (info->number_vehicles + 1) * sizeof (struct Car));
    if (*car == NULL) return 1;                         // Ошибка выделения памяти
    info->number_vehicles ++;                           // Учеличиваем счетчик автомобилей
    (*car)[info->number_vehicles - 1].current_odo = 0;  // Обнуляем одометр нового авто
    (*car)[info->number_vehicles - 1].tire_sets = 0;    // В начале колес нет
    (*car)[info->number_vehicles - 1].tire == NULL;     // Обнуляем структуру шин
    return 0;
}

int add_tire (struct Car *car){

}

int main(void){
    struct Info base_info = {0};
    struct Car *cars = NULL;

    
    free(cars);
    return 0;
}