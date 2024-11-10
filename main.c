#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {false, true} bool; // Определяю булевый тип

struct Info {               // Служебная структура
    int number_vehicles;    // Количество автомобилей в базе
    int current_car;        // Текущий автомобиль в программе
};

struct Car {            // Структура хранения данных автомобиля
    char name[100];     // Информация о модели авто
    int current_odo;    // Текущее значение одометра
    int current_tire;   // Установленный комплект колес
    int tire_sets;      // Количество комплектов шин у этого автомобиля
    struct Tire *tire;  // Структура пробегов комплектов шин 
};

struct Tire {           // Структура хранения данных о комплектах шин
    char name[100];     // Информация о модели и размерности шины
    bool repair;        // Информация о наличии ремонтов
    int odo_count;      // Количество записей о пробеге комплекта шин
    struct Odo *odo;    // Структура интевалов эксплуатации
};

struct Odo {            // Структура хранения интервалов эксплуатации шин
    int start_interval; // Начало и конец пробега
    int end_interval;
};

// Функция освобождения выделенной памяти при выходе

void free_memory(struct Car *car_array, int car_count) {
    
    if (car_count == 0) return 0; // Если машины не добавлены, освобождать нечего

    for (int i = 0; i < car_count; i++) {
        struct Car *car = &car_array[i];
        
        // Освобождаем память для каждого комплекта шин у автомобиля
        for (int j = 0; j < car->tire_sets; j++) {
            struct Tire *tire = &car->tire[j];
            
            // Освобождаем память для каждого интервала эксплуатации шин (Odo)
            if (tire->odo != NULL) {
                free(tire->odo);
                tire->odo = NULL; // Обнуляем указатель после освобождения
            }
        }
        
        // Освобождаем память для массива шин, если он был выделен
        if (car->tire != NULL) {
            free(car->tire);
            car->tire = NULL; // Обнуляем указатель после освобождения
        }
    }

    // Освобождаем память для массива автомобилей
    free(car_array);
    car_array = NULL; // Обнуляем указатель после освобождения
}


// Функция добавления автомобиля

int add_car (struct Info *info, struct Car **car){

    if (info->number_vehicles == 0){
        *car = malloc (sizeof (struct Car)); // Выделяем память
        if (*car == NULL) return 1;          // Ошибка выделения памяти
        info->number_vehicles = 1;           // Инициализируем количество авто
    } else {
        *car = realloc (*car, (info->number_vehicles + 1) * sizeof (struct Car));
        if (*car == NULL) return 1;           // Ошибка выделения памяти
        info->number_vehicles ++;            // Увеличиваем счетчик автомобилей        
    }

    int new_vehicle = info->number_vehicles -1;

    (*car)[new_vehicle].tire_sets = 0;               // В начале колес нет
    (*car)[new_vehicle].tire = NULL;                 // Обнуляем стукруру указателей шин

    printf("Введите марку и модель автомобиля:");
    fgets((*car)[new_vehicle].name, 100, stdin);
    if ((strchr((*car)[new_vehicle].name, '\n')) != NULL) *strchr((*car)[new_vehicle].name, '\n') = '\0';

    printf("Введите текущий пробег:");
    scanf("%d" ,&(*car)[new_vehicle].current_odo);
    while (getchar() != '\n'); // Удаляем оставшийся \n в потоке ввода  

    return 0;
}

// Функция вывода данных автомобиля

void print_car(struct Info *info, struct Car *car){ // Вывод данных автомобилей
    system("clear");
    printf("%s\n\n", car->name);
    printf("Показание одометра: %d\n\n", car->current_odo);
    printf("Всего комплектов колес: %d\n", car->tire_sets);
    if (car->tire_sets > 0){
        printf("Сейчас установлены: %s (%d комплект)\n\n", car->tire[car->current_tire].name, car->current_tire + 1);
        if (car->tire_sets > 1){
            for (int i = 0; i < car->tire_sets; ++i){
                printf("Комплект номер %d: %s.\n", i+1, car->tire[i].name);
            }
        }
    }
}

// Функция добавления следующего автомобиля (удалить)

int add_car_old (struct Info *info, struct Car **car){
    *car = realloc (*car, (info->number_vehicles + 1) * sizeof (struct Car));
    if (car == NULL) return 1;                         // Ошибка выделения памяти
    info->number_vehicles ++;                           // Учеличиваем счетчик автомобилей
    (*car)[info->number_vehicles - 1].current_odo = 0;  // Обнуляем одометр нового авто
    (*car)[info->number_vehicles - 1].tire_sets = 0;    // В начале колес нет
    (*car)[info->number_vehicles - 1].tire == NULL;     // Обнуляем структуру шин
    return 0;
}

int add_tire (struct Car *car, int current_car){

    if (car[current_car].tire_sets == 0){ // Если еще нет ни одного комплекта шин, выделяем новый
        car[current_car].tire = (struct Tire *) malloc(sizeof(struct Tire));
        if (car[current_car].tire == NULL) return 1; // Ошибка выделения памяти комплектам шин
    } else {
        // Если уже были записи о шинах, добавляем
        car[current_car].tire = (struct Tire *) realloc(car[current_car].tire, (car[current_car].tire_sets + 1) * sizeof (struct Tire));
        if (car[current_car].tire == NULL) return 1; // Проверка на ошибка выделения памяти комплектам шин       
    }

    car[current_car].tire_sets ++;
    int new_tire = car[current_car].tire_sets - 1; // Упрощаем указатель на новый комплект колес
    
    car[current_car].tire[new_tire].odo = NULL;
    car[current_car].tire[new_tire].odo_count = 0;
    car[current_car].tire[new_tire].repair = false;

    printf("Введите марку и модель шин:");
    fgets(car[current_car].tire[new_tire].name, 100, stdin);
    if ((strchr(car[current_car].tire[new_tire].name, '\n')) != NULL) *strchr(car[current_car].tire[new_tire].name, '\n') = '\0';

    printf("Введите текущий пробег шин, если они не новые:");
    scanf("%d" ,&car[current_car].tire[new_tire].odo_count);
    while (getchar() != '\n'); // Удаляем оставшийся \n в потоке ввода 
    
    return 0;
}

int save_to_file (struct Car *car, struct Info *info){
    FILE *file = fopen ("tires.dat", "wb");
    if (file == NULL) return 1; // Ошибка открытия файла

    fwrite (&info, sizeof(struct Info), 1, file); // Запись количества автомобилей

    for (int i = 0; i < info->number_vehicles; i++){ // Цикл от количества автомобилй
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

    fread (&info, sizeof(struct Info), 1, file);

    // Выделяем память под автомобили
    *car = (struct Car *)malloc (info->number_vehicles * sizeof (struct Car));
    if (*car == NULL){
        fclose(file);
        return 2; // Ошибка выделения памяти автомобилям
    }

    for (int i = 0; i < info->number_vehicles; i++){
        fread (&(*car)[i], sizeof(struct Car), 1, file);
    
        // Выделяем память под шины
        (*car)[i].tire = (struct Tire *)malloc((*car)[i].tire_sets * sizeof(struct Tire));
        if ((*car)[i].tire == NULL) {
            fclose(file);
            return 2; // Ошибка выделения памяти комплектам шин
        }

        for (int j = 0; j < (*car)[i].tire_sets; j++) {
            // Считываем данные о комплекте шин
            fread(&(*car)[i].tire[j], sizeof(struct Tire), 1, file);

            // Выделяем память для массива интервалов эксплуатации
            (*car)[i].tire[j].odo = (struct Odo *)malloc((*car)[i].tire[j].odo_count * sizeof(struct Odo));
            if ((*car)[i].tire[j].odo == NULL) {
                fclose(file);
                return 2; // Ошибка выделения памяти комплектам шин
            }
            // Считываем интервалы эксплуатации
            fread((*car)[i].tire[j].odo, sizeof(struct Odo), (*car)[i].tire[j].odo_count, file);
        }   
    }
    fclose(file);
    return 0;
}

int main(void){

    struct Info info;
    struct Car *car = NULL;
    int flag = 0;

    system("clear");
    flag = load_from_file(&car, &info);

    if (flag == 1) {
        printf ("Файла с данными нет, начинаем новый автопроект\n\n");    
        add_car(&info, &car);
    } else if (flag == 2) {
        printf ("Ошибка выделения памяти. Программа остановлена");
        return 1;
    }

//    save_to_file(car, info.number_vehicles);

    free(car);

    return 0;
}