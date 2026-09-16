#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#ifdef _WIN32
#include <windows.h>
#endif


#define MIN_SIZE 2
#define MAX_SIZE 50


static void setup_console(void)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}


double row_average(size_t cols, const int *row)
{
    const int *end = row + cols;
    long long sum = 0;

    for (const int *current = row; current < end; ++current) {
        sum += *current;
    }
    return (double)sum / (double)cols;
}

void fill_row_with_average(size_t cols, int *row)
{
    int average = (int)row_average(cols, row);
    int *end = row + cols;

    for (int *current = row; current < end; ++current) {
        *current = average;
    }
}

static int value_width(size_t rows, size_t cols, const int matrix[rows][cols])
{
    int width = 1;

    for (size_t i = 0; i < rows; ++i) {
        const int *row = *(matrix + i);
        const int *end = row + cols;

        for (const int *current = row; current < end; ++current) {
            int digits = 1;
            int value = *current;

            if (value < 0) {
                ++digits;
            }
            while (value / 10 != 0) {
                value /= 10;
                ++digits;
            }
            if (digits > width) {
                width = digits;
            }
        }
    }
    return width;
}

void print_matrix(size_t rows, size_t cols, const int matrix[rows][cols])
{
    int width = value_width(rows, cols, matrix);

    for (size_t i = 0; i < rows; ++i) {
        const int *row = *(matrix + i);
        const int *end = row + cols;

        for (const int *current = row; current < end; ++current) {
            if (current != row) {
                putchar(' ');
            }
            printf("%*d", width, *current);
        }
        putchar('\n');
    }
}

const int *find_matrix_max(size_t rows, size_t cols, const int matrix[rows][cols],
                           size_t *max_row, size_t *max_col)
{
    const int *max_pointer = *(matrix + 0);

    if (max_row != NULL) {
        *max_row = 0;
    }
    if (max_col != NULL) {
        *max_col = 0;
    }

    for (size_t i = 0; i < rows; ++i) {
        const int *row = *(matrix + i);
        const int *end = row + cols;

        for (const int *p = row; p < end; ++p) {
            if (*p > *max_pointer) {
                max_pointer = p;
                if (max_row != NULL) {
                    *max_row = i;
                }
                if (max_col != NULL) {
                    *max_col = (size_t)(p - row);
                }
            }
        }
    }
    return max_pointer;
}

double matrix_average(size_t rows, size_t cols, const int matrix[rows][cols])
{
    long long sum = 0;

    for (size_t i = 0; i < rows; ++i) {
        const int *row = *(matrix + i);
        const int *end = row + cols;

        for (const int *current = row; current < end; ++current) {
            sum += *current;
        }
    }
    return (double)sum / (double)(rows * cols);
}

size_t count_above_average(size_t rows, size_t cols, const int matrix[rows][cols],
                           double average)
{
    size_t count = 0;

    for (size_t i = 0; i < rows; ++i) {
        const int *row = *(matrix + i);
        const int *end = row + cols;

        for (const int *current = row; current < end; ++current) {
            if ((double)*current > average) {
                ++count;
            }
        }
    }
    return count;
}

size_t hottest_row(size_t rows, size_t cols, const int matrix[rows][cols])
{
    size_t best_row = 0;
    double best_average = row_average(cols, *(matrix + 0));

    for (size_t i = 1; i < rows; ++i) {
        double average = row_average(cols, *(matrix + i));

        if (average > best_average) {
            best_average = average;
            best_row = i;
        }
    }
    return best_row;
}

int max_neighbor_difference(size_t rows, size_t cols, const int matrix[rows][cols])
{
    int max_difference = 0;

    for (size_t i = 0; i < rows; ++i) {
        const int *row = *(matrix + i);
        const int *end = row + cols;

        for (const int *current = row + 1; current < end; ++current) {
            int difference = *current - *(current - 1);

            if (difference < 0) {
                difference = -difference;
            }
            if (difference > max_difference) {
                max_difference = difference;
            }
        }
    }
    return max_difference;
}

static int read_dimension(const char *name, size_t *dimension)
{
    long value;

    if (scanf("%ld", &value) != 1) {
        printf("Ошибка: не удалось прочитать значение %s.\n", name);
        return 0;
    }
    if (value < MIN_SIZE || value > MAX_SIZE) {
        printf("Ошибка: %s должно быть в диапазоне %d..%d.\n", name, MIN_SIZE, MAX_SIZE);
        return 0;
    }
    *dimension = (size_t)value;
    return 1;
}

static int read_matrix(size_t rows, size_t cols, int matrix[rows][cols])
{
    for (size_t i = 0; i < rows; ++i) {
        int *row = *(matrix + i);
        int *end = row + cols;

        for (int *current = row; current < end; ++current) {
            if (scanf("%d", current) != 1) {
                printf("Ошибка: не удалось прочитать элемент [%zu][%td].\n", i, current - row);
                return 0;
            }
        }
    }
    return 1;
}


static void analyze_matrix(size_t rows, size_t cols, int matrix[rows][cols])
{
    const int (*read_only)[cols] = (const int (*)[cols])matrix;
    const int *max_pointer;
    size_t max_row = 0;
    size_t max_col = 0;
    double average;
    size_t above_average;
    size_t hot_row;
    double hot_row_average;
    int difference;

    printf("Исходная карта:\n");
    print_matrix(rows, cols, read_only);
    putchar('\n');

    max_pointer = find_matrix_max(rows, cols, read_only, &max_row, &max_col);
    average = matrix_average(rows, cols, read_only);
    above_average = count_above_average(rows, cols, read_only, average);

    printf("Максимальная температура: %d\n", *max_pointer);
    printf("Строка: %zu\n", max_row);
    printf("Столбец: %zu\n", max_col);
    printf("Средняя температура карты: %.2f\n", average);
    printf("Количество значений выше среднего: %zu\n", above_average);

    hot_row = hottest_row(rows, cols, read_only);
    hot_row_average = row_average(cols, *(read_only + hot_row));

    printf("Строка с максимальным средним: %zu\n", hot_row);
    printf("Среднее выбранной строки: %.2f\n", hot_row_average);
    printf("Значение после приведения к int: %d\n", (int)hot_row_average);
    putchar('\n');

    fill_row_with_average(cols, *(matrix + hot_row));

    printf("Карта после замены выбранной строки:\n");
    print_matrix(rows, cols, read_only);
    putchar('\n');

    difference = max_neighbor_difference(rows, cols, read_only);
    printf("Максимальная разница соседних температур: %d\n", difference);
}

int main(void)
{
    setup_console();
    size_t rows;
    size_t cols;

    printf("Введите количество строк R (%d..%d): ", MIN_SIZE, MAX_SIZE);
    if (!read_dimension("R", &rows)) {
        return EXIT_FAILURE;
    }

    printf("Введите количество столбцов C (%d..%d): ", MIN_SIZE, MAX_SIZE);
    if (!read_dimension("C", &cols)) {
        return EXIT_FAILURE;
    }

    {
        int matrix[rows][cols];

        printf("Введите %zu целых значений температуры: ", rows * cols);
        if (!read_matrix(rows, cols, matrix)) {
            return EXIT_FAILURE;
        }
        putchar('\n');

        analyze_matrix(rows, cols, matrix);
    }

    return EXIT_SUCCESS;
}
