#include <stdio.h>
#include <ctype.h>    // для tolower
#include <inttypes.h> // для uint64_t
#include <stdbool.h>  // для true false

bool new_word(int symbol) // true если символ - разделитель
{
    return (symbol == '\n' || symbol == '\t' || symbol == ',' || symbol == ' ') ? true : false;
}

uint64_t char_to_set(char c) // если символ принадлежит латинскому алфавиту, вернет его битовую маску
{
    c = tolower(c);
    if (c < 'a' || c > 'z')
        return 0;
    else
        return 1u << (c - 'a');
}

// битовая маска всех неглухих согласных
const uint64_t consonants = (1u << ('b' - 'a') | 1u << ('c' - 'a') | 1u << ('d' - 'a') | 1u << ('g' - 'a') |
                             1u << ('j' - 'a') | 1u << ('k' - 'a') | 1u << ('l' - 'a') | 1u << ('m' - 'a') |
                             1u << ('n' - 'a') | 1u << ('q' - 'a') | 1u << ('r' - 'a') | 1u << ('v' - 'a') |
                             1u << ('w' - 'a') | 1u << ('x' - 'a') | 1u << ('z' - 'a'));

// битовая маска всех гласных
const uint64_t vowels = (1u << ('a' - 'a') | 1u << ('e' - 'a') | 1u << ('i' - 'a') |
                         1u << ('o' - 'a') | 1u << ('u' - 'a') | 1u << ('y' - 'a'));

// битовая маска всех глухих согласных
const uint64_t voiceless_consonants = (1u << ('k' - 'a') | 1u << ('p' - 'a') | 1u << ('s' - 'a') |
                                       1u << ('t' - 'a') | 1u << ('f' - 'a') | 1u << ('h' - 'a'));

int main()
{
    int c;                        // переменная для хранения текущего введенного символа
    unsigned int letters_set = 0; // Битовая маска, которая хранит все буквы, встретившиеся в текущем слове
    unsigned int last_letter_set; // Битовая маска, хранит значение letters_set на предыдущей итерации цикла
    while ((c = getchar()) != '*')
    {
        if (new_word(c))
        {
            if (letters_set == 0 || new_word(last_letter_set)) // если слово не содержит букв или если предыдущий символ также является разделителем, то текущее слово пропускается
                continue;
            if (letters_set & consonants) // если в слове есть согласные
                printf("No\n");
            else if (letters_set & voiceless_consonants & vowels) // если в слове есть одновременно глухие согласные и гласные
                printf("Yes\n");
            else if (letters_set & voiceless_consonants) // если в слове есть глухие согласные
                printf("Yes\n");
            else
                printf("No\n");
            letters_set = 0; // брос битовой маски для нового слова
        }

        last_letter_set = letters_set; // сохранение значения letters_set в last_letter для следующей итерации

        letters_set = letters_set | char_to_set(c); //  добавление битовой маски для текущего символа в битовую маску слова
    }
    return 0;
}