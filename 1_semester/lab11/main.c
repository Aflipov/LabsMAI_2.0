#include <stdio.h>
#include <stdlib.h>  // для atol
#include <string.h>  // для операций над строками
#include <ctype.h>   // для isdigit
#include <stdbool.h> // для true false

#define MAX_NUMBER_LENGTH 6 // максимальная длина числа "65535", один байт нужен для '\0'
#define MAX_LINE_LENGTH 256 // максимальная длина строки

bool isValidNumber(const char *str) // проверка числа
{
    int len = strlen(str);

    if (len == 0 || len > MAX_NUMBER_LENGTH - 1) // один байт нужен для '\0'
        return false;

    for (int i = 0; i < len; i++) // точно ли число из цифр
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }

    int num = atol(str);        // приводим стр к инту
    if (num < 1 || num > 65535) // проверка на размер числа
        return false;
    return true;
}

int count_valid_numbers(const char *line)
{
    int count = 0;          // счетчик подошедших чисел
    int i = 0;              // индекс рассматриваемого в данный момент символа строки
    int len = strlen(line); // длина строки
    char number_str[MAX_NUMBER_LENGTH];

    while (i < len)
    {

        while (i < len && !isdigit(line[i])) // пропускаем нецифровые символы
        {
            i++;
        }

        if (i == len) // выходим в конце строки
            break;

        int start = i; // индекс начало числа

        while (i < len && isdigit(line[i])) // ищем индекс конеца числа
        {
            i++;
        }

        int number_len = i - start;
        if (number_len >= MAX_NUMBER_LENGTH)
        {
            i++;
            continue; // проверка на длинну числа
        }
        strncpy(number_str, line + start, number_len); // копируем найденное число в отдельную переменную
        number_str[number_len] = '\0';                 // ставим в комне строки символ завершения строки

        if (isValidNumber(number_str)) // проверяем на корректность начденное число
        {
            printf("%s\n", number_str);
            count++;
        }
    }
    return count;
}

int main()
{
    char line[MAX_LINE_LENGTH];

    printf("Enter some digits: ");
    fgets(line, sizeof(line), stdin);                                                 // ввод строки
    line[strcspn(line, "\n")] = 0;                                                    // убираем возножный символ \n
    printf("The number of positive 16-bit numbers: %d\n", count_valid_numbers(line)); // обработка строки и вывод результата
    return 0;
}