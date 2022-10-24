#include "Cache.h"
#include "Set.h"
#include <iostream>

using std::cout;
using std::endl;

// Constructor
Cache::Cache(int cache_size, int set_size, int block_size, bool write_allocate, write_type write_t, evict_type evict_t) : 
cache_size(cache_size), set_size(set_size), block_size(block_size), write_allocate(write_allocate), write_t(write_t), evict_t(evict_t), access_counter(0) {
    //Create all sets
    for(int i = 0; i < cache_size; i++){
        Set s = Set(set_size, block_size);
        sets.push_back(s);
    }
}

int Cache::write(uint32_t address) {

    //need to check for 0?
    int index_size = log2(set_size);
    int offset_size = log2(block_size);
    int tag_size = 32 - index_size - offset_size;


    //get index: clear tag
    int index = address << tag_size;
    //clear offset
    index >>= (tag_size + offset_size);

    //get tag
    int tag = address >> (index_size + offset_size);

    //check if tag exists in index, use map (maps tag to index)
    Set &s = sets[log2(index)];
    auto i = s.slots_map.find(tag);
    if (i != s.slots_map.end()) { // hit
        int index_of_slot = i->second;
        //update data
        s.slots[index_of_slot].mapped_memory = address;
        //update access timestamp
        s.slots[index_of_slot].access_ts = ++access_counter;
        return 1;

    } else { // miss (need to handle invalid)
        //remove logic based on LRU
        Slot lru = s.slots[0];
        int rm_idx = 0;

        //if first slot is invalid then no need to run
        if(lru.valid){
            for(int i = 1; i < s.slots.size(); i++){
                if(!s.slots[i].valid){ //if slot is invalid use it
                    rm_idx = i;
                    break;
                }
                if(s.slots[i].access_ts < s.slots[rm_idx].access_ts){
                    rm_idx = i;
                }
            }
        }

        //update map 
        //if valid
        if(s.slots[rm_idx].valid){
            s.slots_map.erase(s.slots[rm_idx].tag);
        }
        s.slots_map.insert(tag,rm_idx);

        //update data
        s.slots[rm_idx].valid = true;
        s.slots[rm_idx].tag = tag;
        s.slots[rm_idx].mapped_memory = address;
        s.slots[rm_idx].access_ts = ++access_counter;
        return -1;
    }


    

    /**
     * 1. We then call a function in cache that breaks the memory into tag, index, offset based on cache size (index - offset length), set size (index length), and block size (offset length).
     * The function returns 1 if a hit, and 0 if it is a miss (for counting purposes). In the function, check if the tag at corresponding index exists. 
     * If not, its a miss. We then replace a slot in the set based on LRU or FIFO
    if the tag exists, then we update it. Update the access_ts
    */

}

    int log2(int memory){
        int result = 0;
        while (memory >>= 1) result++; 
        return result;
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