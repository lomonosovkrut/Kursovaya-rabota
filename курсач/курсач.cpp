
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <string.h>

// Объявление функций
void generateRandomMatrix(int*** matrix, int* size, int directed);
void manualInputMatrix(int*** matrix, int* size);
void writeMatrixToFile(int** matrix, int size, const char* filename);
void readMatrixFromFile(int*** matrix, int* size, const char* filename);
int hasEulerianCycle(int** matrix, int size, int directed);
void findEulerianCycle(int** matrix, int size, int directed);
void freeMatrix(int** matrix, int size);
void promptMatrixEdit(int*** matrix, int* size, int directed);

int main() {
    setlocale(LC_ALL, "RUS");
    int** matrix = NULL;
    int directed = 0;
    int size = 0;
    int choice;
    char filename[50];

    do {
        printf("Меню:\n");
        printf("1) Генерация рандомной матрицы G графа\n");
        printf("2) Ручной ввод матрицы G графа\n");
        printf("3) Запись матрицы в файл\n");
        printf("4) Получение данных из файла\n");
        printf("5) Выход\n");
        printf("Введите ваш выбор: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: {
            freeMatrix(matrix, size);
            printf("Выберите тип графа:\n");
            printf("1) Неориентированный граф\n");
            printf("2) Ориентированный граф\n");
            printf("Введите ваш выбор: ");
            int graphType;
            scanf("%d", &graphType);
            directed = (graphType == 2);
            generateRandomMatrix(&matrix, &size, directed);

            if (!hasEulerianCycle(matrix, size, directed)) {
                printf("Граф не содержит Эйлеров цикл. Хотите отредактировать матрицу вручную? (1 - Да, 0 - Нет): ");
                int editChoice;
                scanf("%d", &editChoice);
                if (editChoice == 1) {
                    promptMatrixEdit(&matrix, &size, directed);
                }
            }
            break;
        }
        case 2:
            freeMatrix(matrix, size);
            manualInputMatrix(&matrix, &size);
            break;

        case 3:
            if (matrix) {
                printf("Введите имя файла: ");
                scanf("%s", filename);
                writeMatrixToFile(matrix, size, filename);
            }
            else {
                printf("Матрица не задана!\n");
            }
            break;

        case 4:
            printf("Введите имя файла: ");
            scanf("%s", filename);
            freeMatrix(matrix, size);
            readMatrixFromFile(&matrix, &size, filename);
            if (matrix) {
                printf("Считанная матрица:\n");
                for (int i = 0; i < size; i++) {
                    for (int j = 0; j < size; j++) {
                        printf("%d ", matrix[i][j]);
                    }
                    printf("\n");
                }
                if (hasEulerianCycle(matrix, size, directed)) {
                    printf("Граф содержит Эйлеров цикл. Поиск цикла:\n");
                    findEulerianCycle(matrix, size, directed);
                }
                else {
                    printf("Граф не содержит Эйлеров цикл.\n");
                }
            }
            break;

        case 5:
            printf("Выход из программы.\n");
            break;

        default:
            printf("Неверный выбор, попробуйте снова.\n");
        }

        if ((choice == 1 || choice == 2) && matrix) {
            if (hasEulerianCycle(matrix, size, directed)) {
                printf("Граф содержит Эйлеров цикл. Поиск цикла:\n");
                findEulerianCycle(matrix, size, directed);
            }
            else {
                printf("Граф не содержит Эйлеров цикл.\n");
            }
        }
    } while (choice != 5);

    freeMatrix(matrix, size);
    return 0;
}

// Функция, позволяющая редактировать матрицу вручную
void promptMatrixEdit(int*** matrix, int* size, int directed) {
    printf("Редактирование матрицы вручную.\n");
    printf("Введите новую матрицу размером %d x %d:\n", *size, *size);
    for (int i = 0; i < *size; i++) {
        for (int j = 0; j < *size; j++) {
            scanf("%d", &(*matrix)[i][j]);
        }
    }

    printf("Обновленная матрица:\n");
    for (int i = 0; i < *size; i++) {
        for (int j = 0; j < *size; j++) {
            printf("%d ", (*matrix)[i][j]);
        }
        printf("\n");
    }

    if (hasEulerianCycle(*matrix, *size, directed)) {
        printf("Матрица теперь содержит Эйлеров цикл.\n");
    }
    else {
        printf("Матрица все еще не содержит Эйлеров цикл.\n");
    }
}

void generateRandomMatrix(int*** matrix, int* size, int directed) {
    printf("Введите размер матрицы (количество вершин графа): ");
    scanf("%d", size);

    // Выделяем память под матрицу
    *matrix = (int**)malloc(*size * sizeof(int*));
    for (int i = 0; i < *size; i++) {
        (*matrix)[i] = (int*)calloc(*size, sizeof(int));
    }

    srand((unsigned int)time(NULL));

    // Генерация случайного графа
    if (directed) {
        for (int i = 0; i < *size; i++) {
            for (int j = 0; j < *size; j++) {
                if (i != j && rand() % 2) {
                    // Добавляем ребро только в одном направлении
                    if ((*matrix)[i][j] == 0 && (*matrix)[j][i] == 0) {
                        (*matrix)[i][j] = 1; // Ребро из i в j
                    }
                }
            }
        }
    }
    else {
        // Для неориентированного графа
        for (int i = 0; i < *size; i++) {
            for (int j = i + 1; j < *size; j++) {
                if (rand() % 2) {
                    (*matrix)[i][j] = 1;
                    (*matrix)[j][i] = 1;
                }
            }
        }
    }

    // Вывод матрицы
    printf("Сгенерированная матрица:\n");
    for (int i = 0; i < *size; i++) {
        for (int j = 0; j < *size; j++) {
            printf("%d ", (*matrix)[i][j]);
        }
        printf("\n");
    }
}


// Ввод матрицы вручную
void manualInputMatrix(int*** matrix, int* size) {
    printf("Введите размер матрицы (количество вершин графа): ");
    scanf("%d", size);

    *matrix = (int**)malloc(*size * sizeof(int*));
    for (int i = 0; i < *size; i++) {
        (*matrix)[i] = (int*)malloc(*size * sizeof(int));
    }

    printf("Введите матрицу смежности:\n");
    for (int i = 0; i < *size; i++) {
        for (int j = 0; j < *size; j++) {
            scanf("%d", &(*matrix)[i][j]);
        }
    }
}

void writeMatrixToFile(int** matrix, int size, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Ошибка открытия файла для записи.\n");
        return;
    }

    fprintf(file, "%d\n", size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fprintf(file, "%d ", matrix[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    printf("Матрица записана в файл %s.\n", filename);
}

void readMatrixFromFile(int*** matrix, int* size, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Ошибка открытия файла для чтения.\n");
        return;
    }

    fscanf(file, "%d", size);
    *matrix = (int**)malloc(*size * sizeof(int*));
    for (int i = 0; i < *size; i++) {
        (*matrix)[i] = (int*)malloc(*size * sizeof(int));
    }

    for (int i = 0; i < *size; i++) {
        for (int j = 0; j < *size; j++) {
            fscanf(file, "%d", &(*matrix)[i][j]);
        }
    }
    fclose(file);
    printf("Матрица считана из файла %s.\n", filename);
}

// Функция для обхода графа в глубину (DFS)
void dfs(int v, int* visited, int** matrix, int size) {
    visited[v] = 1;
    for (int i = 0; i < size; i++) {
        if (matrix[v][i] > 0 && !visited[i]) {
            dfs(i, visited, matrix, size);
        }
    }
}

// Проверка наличия Эйлерова цикла
int hasEulerianCycle(int** matrix, int size, int directed) {
    int* visited = (int*)calloc(size, sizeof(int));

    if (directed) {
        // Для ориентированного графа проверяем равенство входящих и исходящих степеней
        int* inDegree = (int*)calloc(size, sizeof(int));
        int* outDegree = (int*)calloc(size, sizeof(int));

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (matrix[i][j] > 0) {
                    outDegree[i]++;
                    inDegree[j]++;
                }
            }
        }

        for (int i = 0; i < size; i++) {
            if (inDegree[i] != outDegree[i]) {
                free(visited);
                free(inDegree);
                free(outDegree);
                return 0; // Неравенство степеней исключает Эйлеров цикл
            }
        }

        free(inDegree);
        free(outDegree);

        // Проверяем связность графа
        int startVertex = -1;
        for (int i = 0; i < size; i++) {
            int degree = 0;
            for (int j = 0; j < size; j++) {
                degree += matrix[i][j];
            }
            if (degree > 0) { // Ищем вершину с ненулевой степенью
                startVertex = i;
                break;
            }
        }

        if (startVertex == -1) {
            free(visited);
            return 1; // Граф пуст, но считается, что Эйлеров цикл есть
        }

        // Проверяем связность
        dfs(startVertex, visited, matrix, size);
        for (int i = 0; i < size; i++) {
            int degree = 0;
            for (int j = 0; j < size; j++) {
                degree += matrix[i][j];
            }
            if (degree > 0 && !visited[i]) {
                free(visited);
                return 0; // Граф не связан
            }
        }

        // Проверяем связность по входящим рёбрам
        memset(visited, 0, size * sizeof(int)); // Сбрасываем массив visited
        int** reverseMatrix = (int**)malloc(size * sizeof(int*));
        for (int i = 0; i < size; i++) {
            reverseMatrix[i] = (int*)calloc(size, sizeof(int));
        }
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (matrix[i][j] > 0) {
                    reverseMatrix[j][i] = 1; // Обратное направление
                }
            }
        }

        dfs(startVertex, visited, reverseMatrix, size);
        for (int i = 0; i < size; i++) {
            int degree = 0;
            for (int j = 0; j < size; j++) {
                degree += matrix[i][j];
            }
            if (degree > 0 && !visited[i]) {
                for (int k = 0; k < size; k++) {
                    free(reverseMatrix[k]);
                }
                free(reverseMatrix);
                free(visited);
                return 0; // Граф не связан в обратном направлении
            }
        }

        for (int k = 0; k < size; k++) {
            free(reverseMatrix[k]);
        }
        free(reverseMatrix);
    }
    else {
        // Для неориентированного графа проверяем чётность степеней всех вершин
        for (int i = 0; i < size; i++) {
            int degree = 0;
            for (int j = 0; j < size; j++) {
                degree += matrix[i][j];
            }
            if (degree % 2 != 0) {
                free(visited);
                return 0; // Нечётная степень исключает Эйлеров цикл
            }
        }

        // Проверяем связность графа
        int startVertex = -1;
        for (int i = 0; i < size; i++) {
            int degree = 0;
            for (int j = 0; j < size; j++) {
                degree += matrix[i][j];
            }
            if (degree > 0) { // Ищем вершину с ненулевой степенью
                startVertex = i;
                break;
            }
        }

        if (startVertex == -1) {
            free(visited);
            return 1; // Граф пуст, но считается, что Эйлеров цикл есть
        }

        dfs(startVertex, visited, matrix, size);
        for (int i = 0; i < size; i++) {
            int degree = 0;
            for (int j = 0; j < size; j++) {
                degree += matrix[i][j];
            }
            if (degree > 0 && !visited[i]) {
                free(visited);
                return 0; // Граф не связан
            }
        }
    }

    free(visited);
    return 1; // Граф связан, и условия для Эйлерова цикла выполнены
}

// Нахождение Эйлерова цикла
void findEulerianCycle(int** matrix, int size, int directed) {
    int* stack = (int*)malloc((size_t)size * (size_t)size * sizeof(int));  // Стек для пути
    int* cycle = (int*)malloc((size_t)size * (size_t)size * sizeof(int));  // Массив для хранения цикла
    int top = -1, cycleIndex = 0, current = 0;

    if (!stack || !cycle) {
        fprintf(stderr, "Ошибка выделения памяти.\n");
        exit(EXIT_FAILURE);
    }

    // Копируем матрицу
    int** tempMatrix = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        tempMatrix[i] = (int*)malloc(size * sizeof(int));
        memcpy(tempMatrix[i], matrix[i], size * sizeof(int));
    }

    stack[++top] = current;
    while (top >= 0) {
        current = stack[top];
        int found = 0;
        for (int i = 0; i < size; i++) {
            if (tempMatrix[current][i] > 0) {
                // Проходим по ребру
                stack[++top] = i;
                if (directed) {
                    tempMatrix[current][i]--;
                }
                else {
                    tempMatrix[current][i]--;
                    tempMatrix[i][current]--;
                }
                found = 1;
                break;
            }
        }
        if (!found) {
            // Если нет соседей, добавляем вершину в цикл
            cycle[cycleIndex++] = current;
            top--;
        }
    }

    // Выводим Эйлеров цикл
    printf("Эйлеров цикл: ");
    for (int i = cycleIndex - 1; i >= 0; i--) {
        printf("%d ", cycle[i]);
    }
    printf("\n");

    // Освобождаем память
    free(stack);
    free(cycle);
    for (int i = 0; i < size; i++) {
        free(tempMatrix[i]);
    }
    free(tempMatrix);
}

// Освобождение памяти
void freeMatrix(int** matrix, int size) {
    if (matrix) {
        for (int i = 0; i < size; i++) {
            free(matrix[i]);
        }
        free(matrix);
    }
}

