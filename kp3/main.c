#include <stdio.h>
#include <float.h>
#include <math.h>

double epsilon; // глобальная переменная под машиное эпсилон

void setMachineEpsilon() // получение машинного эпсилон делением 1 на 2 до упора
{
    epsilon = 1.0;
    while (1.0 + epsilon / 2 > 1.0) // стремимся к 1.0 + epsilon = 1.0
        epsilon /= 2.0;
}

double getSpecialEpsilon(double x) // получение машинного эпсилон делением для конкретного числа
{
    x = fabs(x);
    double eps = x;

    while (x + eps / 2 > x) // стремимся к x + eps = x
        eps /= 2.0;
    return eps;
}

double func(double x) // значение нашей функции(арктангенс) в точке x через встроенную ф-ию atan()
{
    return atan(x);
}

void getNextSeriesElement(double x, int *iteration, double *element) // получаем iteration+1-ый член ряда для функции в точке x
{
    double current_iteration = *iteration;
    if (current_iteration == 0) // первый элемент ряда = x
    {
        *element = x;
    }
    else // n-ый эл-т ряда = n-1-ый * (x^2(2n-1))/(2n+1)
    {
        *element *= (-1.0) * x * x * (2.0 * (current_iteration)-1.0) / (2.0 * (current_iteration) + 1.0);
    }
    *iteration += 1; // счетчик итераций привязан к расчету эл-тов ряда
}

void getTaylorSeries(double x, double *series_sum, int *series_terms_amount) // частичная сумма и кол-во эл-ов ряда тейлора нашей функции в точке x
{
    double current_sum = 0.0;     // переменная под частичную сумма ряда
    double current_element = 0.0; // переменная под текущий член ряда
    int interation = 0;           // номер текущего члена ряда(ряд суммируется начиная с n = 0)

    do
    {
        getNextSeriesElement(x, &interation, &current_element); // вычисляем элемент ряда
        current_sum += current_element;                         // плюсуем к сумме
    } while (fabs(current_element) > getSpecialEpsilon(current_sum) && interation + 1 < 100); // стремимся к 1 + epsilon = 1

    *series_sum = current_sum;         // возвращаем сумму ряда
    *series_terms_amount = interation; // возвращаем кол-во эл-тов ряда
}

int main(void) // мейн
{
    double a = 0.0; // начала исходного отрезка
    double b = 0.5; // конец исходного отрезка
    int n;          // переменная под кол-во частей, на которые будет разбит отрезок [a, b]

    setMachineEpsilon(); // вычислили машинное эпсилон

    printf("Enter the number of splits for the segment [%.1lf, %.1lf]: ", a, b);              // введите n
    scanf("%d", &n);                                                                          // получаем n из ввода
    printf("\nMachine epsilon for 1.0 is equal to: %.20lf\n\n", epsilon);                     // вывод машинного эпсилон
    printf("Table for values of Taylor series and of base function\n");                       // шапка таблицы
    printf("----------------------------------------------------------------------------\n"); //
    printf("| step |   x   |      series_sum      |       atan(x)        | iterations  |\n"); //
    printf("----------------------------------------------------------------------------\n"); //

    double step = (b - a) / (double)n; // рассчитываем длинну отрезков (шаг точки x с каждой итерацией)

    double currentX;             // переменная под текущую точку x
    double series_sum;           // переменная под текущую частичную сумму ряда
    int series_terms_amount;     // переменная под текущее кол-во эл-тов в частичной сумме ряда
    for (int i = 0; i <= n; ++i) // перебираем все n + 1 точек x на [0.0, 0.5] с шагом step
    {
        currentX = a + step * (double)i;                              // рассчитываем текущую точку x
        getTaylorSeries(currentX, &series_sum, &series_terms_amount); // вычисляем текущие сумму и кол-во эл-ов ряда

        printf("|  %2d  | %.3lf | %.18lf | %.18lf |     %3d     |\n", i, currentX, series_sum, func(currentX), series_terms_amount); // выводим строку таблицы
        printf("----------------------------------------------------------------------------\n");
    }
    printf("\n\n\nFirst %2d elements of Taylor series for x = 0.5\n", n);
    double elem;
    int iter = 0;
    for (int i = 0; i <= n; i++)
    {
        getNextSeriesElement(0.5, &iter, &elem);
        printf("%3d:%24.18lf\n", i, elem);
    }

    printf("\n\n");
    printf("%.20lf\n", getSpecialEpsilon(0.001));
    printf("%.20lf\n", getSpecialEpsilon(1.0));
    printf("%.20lf\n", getSpecialEpsilon(100.0));
    printf("%.20lf\n", getSpecialEpsilon(1000000.0));

    return 0;
}