/*
Чеклин Павел ИУ7-32Б вариант 3
Обработка разреженных матриц

Разреженная (содержащая много нулей) матрица хранится в форме 3-х объектов:
- вектор A содержит значения ненулевых элементов;
- вектор JA содержит номера столбцов для элементов вектора A;
- связный список IA, в элементе Nk которого находится номер компонент в A и JA, с которых начинается
описание строки Nk матрицы A.

1. Смоделировать операцию умножения матрицы и вектора-столбца,
хранящихся в этой форме, с получением результата в той же форме.
2. Произвести операцию умножения, применяя стандартный алгоритм работы с матрицами.
3. Сравнить время выполнения операций и объем памяти при использовании
этих 2-х алгоритмов при различном проценте заполнения матриц.

При разработке интерфейса программы следует предусмотреть: указание формата и диапазона вводимых данных,
указание операции, производимой программой,
наличие пояснений при выводе результата,
указание формата выводимых данных
возможность заполнения матриц вручную (даже при большой размерности, например, 1000*1000).

При тестировании программы необходимо: проверить правильность ввода
проконтролировать правильность вывода данных (т.е. их соответствие требуемому формату);
проверить правильность выполнения операций;
обеспечить вывод сообщений при отсутствии входных данных («пустой
ввод»);

обеспечить вывод сообщений при нулевых результате или вывод нулевого результата при ненулевом входе;
обеспечить возможность ввода данных и вывода результата как при малых матрицах, так и при больших (например, 1000 * 1000).

сравнить время выполнения стандартного алгоритма обработки матриц и алгоритма обработки
разреженных матриц при различной заполненности матриц (от 1 элемента до 100% заполненности).

сравнить объем требуемой памяти для реализации стандартного алгоритма обработки матриц
и алгоритма обработки разреженных матриц при различном проценте заполнения матриц и при различном их размере.

Следует также протестировать программу при полной загрузке системы, то есть
при полном заполнении матриц. Программа должна адекватно реагировать на
неверный ввод, пустой ввод и выход за границы матрицы или вектора. Необходимо тщательно следить за освобождением динамической памяти при окончании программы.

 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// #include "test.h"
#include "menu.h"
#include "matrix.h"
#include "smatrix.h"

void matrix_test()
{
    matrix m1, m2, res;
    matrix_init(&m1);
    matrix_init(&m2);
    matrix_init(&res);

    puts("m1");
    matrix_input(&m1);
    puts("m2");
    matrix_input(&m2);

    matrix_output(m1);
    matrix_output(m2);

    matrix_product(m1, m2, &res);
    matrix_output(res);

    matrix_delete(&m1);
    matrix_delete(&m2);
    matrix_delete(&res);
}

void smatrix_test()
{
    s_matr m;
    s_matr_init(&m);
    s_matr_input(&m);
    s_matr_output(&m);
    s_matr_delete(&m);
}

int main()
{
    // menu_mainloop();
    // matrix_test();
    smatrix_test();
    return 0;
}
