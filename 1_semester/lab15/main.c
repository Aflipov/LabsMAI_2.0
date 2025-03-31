#include <stdio.h>
#include <inttypes.h>

void define_matrix(int size, int matrix[8][8]) // Заполняем кусок матрицы размером n*n в болванке 8*8
{
    int num = 1;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            matrix[i][j] = num;
            num += 1;
        }
    }
}

void print_matrix(int size, int matrix[8][8]) // Выводим матрицу
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int func(int size, int matrix[8][8])
{
    int imax = 0, jmax = 0; // Объявляем переменные для хранения индекса макс элемента

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (matrix[i][j] > matrix[imax][jmax]) // Ищем макс значение
            {
                imax = i;
                jmax = j;
            };
        }
    }

    int res = 0;
    for (int i = 0; i < size; ++i)
    {
        res += matrix[imax][i]; //  суммируем эл-ты в первой строке с макс эл-том матрицы
    }

    return res;
}

int main()
{
    int matrix[8][8]; // объявляем матрицу 8*8
    for (int i = 2; i < 9; i++)
    {
        printf("Для матрицы размера %d:\n", i);
        define_matrix(i, matrix);
        print_matrix(i, matrix);
        printf("%d\n", func(i, matrix));
    }

    return 0;
}