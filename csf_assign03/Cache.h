#ifndef CACHE_H
#define CACHE_H

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

    private:

    int cache_size;         // number of sets in the cache (a positive power-of-2)
    int set_size;           // number of blocks in each set (a positive power-of-2)˜˜
    int block_size;         // number of bytes in each block (a positive power-of-2, at least 4)
    bool write_allocate;    // write-allocate or no-write-allocate
    write_type write_t;     // write-through or write-back
    evict_type evict_t;     // lru (least-recently-used) or fifo evictions

    // TODO: a cache is a vector of sets, a set is a vector of slots (slots have tag, a boolean for valid; information about the stuff)
};

#endif