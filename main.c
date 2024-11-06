#include <stdio.h>
#include <stdlib.h>

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
    if (car == NULL) return 1;                         // Ошибка выделения памяти
    info->number_vehicles ++;                           // Учеличиваем счетчик автомобилей
    (*car)[info->number_vehicles - 1].current_odo = 0;  // Обнуляем одометр нового авто
    (*car)[info->number_vehicles - 1].tire_sets = 0;    // В начале колес нет
    (*car)[info->number_vehicles - 1].tire == NULL;     // Обнуляем структуру шин
    return 0;
}

int add_tire (struct Car *car){

}

int save_to_file (struct Car *car, int numcars){
    FILE *file = fopen ("tires.dat", "wb");
    if (file == NULL) return 1; // Ошибка открытия файла

    fwrite (&numcars, sizeof(int), 1, file); // Запись количества автомобилей

    for (int i = 0; i < numcars; i++){ // Цикл от количества автомобилй
        fwrite (&car[i], sizeof(struct Car), 1, file); // Данные автомобиля
        for( int j = 0; j < car[i].tire_sets; j ++){ 
            fwrite(&car[i].tire[j], sizeof (struct Tire), 1, file); // Записываем каждый комплект шин
            fwrite(&car[i].tire[j].odo, sizeof (struct Odo), car[i].tire[j].odo_count, file); // Записываем интервалы пробега

        }
    }
    fclose(file);
    return 0;
}

int load_from_file (struct Car **car, struct Info *info){
    FILE *file = fopen("tires.dat", "rb");
    if (file == NULL) return 1; // Ошибка открытия файла

    fread (&info->number_vehicles, sizeof(int), 1, file);

    // Выделяем память под автомобили
    *car = (struct Car *)malloc (info->number_vehicles * sizeof (struct Car));
    if (*car == NULL){
        fclose(file);
        return 3; // Ошибка выделения памяти автомобилям
    }

    for (int i = 0; i < info->number_vehicles; i++){
        fread (&(*car)[i], sizeof(struct Car), 1, file);
    
        (*car)[i].tire = (struct Tire *)malloc((*car)[i].tire_sets * sizeof(struct Tire));
        if ((*car)[i].tire == NULL) {
            fclose(file);
            return 3; // Ошибка выделения памяти комплектам шин
        }

        for (int j = 0; j < (*car)[i].tire_sets; j++) {
            // Считываем данные о комплекте шин
            fread(&(*car)[i].tire[j], sizeof(struct Tire), 1, file);

            // Выделяем память для массива интервалов эксплуатации
            (*car)[i].tire[j].odo = (struct Odo *)malloc((*car)[i].tire[j].odo_count * sizeof(struct Odo));
            if ((*car)[i].tire[j].odo == NULL) {
                fclose(file);
                return 3; // Ошибка выделения памяти комплектам шин
            }
            // Считываем интервалы эксплуатации
            fread((*car)[i].tire[j].odo, sizeof(struct Odo), (*car)[i].tire[j].odo_count, file);
        }   
    }
    fclose(file);
    return 0;
}

int main(void){

    struct Info base_info;
    struct Car *car = NULL;

    first_car(&base_info, &car);

    save_to_file(car, base_info.number_vehicles);

    free(car);
    return 0;
}