#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#ifdef _WIN32
#include <windows.h>
#endif


#define MIN_COUNT 2
#define MAX_COUNT 1000


static void setup_console(void)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}


int *find_min(int *begin, int *end)
{
    int *minimum;

    if (begin >= end) {
        return NULL;
    }

    minimum = begin;
    for (int *current = begin + 1; current < end; ++current) {
        if (*current < *minimum) {
            minimum = current;
        }
    }
    return minimum;
}

int *find_max(int *begin, int *end)
{
    int *maximum;

    if (begin >= end) {
        return NULL;
    }

    maximum = begin;
    for (int *current = begin + 1; current < end; ++current) {
        if (*current > *maximum) {
            maximum = current;
        }
    }
    return maximum;
}

long long sum_between(const int *first, const int *second)
{
    const int *left = first;
    const int *right = second;
    long long sum = 0;

    if (left > right) {
        const int *temporary = left;
        left = right;
        right = temporary;
    }

    for (const int *current = left + 1; current < right; ++current) {
        sum += *current;
    }
    return sum;
}

size_t find_longest_increasing(int *begin, int *end, int **sequence_begin)
{
    int *best_begin;
    int *run_begin;
    size_t best_length;

    if (begin >= end) {
        if (sequence_begin != NULL) {
            *sequence_begin = NULL;
        }
        return 1;
    }

    best_begin = begin;
    run_begin = begin;
    best_length = 1;

    for (int *current = begin + 1; current < end; ++current) {
        if (*current > *(current - 1)) {
            size_t run_length = (size_t)(current - run_begin) + 1;

            if (run_length > best_length) {
                best_length = run_length;
                best_begin = run_begin;
            }
        } else {
            run_begin = current;
        }
    }

    if (sequence_begin != NULL) {
        *sequence_begin = best_begin;
    }
    return best_length;
}

void reverse_range(int *begin, int *end)
{
    int *left;
    int *right;

    if (begin >= end) {
        return;
    }

    left = begin;
    right = end - 1;

    while (left < right) {
        int temporary = *left;

        *left = *right;
        *right = temporary;
        ++left;
        --right;
    }
}

void print_array(const int *begin, const int *end)
{
    for (const int *current = begin; current < end; ++current) {
        if (current != begin) {
            putchar(' ');
        }
        printf("%d", *current);
    }
    putchar('\n');
}

static int read_count(size_t *count)
{
    long value;

    if (scanf("%ld", &value) != 1) {
        printf("Ошибка: не удалось прочитать количество элементов.\n");
        return 0;
    }
    if (value < MIN_COUNT || value > MAX_COUNT) {
        printf("Ошибка: N должно быть в диапазоне %d..%d.\n", MIN_COUNT, MAX_COUNT);
        return 0;
    }
    *count = (size_t)value;
    return 1;
}

static int read_array(int *begin, int *end)
{
    for (int *current = begin; current < end; ++current) {
        if (scanf("%d", current) != 1) {
            printf("Ошибка: не удалось прочитать элемент номер %td.\n",
                    current - begin);
            return 0;
        }
    }
    return 1;
}

static size_t distance_between(const int *first, const int *second)
{
    if (first > second) {
        return (size_t)(first - second);
    }
    return (size_t)(second - first);
}

static void analyze_array(int *begin, int *end)
{
    int *minimum = find_min(begin, end);
    int *maximum = find_max(begin, end);
    int *sequence_begin = NULL;
    size_t sequence_length;
    long long inner_sum;

    printf("Исходный массив:\n");
    print_array(begin, end);
    putchar('\n');

    inner_sum = sum_between(minimum, maximum);

    printf("Минимум: %d\n", *minimum);
    printf("Позиция минимума: %td\n", minimum - begin);
    printf("Максимум: %d\n", *maximum);
    printf("Позиция максимума: %td\n", maximum - begin);
    printf("Расстояние между минимумом и максимумом: %zu\n",
           distance_between(minimum, maximum));
    printf("Сумма элементов между минимумом и максимумом: %lld\n", inner_sum);
    putchar('\n');

    sequence_length = find_longest_increasing(begin, end, &sequence_begin);

    printf("Самый длинный возрастающий участок:\n");
    print_array(sequence_begin, sequence_begin + sequence_length);
    printf("Длина: %zu\n", sequence_length);
    printf("Позиция начала участка: %td\n", sequence_begin - begin);
    putchar('\n');

    reverse_range(sequence_begin, sequence_begin + sequence_length);

    printf("Массив после разворота выбранного участка:\n");
    print_array(begin, end);
}

int main(void)
{
    static int values[MAX_COUNT];
    size_t count;

    setup_console();

    printf("Введите количество элементов N (%d..%d): ", MIN_COUNT, MAX_COUNT);
    if (!read_count(&count)) {
        return EXIT_FAILURE;
    }

    printf("Введите %zu целых чисел: ", count);
    if (!read_array(values, values + count)) {
        return EXIT_FAILURE;
    }
    putchar('\n');

    analyze_array(values, values + count);

    return EXIT_SUCCESS;
}
