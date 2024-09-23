#include <iostream>
#include "ideal.hpp"

const int MAX_NUMBER_OF_PAGES = 1000;

/*
Алгоритм строится на том, что сначала пользователь вводит данные, а затем происходит кеширование,
то есть программе заранее известен весь порядок ключей, по которым происходит кеширование.
Таким образом мы можем заглянуть вперед и посмотреть, какие данные мы используем еще не скоро и 
можем выкинуть.
*/

int main() {
    size_t cache_size = 0, input_elem_num = 0, hits = 0;
    std::cin >> cache_size >> input_elem_num;
    int keys[MAX_NUMBER_OF_PAGES] = {0};
    input_elem_num = (input_elem_num < MAX_NUMBER_OF_PAGES) ? input_elem_num : MAX_NUMBER_OF_PAGES;
    for (size_t i = 0; i < input_elem_num; i++) {
        std::cin >> keys[i];
    }

    ideal_cache_t<int> cache(cache_size, input_elem_num, keys);

    for (size_t i = 0; i < input_elem_num; i++) {
        if (cache.lookup_update()) {
            hits++;
        }
        //cache.print_state();
    }

    std::cout << hits << std::endl;
    return 0;
}