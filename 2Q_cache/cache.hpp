#include <list>
#include <unordered_map>
#include <iterator>
#include <stdio.h>
#include <assert.h>

int slow_get_page(int key) {
    return key;
}

template <typename elem_t, typename key_t = int>
struct hash_map {
    size_t max_size;
    //hash_map(size_t size) : max_size(size) {}

    int cycle = 0;

    using list_iterator = typename std::list<elem_t>::iterator;
    std::unordered_map<key_t, list_iterator> map;

    bool full() const {
        return map.size() >= max_size;
    }

    void inc_cycle() {
        cycle = (cycle + 1) % max_size;
    }
};



template <typename elem_t, typename key_t = int>
struct cache_t {
    const double out_ratio = 0.6;
    const double hot_ratio = 0.2;

    size_t cache_size;

    const size_t out_size = (size_t) ((double) cache_size * out_ratio);
    const size_t hot_size = (size_t) ((double) cache_size * hot_ratio);
    const size_t in_size  = cache_size - (out_size + hot_size);

    std::list<elem_t> cache_list;

    hash_map<elem_t, key_t> in_map;
    hash_map<elem_t, key_t> out_map;
    hash_map<elem_t, key_t> hot_map;

    cache_t(size_t size) : cache_size(size < 5 ? 5 : size) {
        if (size < 5) {
            printf("The specified cache size is too small, set to default value (5)\n");
        } 
        in_map.max_size  = in_size;
        out_map.max_size = out_size;
        hot_map.max_size = hot_size;
    }

    bool lookup_update(key_t key) {
        auto hash_it = in_map.map.find(key);
        if (hash_it != in_map.map.end()) {
            return true;
        }

        hash_it = hot_map.map.find(key);
        if (hash_it != hot_map.map.end()) {
            return true;
        }

        hash_it = out_map.map.find(key);
        if (hash_it != out_map.map.end()) {
            if (hot_map.full()) {
                auto hot_elem_it = hot_map.map.begin();
                std::advance(hot_elem_it, hot_map.cycle);
                cache_list.erase(hot_elem_it->second);
                hot_map.map.erase(hot_elem_it);
            }

            hot_map.map[key] = hash_it->second;
            hot_map.inc_cycle();
            out_map.map.erase(hash_it);

            return true;
        }

        if (in_map.full()) {
            if (out_map.full()) {
                auto out_elem_it = out_map.map.begin();
                std::advance(out_elem_it, out_map.cycle);
                cache_list.erase(out_elem_it->second);
                out_map.map.erase(out_elem_it);
            }

            auto in_elem_it = in_map.map.begin();
            std::advance(in_elem_it, in_map.cycle);
            out_map.map[in_elem_it->first] = in_elem_it->second;
            out_map.inc_cycle();
            in_map.map.erase(in_elem_it);
        }

        cache_list.push_front(slow_get_page(key));
        in_map.map[key] = cache_list.begin();
        in_map.inc_cycle();

        return false;
    }

    void print_state() {
        printf("---------------------------------------------------\n");
        printf("\ncache_size = %d;\n", cache_size);
        printf("cache_list: { ");
        for (int n : cache_list) printf("%d, ", n);

        printf("};\n\nin_size = %d (%d);\ncycle = %d\n\n", in_map.map.size(), in_map.max_size, in_map.cycle);
        for (const auto& [key, value] : in_map.map) {
            printf("Key [%d] Value [%d]\n", key, *value);
        }

        printf("\nout_size = %d (%d);\ncycle = %d\n\n", out_map.map.size(), out_map.max_size, out_map.cycle);
        for (const auto& [key, value] : out_map.map) {
            printf("Key [%d] Value [%d]\n", key, *value);
        }

        printf("\nhot_size = %d (%d);\ncycle = %d\n\n", hot_map.map.size(), hot_map.max_size, hot_map.cycle);
        for (const auto& [key, value] : hot_map.map) {
            printf("Key [%d] Value [%d]\n", key, *value);
        }
        printf("---------------------------------------------------\n");
    }
};