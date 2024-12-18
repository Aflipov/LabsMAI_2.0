#include <stdio.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <assert.h>

bool new_word (int symbol) {
    return (symbol == '\n' || symbol == '\t' || symbol == ',' || symbol == ' ') ? true : false;
}

uint64_t char_to_set(char c) {
    c = tolower(c);
    if (c < 'a' || c > 'z')
        return 0;
    else
        return 1u << (c - 'a');
}

void test_new_word() {
    assert(new_word(' ') == true);
    assert(new_word('\t') == true);
    assert(new_word('t') == false);
}

void test_char_to_set() {
    assert(char_to_set('a') == 1);
    assert(char_to_set('3') == 0);
}

void all_tests() {
    test_new_word();
    test_char_to_set();
}

const uint64_t CONSONANTS = (
        1u << ('b' - 'a') | 1u << ('c' - 'a') | 1u << ('d' - 'a') | 1u << ('g' - 'a') |
        1u << ('j' - 'a') | 1u << ('k' - 'a') | 1u << ('l' - 'a') | 1u << ('m' - 'a') |
        1u << ('n' - 'a') | 1u << ('q' - 'a') | 1u << ('r' - 'a') |
        1u << ('v' - 'a') | 1u << ('w' - 'a') | 1u << ('x' - 'a') | 1u << ('z' - 'a')
);

const uint64_t vowels = (
        1u << ('a' - 'a') | 1u << ('e' - 'a') | 1u << ('i' - 'a') | 1u << ('o' - 'a') | 1u << ('u' - 'a') | 1u << ('y' - 'a')
);

const uint64_t moe = (
//        к - k
//        п - p
//        с - s
//        т - t
//        ф - f
//        х - h

        1u << ('k' - 'a') | 1u << ('p' - 'a') | 1u << ('s' - 'a') | 1u << ('t' - 'a') | 1u << ('f' - 'a') | 1u << ('h' - 'a')
);

int main() {
    all_tests();

    char set;
    int c;
    unsigned int letters_set = 0, last_letter;
    while ((c = getchar()) != '*') {
        if (new_word(c)) {
            if (letters_set == 0 || new_word(last_letter))
                continue;
            if (letters_set & CONSONANTS)
                printf("No\n");
            else if (letters_set & moe & vowels)
                printf("Yes\n");
            else if (letters_set & moe)
                printf("Yes\n");
            else
                printf("No\n");
            letters_set = 0;
        }

        last_letter = letters_set;

        letters_set = letters_set | char_to_set(c);
    }
    return 0;
}