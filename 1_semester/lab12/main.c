#include <stdio.h>

int abs(int a) // модуль числа
{
    return (a >= 0) ? a : -a;
}

int main()
{
    int number;       // переменная под введенное число
    int abs_num_copy; // переменная под копию введенного числа, изменится в процессе работы программы
    int digit;        // переменная под рассматриваемую цифру числа
    int result = 0;       // переменная под ответ
    int sign;         // переменная под знак исзодного числа (1 / -1)
    int k = 1;        // переменная под число текущее разрядов полученнного числа
    int y;            // для обработки повтора программы / выхода из нее

    printf("Введите целое число: ");
    while (scanf("%d", &number) != EOF) // считываем число
    {
        if (abs(number) < 10) // проверна на размер(число из 1 цифры - не интересно)
        {
            printf("Введено однозначное число, попробуйте еще раз.\nВведите целое число: ");
            continue; // иначе все заново
        }

        if (number < 0) // определяем знак, нужно для ответа
        {
            sign = -1;
            abs_num_copy = sign * (number / 10); // сразу отбрасываем последнюю цифру числа
        }
        else
        {
            abs_num_copy = number / 10;
            sign = 1;
        }

        while (abs_num_copy > 0) // пока число не закончилось
        {
            digit = abs_num_copy % 10;         // берем последнюю цифру сила
            result = result + digit * k;       // прибавляем ее к ответу
            k = k * 10;                        // увеличиваем число разрядов ответа
            abs_num_copy = abs_num_copy / 100; // отбрвсываем 2 цифры от числа
        }
        
        printf("Ответ: %d\n", result);

        k = 1;      // обнуляем разряды результата
        result = 0; // обнуляем результат
        printf("Продолжить? (1, если да, или любое другое число, если нет) ");
        scanf("%d", &y); // считываем ответ пользователя
        if (y != 1)
        {
            printf("Программа завершена.\n");
            printf("\n");
            break;
        }
        else
        {
            printf("\n");
            printf("Введите целое число: ");
        }
    }
    return 0;
}