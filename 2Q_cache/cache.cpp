#include <iostream>
#include "cache.hpp"

int main() {
    size_t cache_size = 0, input_elem_num = 0, hits = 0;
    std::cin >> cache_size >> input_elem_num;
    cache_t<int> cache(cache_size);

    for (size_t page = 0; page < input_elem_num; page++) {
        int key = 0;
        std::cin >> key;
        if (cache.lookup_update(key)) {
            hits++;
        }
        // cache.print_state();
    }

    std::cout << hits << std::endl;
    return 0;
}