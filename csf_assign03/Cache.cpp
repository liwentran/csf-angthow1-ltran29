#include "Cache.h"
#include "Set.h"
#include <iostream>

using std::cout;
using std::endl;

// Constructor
Cache::Cache(int cache_size, int set_size, int block_size, bool write_allocate, write_type write_t, evict_type evict_t) : cache_size(cache_size), set_size(set_size), block_size(block_size), write_allocate(write_allocate), write_t(write_t), evict_t(evict_t) {
    //Create all sets
    for(int i = 0; i < cache_size; i++){
        Set s = Set(set_size, block_size);
        sets.push_back(s);
    }
}

int Cache::write(uint32_t address) {

    int index;
    int tag;
    int offset;

    

    /**
     * 1. We then call a function in cache that breaks the memory into tag, index, offset based on cache size (index - offset length), set size (index length), and block size (offset length).
     * The function returns 1 if a hit, and 0 if it is a miss (for counting purposes). In the function, check if the tag at corresponding index exists. 
     * If not, its a miss. We then replace a slot in the set based on LRU or FIFO
    if the tag exists, then we update it. Update the access_ts
    */

}

// Outputs the design parameters
void Cache::print_design() {
    cout << "Cache size: " << cache_size << endl 
        << "Set size: " << set_size << endl
        << "Block size: " << block_size << endl
        << "Write allocate?: " << write_allocate << endl 
        << "Write type: " << (write_t==write_through ? "write-through" : "write_back") << endl
        << "Evict type: " << (evict_t==lru ? "lru" : "fifo") << endl;
}