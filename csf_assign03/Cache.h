#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include "Set.h"
#include <stdint.h>
using std::vector;

// design parameters
enum write_type { write_through, write_back };
enum evict_type { lru, fifo };

class Cache {
    public:
    // Constructor
    Cache(int cache_size, int set_size, int block_size, bool write_allocate, write_type write_t, evict_type evict_t);

    // Destructor
    // ~Cache();

    // Output the design parameters
    void print_design();
    //returns -1 if miss, 1 if hit
    int load(uint32_t address, bool is_dirty);
    int store(uint32_t address);
    unsigned long get_cycles();

    private:

    int log2(int memory);

    // TODO: move the cache design parameters to a struct?
    int cache_size;         // number of sets in the cache (a positive power-of-2)
    int set_size;           // number of blocks in each set (a positive power-of-2)˜˜
    int block_size;         // number of bytes in each block (a positive power-of-2, at least 4)
    bool write_allocate;    // write-allocate or no-write-allocate
    write_type write_t;     // write-through or write-back
    evict_type evict_t;     // lru (least-recently-used) or fifo evictions
    unsigned long cycles;
    int access_counter;

    vector<Set> sets;       // a cache is a vector of sets
};

#endif