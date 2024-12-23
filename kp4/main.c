#include <stdio.h>
#include <math.h>

int x1_iterations, x2_iterations, x3_iterations; // глобальные переменные
double epsilon, A, B;                            //

void eps() // вычисляем машинное эпсилон
{
    epsilon = 1.0;
    do
    {
        epsilon /= 2.0;
    } while ((1.0 + epsilon / 2.0) > 1.0);
}


double function_23(double x) // функция из варианта 23
{
    return 3.0 * x - 4.0 * log(x) - 5.0;
}

double function_24(double x) // функция из варианта 24
{
    return cos(2.0 / x) - 2.0 * sin(1.0 / x) + 1.0 / x;
}

double function_25(double x) // функция из варианта 24
{
    return sqrt(1.0 - 0.4 * x * x) - asin(x);
}

double iter_function_23(double x) // функция из варианта 23 для итерационного метода
{
    return x - function_23(x);
}

double iter_function_24(double x) // функция из варианта 24 для итерационного метода
{
    return x - function_24(x);
}

double iter_function_25(double x) // функция из варианта 25 для итерационного метода
{
    return x + function_25(x);
}

double derivative_23(double x) // производная функции из варианта 23
{
    return 3.0 - 4.0 / x;
}

double derivative_24(double x) // производная функции из варианта 24
{
    return (2.0 * sin(2.0 / x) + 2.0 * cos(1.0 / x - 1.0)) / (x * x);
}

double derivative_25(double x) // производная функции из варианта 25
{
    return -(2.0 * x) / (sqrt(25.0 - 10.0 * x * x)) - (1.0) / (sqrt(1.0 - x * x));
}


double Dichotomy_Method(double (*function)(double x)) // метод дихотомии (половинного деления)
{
    double a = A;      // начало отрезка
    double b = B;      // конец отрезка
    int iteration = 0; // счетчик итераций
    do
    {
        double middle = (a + b) / 2.0;            // делим отрезок пополам
        if (function(a) * function(middle) > 0.0) // отбрасываем отрезок без корня
        {
            a = middle;
        }
        else
        {
            b = middle;
        }
        iteration++;
    } while (fabs(a - b) > epsilon && iteration < 50);

    x1_iterations = iteration;

    return (a + b) / 2.0;
}

double Iterations_Method(double (*iter_function)(double x)) // метод итераций
{
    double x = (A + B) / 2; // начальное приближение корня = середина исходного отрезка
    double next_x = 0;      // переменная под следующее приближение корня
    int iteration = 0;      // счетчик итераций
    do
    {
        next_x = iter_function(x);                         // вычисляем следующее приближение корня
        if (fabs(next_x - x) < epsilon || iteration >= 50) // останавливаемся если шаг меньше эпсилон
        {
            break;
        }
        x = next_x; // принимаем следующее приближение корня за текущее
        iteration++;
    } while (1);

    x2_iterations = iteration;

    return next_x; // возвращаем приближенное значение корня
}

double Newtons_Method(double (*function)(double x), double (*derivative)(double x)) // метод Ньютона
{
    double x = (A + B) / 2.0; // начальное приближение корня = середина исходного отрезка
    double next_x;            // переменная под следующее приближение корня
    int iteration = 0;        // счетчик итераций
    do
    {
        next_x = x - function(x) / derivative(x);          // вычисляем следующее приближение корня
        if (fabs(next_x - x) < epsilon || iteration >= 50) // останавливаемся если шаг меньше эпсилон
        {
            break;
        }
        x = next_x; // принимаем следующее приближение корня за текущее
        iteration++;
    } while (1);

    x3_iterations = iteration;

    return next_x; // возвращаем приближенное значение корня
}


void Line() // вывод разделительной линии
{
    printf("|-----------------------|-----------------------|-----------------------|\n");
}

void print_results(double x1, double x2, double x3, double (*function)(double x)) // вывод результатов вычислений
{
    Line();                                                                                                                                                     //
    printf("|   dichotomy method    |   iteration method    |    Newton's method    |    methods\n");                                                           // шапка
    Line();                                                                                                                                                     //
    printf("|%22.18f |%22.18f |%22.18f |    roots\n", x1, x2, x3);                                                                                              // корни
    printf("|%22.18f |%22.18f |%22.18f |    values\n", function(x1), function(x2), function(x3));                                                               // значения в x
    printf("|         %3d           |         %3d           |         %3d           |    iterations(max = 50)\n", x1_iterations, x2_iterations, x3_iterations); // кол-во итераций
    Line();                                                                                                                                                     //
}


int main() // мейн
{
    eps();                                           // вычисляем машинное эпсилон
    printf("Machine epsilon = %.20lf\n\n", epsilon); // выводим машинное эпсилон

    A = 2.0; // вводим отрезок для функции из варианта 23
    B = 4.0; //

    double x1 = Dichotomy_Method(function_23);              // считаем приближенное значение корня функции из варианта 23 3-мя методами
    double x2 = Iterations_Method(iter_function_23);        //
    double x3 = Newtons_Method(function_23, derivative_23); //

    printf("Function: 3 * x - 4 * ln(x) - 5\nInterval: [2, 4]\n"); // шапка 1 таблицы
    print_results(x1, x2, x3, function_23);                        // выводим результаты вычислений

    printf("\n\n");

    A = 1.0; // вводим отрезок для функции из варианта 24
    B = 2.0; //

    x1 = Dichotomy_Method(function_24);              // считаем приближенное значение корня функции из варианта 24 3-мя методами
    x2 = Iterations_Method(iter_function_24);        //
    x3 = Newtons_Method(function_24, derivative_24); //

    printf("Function: cos(2/x) - 2 * sin(1/x) + 1 / x\nInterval: [1, 2]\n"); // шапка 2 таблицы
    print_results(x1, x2, x3, function_24);                                  // выводим результаты вычислений

    printf("\n\n");

    A = 0.0; // вводим отрезок для функции из варианта 25
    B = 1.0; //

    x1 = Dichotomy_Method(function_25);              // считаем приближенное значение корня функции из варианта 25 3-мя методами
    x2 = Iterations_Method(iter_function_25);        //
    x3 = Newtons_Method(function_25, derivative_25); //

    printf("Function: sqrt(1 - 0.4 * x^2) - arcsin(x)\nInterval: [0, 1]\n"); // шапка 3 таблицы
    print_results(x1, x2, x3, function_25);                                  // выводим результаты вычислений

    return 0;
}