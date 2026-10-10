import random

#task1

def to_number(s):
    x = float(s)
    return int(x) if x.is_integer() else x


def read_positive_int(prompt):
    while True:
        try:
            value = int(input(prompt))
        except ValueError:
            print("Ошибка: нужно ввести целое число.")
            continue
        if value <= 0:
            print("Ошибка: число должно быть больше 0.")
            continue
        return value


def input_matrix(rows, cols):
    matrix = [[0 for _ in range(cols)] for _ in range(rows)]

    for i in range(rows):
        while True:
            parts = input(f"Введите строку {i + 1}: ").split()
            if len(parts) != cols:
                print(f"Ошибка: в строке должно быть ровно {cols} элемент(а/ов).")
                continue
            try:
                row = [to_number(x) for x in parts]
            except ValueError:
                print("Ошибка: все элементы должны быть числами.")
                continue
            break

        for j in range(cols):
            matrix[i][j] = row[j]

    return matrix


def print_matrix(matrix):
    for row in matrix:
        print(" ".join(f"{x:>6}" for x in row))


#task2

def random_matrix(rows, cols):
    matrix = []
    for i in range(rows):
        row = []
        for j in range(cols):
            row.append(random.randint(1, 100))
        matrix.append(row)
    return matrix

def add_matrices(a, b):
    rows, cols = len(a), len(a[0])
    c = [[0 for _ in range(cols)] for _ in range(rows)]
    for i in range(rows):
        for j in range(cols):
            c[i][j] = a[i][j] + b[i][j]
    return c


def subtract_matrices(a, b):
    rows, cols = len(a), len(a[0])
    d = [[0 for _ in range(cols)] for _ in range(rows)]
    for i in range(rows):
        for j in range(cols):
            d[i][j] = a[i][j] - b[i][j]
    return d

#task3

def read_number(prompt):
    while True:
        try:
            return to_number(input(prompt))
        except ValueError:
            print("Ошибка: нужно ввести число.")


def multiply_by_scalar(matrix, scalar):
    rows, cols = len(matrix), len(matrix[0])
    result = [[0 for _ in range(cols)] for _ in range(rows)]
    for i in range(rows):
        for j in range(cols):
            result[i][j] = scalar * matrix[i][j]
    return result

#task4

def transpose(matrix):
    rows, cols = len(matrix), len(matrix[0])
    result = [[0 for _ in range(rows)] for _ in range(cols)]
    for i in range(rows):
        for j in range(cols):
            result[j][i] = matrix[i][j]
    return result

#task5

def multiply_matrices(a, b):
    if len(a[0]) != len(b):
        return None
    m, n, k = len(a), len(a[0]), len(b[0])
    c = [[0 for _ in range(k)] for _ in range(m)]
    for i in range(m):
        for j in range(k):
            s = 0
            for t in range(n):
                s += a[i][t] * b[t][j]
            c[i][j] = s
    return c

#task6

def is_square(matrix):
    return len(matrix) == len(matrix[0])


def main_diagonal(matrix):
    if not is_square(matrix):
        return None
    n = len(matrix)
    result = []
    for i in range(n):
        result.append(matrix[i][i])
    return result


def secondary_diagonal(matrix):
    if not is_square(matrix):
        return None
    n = len(matrix)
    result = []
    for i in range(n):
        result.append(matrix[i][n - 1 - i])
    return result


def trace(matrix):
    if not is_square(matrix):
        return None
    s = 0
    for i in range(len(matrix)):
        s += matrix[i][i]
    return s

#task7

def determinant(matrix):
    if not is_square(matrix):
        return None
    n = len(matrix)
    if n == 1:
        return matrix[0][0]
    if n == 2:
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]
    det = 0
    for j in range(n):
        det += matrix[0][j] * cofactor(matrix, 0, j)
    return det

#task8

def get_minor(matrix, row, col):
    n = len(matrix)
    if not (0 <= row < n and 0 <= col < n):
        return None
    minor = []
    for i in range(n):
        if i == row:
            continue
        new_row = []
        for j in range(n):
            if j == col:
                continue
            new_row.append(matrix[i][j])
        minor.append(new_row)
    return minor


def cofactor(matrix, row, col):
    if len(matrix) == 1:
        return 1
    minor = get_minor(matrix, row, col)
    if minor is None:
        return None
    return (-1) ** (row + col) * determinant(minor)

#task9

def inverse_matrix(matrix, eps=1e-12):
    if not is_square(matrix):
        return None
    n = len(matrix)
    e = identity_matrix(n)

    aug = []
    for i in range(n):
        row = []
        for j in range(n):
            row.append(float(matrix[i][j]))
        for j in range(n):
            row.append(float(e[i][j]))
        aug.append(row)

    for col in range(n):
        pivot_row = col
        for i in range(col + 1, n):
            if abs(aug[i][col]) > abs(aug[pivot_row][col]):
                pivot_row = i
        if abs(aug[pivot_row][col]) < eps:
            return None
        aug[col], aug[pivot_row] = aug[pivot_row], aug[col]

        pivot = aug[col][col]
        for j in range(2 * n):
            aug[col][j] /= pivot

        for i in range(n):
            if i != col:
                factor = aug[i][col]
                for j in range(2 * n):
                    aug[i][j] -= factor * aug[col][j]

    inverse = []
    for i in range(n):
        inverse.append(aug[i][n:])
    return inverse


def round_matrix(matrix, digits=3):
    result = []
    for row in matrix:
        new_row = []
        for x in row:
            new_row.append(round(x, digits) + 0.0)
        result.append(new_row)
    return result

#task10

def identity_matrix(n):
    e = [[0 for _ in range(n)] for _ in range(n)]
    for i in range(n):
        e[i][i] = 1
    return e


def is_identity(matrix, eps=1e-9):
    if not is_square(matrix):
        return False
    n = len(matrix)
    for i in range(n):
        for j in range(n):
            expected = 1 if i == j else 0
            if abs(matrix[i][j] - expected) >= eps:
                return False
    return True

#task11

def is_valid_matrix(matrix):
    if len(matrix) == 0 or len(matrix[0]) == 0:
        return False
    for row in matrix:
        if len(row) != len(matrix[0]):
            return False
    return True


def matrix_stats(matrix):
    if not is_valid_matrix(matrix):
        return None
    rows, cols = len(matrix), len(matrix[0])

    all_elements = []
    for row in matrix:
        for x in row:
            all_elements.append(x)

    row_sums = []
    for row in matrix:
        row_sums.append(sum(row))

    col_sums = []
    for j in range(cols):
        s = 0
        for i in range(rows):
            s += matrix[i][j]
        col_sums.append(s)

    total = sum(all_elements)
    return {
        "rows": rows,
        "cols": cols,
        "min": min(all_elements),
        "max": max(all_elements),
        "sum": total,
        "avg": total / len(all_elements),
        "row_sums": row_sums,
        "col_sums": col_sums,
    }


def print_stats(stats):
    print(f"Размер: {stats['rows']} × {stats['cols']}")
    print(f"Минимальный элемент: {stats['min']}")
    print(f"Максимальный элемент: {stats['max']}")
    print(f"Сумма элементов: {stats['sum']}")
    print(f"Среднее значение: {stats['avg']:.2f}")
    print("Суммы строк:", ", ".join(str(s) for s in stats["row_sums"]))
    print("Суммы столбцов:", ", ".join(str(s) for s in stats["col_sums"]))

#task12

def ask_matrix(name):
    print(f"Матрица {name}")
    rows = read_positive_int("Количество строк: ")
    cols = read_positive_int("Количество столбцов: ")
    while True:
        mode = input("Заполнить: 1 - вручную, 2 - случайно: ").strip()
        if mode == "1":
            return input_matrix(rows, cols)
        if mode == "2":
            matrix = random_matrix(rows, cols)
            print_matrix(matrix)
            return matrix
        print("Ошибка: введите 1 или 2.")


def need_matrix(matrix):
    if matrix is None:
        print("Сначала введите матрицу (пункт 1).")
        return False
    return True


def menu_add(a):
    b = ask_matrix("B")
    if len(a) != len(b) or len(a[0]) != len(b[0]):
        print("Ошибка: размеры матриц не совпадают, сложение невозможно.")
        return
    print("A + B:")
    print_matrix(add_matrices(a, b))


def menu_subtract(a):
    b = ask_matrix("B")
    if len(a) != len(b) or len(a[0]) != len(b[0]):
        print("Ошибка: размеры матриц не совпадают, вычитание невозможно.")
        return
    print("A - B:")
    print_matrix(subtract_matrices(a, b))


def menu_scalar(a):
    k = read_number("Введите число k: ")
    print(f"A × {k}:")
    print_matrix(multiply_by_scalar(a, k))


def menu_transpose(a):
    print("A^T:")
    print_matrix(transpose(a))


def menu_multiply(a):
    b = ask_matrix("B")
    c = multiply_matrices(a, b)
    if c is None:
        print("Ошибка: число столбцов A не равно числу строк B, умножение невозможно.")
        return
    print("A × B:")
    print_matrix(c)


def menu_determinant(a):
    if not is_square(a):
        print("Ошибка: определитель есть только у квадратной матрицы.")
        return
    print("Определитель:", determinant(a))


def menu_inverse(a):
    if not is_square(a):
        print("Ошибка: обратная матрица есть только у квадратной матрицы.")
        return
    inverse = inverse_matrix(a)
    if inverse is None:
        print("Обратной матрицы не существует: матрица вырожденная (определитель равен 0).")
        return
    print("A^(-1):")
    print_matrix(round_matrix(inverse))
    if is_identity(multiply_matrices(a, inverse)):
        print("Проверка пройдена: A × A^(-1) = E")
    else:
        print("Проверка не пройдена: результат не единичная матрица")


def menu_stats(a):
    print_stats(matrix_stats(a))


def print_menu():
    print()
    print("МАТРИЧНЫЙ КАЛЬКУЛЯТОР")
    print("1. Ввести матрицу")
    print("2. Вывести матрицу")
    print("3. Сложить две матрицы")
    print("4. Вычесть две матрицы")
    print("5. Умножить матрицу на число")
    print("6. Транспонировать матрицу")
    print("7. Умножить две матрицы")
    print("8. Найти определитель")
    print("9. Найти обратную матрицу")
    print("10. Вывести характеристики матрицы")
    print("0. Завершить программу")


def main():
    matrix = None
    actions = {
        "3": menu_add,
        "4": menu_subtract,
        "5": menu_scalar,
        "6": menu_transpose,
        "7": menu_multiply,
        "8": menu_determinant,
        "9": menu_inverse,
        "10": menu_stats,
    }
    while True:
        print_menu()
        choice = input("Выберите пункт: ").strip()
        if choice == "0":
            print("Программа завершена.")
            break
        elif choice == "1":
            matrix = ask_matrix("A")
        elif choice == "2":
            if need_matrix(matrix):
                print("Матрица A:")
                print_matrix(matrix)
        elif choice in actions:
            if need_matrix(matrix):
                actions[choice](matrix)
        else:
            print("Ошибка: такого пункта нет.")


if __name__ == "__main__":
    main()






