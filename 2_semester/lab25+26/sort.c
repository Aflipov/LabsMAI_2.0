#include <limits.h>
#include "sort.h"

void selection_sort_stack(Stack *s, int order)
{
    Stack *tmp = create_stack(s->capacity);
    Stack *sorted = create_stack(s->capacity);

    while (!is_empty(s))
    {
        int extremum = (order == 1 ? INT_MAX : INT_MIN);

        // найти минимум (для возрастания) или максимум (для убывания)
        while (!is_empty(s))
        {
            int x = pop(s);
            if ((order == 1 && x < extremum) || (order == -1 && x > extremum))
            {
                extremum = x;
            }
            push(tmp, x);
        }

        // переложить обратно, пропуская найденный элемент
        int skipped = 0;
        while (!is_empty(tmp))
        {
            int x = pop(tmp);
            if (x == extremum && !skipped)
            {
                skipped = 1;
            }
            else
            {
                push(s, x);
            }
        }

        // найденный минимум/максимум кладём в "отсортированный" стек
        push(sorted, extremum);
    }

    // переносим всё обратно из sorted → s
    while (!is_empty(sorted))
    {
        push(s, pop(sorted));
    }

    free_stack(tmp);
    free_stack(sorted);
}
