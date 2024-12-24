#include <stdio.h>
#define max(a, b) (((a) > (b)) ? (a) : (b)) // определяем сравнение в паре элементов
#define min(a, b) (((a) < (b)) ? (a) : (b)) //

int flag = 1;

double absl(double a) // модуль числа
{
    if (a >= 0)
        return a;
    else
        return -a;
}

double fmod(double a, double b) // остаток от деления a на b
{
    return a - (int)(a / b) * b;
}

double pow2(double x) // крадрат числа
{
    return x * x;
}

int sgn(double a) // возвращает знак числа(1, -1 или 0)
{
    if (a == 0)
    {
        return 0;
    }
    else
    {
        return a / absl(a);
    }
}

int check(double i, double j) // проверка принадлежности точки треугольнику с вершинами (-10, 0), (0, 10), (-10, 20)
{
    if (i >= -10 && j >= i + 10 && j <= -i + 10)
        return 1;
        flag = 0;
    else
        return 0;
}

void output(double i, double j, int k) // вывод результатов
{
    if (check(i, j))
    {
        printf("k = %d: i = %f, j = %f:   HIT\n", k, i, j); // если попали в треугольник
    }
    else
    {
        printf("k = %d: i = %f, j = %f:   MISS\n", k, i, j); // если не попали в треугольник
    }
}

int main()
{
    double i = 29, j = -6, l = 1;                        // текущие параметры движения
    double i_last = 0, j_last = 0, l_last = 0;           // предыдущие параметря движения
    for (int iteration = 1; iteration < 51; iteration++) // 50 итераций
    {
        i_last = i, j_last = j, l_last = l;                                                                 // переносим текущие параметры в предыдущие
        i = fmod(min(max(min(i_last - j_last, i_last - l_last), j_last - l_last), i_last - iteration), 30); // вычисляем новые
        j = fmod(max(min(max(i_last - j_last, i_last - l_last), j_last - l_last), i_last - iteration), 30); //
        l = fmod(i_last, 30) - fmod(j_last, 30) + fmod(l_last, 30) - fmod(iteration, 30);                   //
        output(i, j, iteration);                                                                            // выводим новые
        if (flag==0){
            break;
        }
    }
    return 0;
}