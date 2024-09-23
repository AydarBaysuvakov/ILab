#include "cache.hpp"
#include <stdio.h>
#include <ctype.h>

const int MAX_INPUT = 1000;

struct test_data {
    size_t cache_size;
    size_t input_elem_num;
    int* keys;
    size_t answer;
};

bool unit_test(size_t cache_size, size_t input_elem_num, int* keys, size_t answer) {
    size_t hits = 0;
    cache_t<int> cache(cache_size);

    for (size_t i = 0; i < input_elem_num; i++) {
        if (cache.lookup_update(keys[i])) {
            hits++;
        }
    }

    return hits == answer;
}

int test()
    {
    int tests_acepted = 0;
    int test_number = 0;
    int input[MAX_INPUT] = {0}; 

    char fname[] = "tests.txt";
    char mode[] = "r";
    FILE *fp = fopen(fname, mode);

    test_data td = {0, 0, input, 0};

    while (fscanf(fp, "%d %d", &td.cache_size, &td.input_elem_num) != EOF) {
        for (int i = 0; i < td.input_elem_num; i++) 
            fscanf(fp, "%d", td.keys + i);
        fscanf(fp, "%d", &td.answer);

        test_number++;
        char c = '\0';
        while ((c = getc(fp)) != '\n')
            {
            if (!isspace(c))
                {
                printf("WARNING: wrong test №%d\n", test_number);
                printf("Acepted %d tests from %d\n", tests_acepted, test_number);
                return tests_acepted;
                }
            }

        int feedback = unit_test(td.cache_size, td.input_elem_num, td.keys, td.answer);
        printf("TEST №%d: ", test_number);
        if (feedback) printf("ACEPTED\n");
        else printf("FAILED\n");
        tests_acepted += feedback;
        }

    printf("Acepted %d tests from %d\n", tests_acepted, test_number);
    return tests_acepted; 
    }

int main() {
    test();
}