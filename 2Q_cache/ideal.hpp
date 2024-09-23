#include <list>
#include <unordered_map>
#include <iterator>
#include <stdio.h>

int slow_get_page(int key) {
    return key;
}

template <typename elem_t, typename key_t = int>
struct ideal_cache_t {
    size_t cache_size;
    size_t keys_num;
    int   *keys;
    int    curent_page = 0;

    std::list<elem_t> cache_list;

    using ListIt = typename std::list<elem_t>::iterator;
    std::unordered_map<key_t, ListIt> cache_hash;

    ideal_cache_t(size_t size, size_t data_n, int* data) : cache_size(size) {
        keys_num = data_n;
        keys = data;
    }

    bool full() {
        return cache_list.size() >= cache_size;
    }

    bool lookup_update() {
        auto hash_it = cache_hash.find(keys[curent_page]);
        if (hash_it == cache_hash.end()) {
            if (full()) {
                // Если хэш переполнен, ищем страницу, которую надо выкинуть.
                key_t key_to_del = most_far_page();
                cache_list.erase(cache_hash[key_to_del]);
                cache_hash.erase(key_to_del);
            }
            cache_list.push_front(slow_get_page(keys[curent_page]));
            cache_hash[keys[curent_page]] = cache_list.begin();
            curent_page++;
            return false;
        }
        curent_page++;
        return true;
    }

    key_t most_far_page() {
        std::unordered_map<key_t, bool> counter;
        key_t most_far = -1;
        // Будем идти по массиву ключей и проверять каждый ключ на наличие в кэше.
        // Чтоб не проверять ключи повторно, в таблице counter будем отмечать, проверен он или нет.

        for (int i = curent_page + 1; i < keys_num; i++) {
            if (counter.find(keys[i]) == counter.end()) {
                counter[keys[i]] = true;
                if (cache_hash.find(keys[i]) != cache_hash.end()) {
                    most_far = keys[i];
                }
            }
        }
        // Если в итоге в очереди не оказалось ключей, которые в данный момент лежат в кэше
        // вернем любой (конкретно, самый первый) элемент, потому что в будущем мы его ни разу не встретим.
        return (most_far != -1) ? most_far : cache_hash.begin()->first;
    }

    void print_state() {
        printf("---------------------------------------------------\n");
        printf("\ncache_size = %d;\n", cache_size);
        printf("cache_list: { ");
        for (int n : cache_list) printf("%d, ", n);

        printf("};\n\nhash_size = %d;\n\n", cache_hash.size());
        for (const auto& [key, value] : cache_hash) {
            printf("Key [%d] Value [%d]\n", key, *value);
        }
        printf("---------------------------------------------------\n");
    }
};